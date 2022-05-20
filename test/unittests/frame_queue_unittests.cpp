
#include "../../src/frame_queue/include/frame_queue.hpp"
#include "thread"
#include "gtest/gtest.h"
#include <chrono>
#include <memory>

using namespace allreadevaluation;

/**
 * \brief Unit tests for FrameQueue class
 */

/* Class representing a dummy frame with an id */
class DummyFrame {
public:
  DummyFrame(unsigned int idFrame) : mIdFrame(idFrame){};
  ~DummyFrame(){};
  unsigned int mIdFrame;
};

/* Class representing a dummy frames producer which
   produces s configurable number of frames,
   one every 10 milliseconds */
class DummyFramesProducer {
public:
  DummyFramesProducer(unsigned int numFramesToProduce,
                      FrameQueue<DummyFrame> &dummyFrameQueue)
      : mNumFramesToProduce(numFramesToProduce),
        mDummyFrameQueue(dummyFrameQueue){};
  ~DummyFramesProducer() { WaitForFinish(); };
  void Start() {
    mProducerThread = std::thread([this]() {
      unsigned int iFrame = 0;
      while (iFrame < mNumFramesToProduce) {
        auto frame = std::make_unique<DummyFrame>(iFrame++);
        mDummyFrameQueue.PushFrameAndNotify(std::move(frame).get());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
      Stop();
    });
  }

  void Stop() { mDummyFrameQueue.StopAndNotify(); }
  void WaitForFinish() {
    if (mProducerThread.joinable()) {
      mProducerThread.join();
    }
  }
  unsigned int mNumFramesToProduce;
  FrameQueue<DummyFrame> &mDummyFrameQueue;
  std::thread mProducerThread;
};

/* Class representing a dummy frames consumer */
class DummyFramesConsumer {
public:
  DummyFramesConsumer(FrameQueue<DummyFrame> &dummyFrameQueue)
      : mDummyFrameQueue(dummyFrameQueue){};
  ~DummyFramesConsumer() { WaitForFinish(); };
  void Start() {
    mConsumerThread = std::thread([this]() {
      auto [m, cv] = mDummyFrameQueue.Subscribe();
      while (true) {
        std::unique_lock l{m};
        cv.wait(l, [&] {
          return mDummyFrameQueue.IsStopped() || mDummyFrameQueue.HasFrames();
        });
        if (mDummyFrameQueue.IsStopped()) {
          break;
        } else {
          ConsumeFrame(mDummyFrameQueue.GetNextFrame());
        }
      }
    });
  }

  void ConsumeFrame(DummyFrame *frame) { mFramesConsumed.push_back(frame); }

  void WaitForFinish() {
    if (mConsumerThread.joinable()) {
      mConsumerThread.join();
    }
  }

  FrameQueue<DummyFrame> &mDummyFrameQueue;
  std::thread mConsumerThread;
  std::vector<DummyFrame *> mFramesConsumed;
};

TEST(FrameQueue, InitializesRunningAndEmpty) {
  FrameQueue<DummyFrame> dummyFrameQueue;
  EXPECT_FALSE(dummyFrameQueue.IsStopped());
  EXPECT_FALSE(dummyFrameQueue.HasFrames());
}

TEST(FrameQueue, ProduceFramesWithoutConsuming) {
  FrameQueue<DummyFrame> dummyFrameQueue;
  unsigned int numFramesToProduce = 10;
  DummyFramesProducer dummyFramesProducer{numFramesToProduce, dummyFrameQueue};
  dummyFramesProducer.Start();
  dummyFramesProducer.WaitForFinish();
  /* No frames must have been consumed by a consumer */
  EXPECT_EQ(dummyFrameQueue.GetNumberFrames(), numFramesToProduce);
  EXPECT_TRUE(dummyFrameQueue.IsStopped());
}

TEST(FrameQueue, ProduceFramesWithConsuming) {
  FrameQueue<DummyFrame> dummyFrameQueue;
  unsigned int numFramesToProduce = 10;
  DummyFramesProducer dummyFramesProducer{numFramesToProduce, dummyFrameQueue};
  DummyFramesConsumer dummyFramesConsumer{dummyFrameQueue};
  dummyFramesProducer.Start();
  dummyFramesConsumer.Start();
  dummyFramesProducer.WaitForFinish();
  /* All the frames must have been consumed by the consumer */
  EXPECT_EQ(dummyFrameQueue.GetNumberFrames(), 0);
  EXPECT_EQ(dummyFramesConsumer.mFramesConsumed.size(), numFramesToProduce);
}