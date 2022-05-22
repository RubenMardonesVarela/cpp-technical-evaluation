#include "../include/gstreamer_frame_motion_consumer.hpp"
#include "../../common/include/common.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

using namespace allreadevaluation;

GStreamerFrameMotionConsumer::GStreamerFrameMotionConsumer(
    FrameQueue<GstSample> &frameQueue)
    : mFrameQueue(frameQueue), mNumFramesConsumed(0), mpPreviousFrame(nullptr),
      mpCurrentFrame(nullptr) {}

GStreamerFrameMotionConsumer::~GStreamerFrameMotionConsumer() { Stop(); }

void GStreamerFrameMotionConsumer::Start() {
  mConsumerThread = std::thread([this]() {
    auto [mutexQueue, cvQueue] = mFrameQueue.Subscribe();
    while (true) {
      std::unique_lock lockQueue{mutexQueue};
      cvQueue.wait(lockQueue, [&] {
        return mFrameQueue.IsStopped() || mFrameQueue.HasFrames();
      });
      if (mFrameQueue.IsStopped()) {
        break;
      } else {
        ConsumeFrame(mFrameQueue.GetNextFrame());
      }
    }
  });
}

void GStreamerFrameMotionConsumer::Stop() {
  mFrameQueue.StopAndNotify();
  if (mConsumerThread.joinable()) {
    mConsumerThread.join();
  }
  mMotionObservers.clear();
  if (mpPreviousFrame) {
    gst_sample_unref(mpPreviousFrame);
    mpPreviousFrame = nullptr;
  }
}

void GStreamerFrameMotionConsumer::AddMotionEventsObserver(
    IEventsMotionObserver *pEventsMotionObserver) {
  mMotionObservers.push_back(pEventsMotionObserver);
}

void GStreamerFrameMotionConsumer::ConsumeFrame(GstSample *frame) {
  mpCurrentFrame = frame;
  if (mpPreviousFrame && mpCurrentFrame) {
    auto motion = mMotionDetector.detect(*mpPreviousFrame, *mpCurrentFrame);
    for (auto &observer : mMotionObservers) {
      if (motion == Motion::MOTION) {
        observer->NotifyMotionEvent();
      } else {
        observer->NotifyNoMotionEvent();
      }
    }
  }
  if (mpPreviousFrame) {
    gst_sample_unref(mpPreviousFrame);
  }
  mpPreviousFrame = mpCurrentFrame;
}
