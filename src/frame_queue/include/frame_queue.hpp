#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include <tuple>

namespace allreadevaluation {

/**
 * \brief Class which represents a queue of frames, to be used
 *        by a single producer-consumer. A producer can use the
 *        methods PushFrame to store a new frame into the queue,
 *        and stop to indicate that production on the queue is
 *        finished. Both methods notify the actions with a condition
 *        variable, so that the consumer can wait for these
 *        notifications to get a new frame, or exit its consumer
 *        thread.
 * \note  The class uses a generic type Frame.
 */

template <typename Frame> class FrameQueue {
public:
  /* Move and copy not allowed */
  FrameQueue(FrameQueue const &) = delete;
  FrameQueue &operator=(FrameQueue const &) = delete;
  FrameQueue(FrameQueue &&) = delete;
  FrameQueue &operator=(FrameQueue &&) = delete;

  /**
   * \brief Constructor.
   */
  FrameQueue() : mIsStopped(false) {}

  /**
   * \brief Destructor.
   */
  ~FrameQueue() {}

  /**
   * \brief      Push a new frame to the queue and notifies
   *             that there is a new frame through a condition
   *             variable.
   * \param[in]  pFrame pointer to the new frame.
   * \note       This method is thread safe.
   */
  void PushFrameAndNotify(Frame *pFrame) {
    {
      std::scoped_lock lock{mMutex};
      mQueue.push(pFrame);
    }
    mConditionVariable.notify_one();
  }

  /**
   * \brief      Set queue to stop state and notifies
   *             that the queue is stopped with a condition
   *             variable.
   * \note       This method is thread safe.
   */
  void StopAndNotify() {
    {
      std::scoped_lock lock{mMutex};
      mIsStopped = true;
    }
    mConditionVariable.notify_one();
  }

  /**
   * \brief   Subscribes to get the sync variables, so that
   *          a consumer can be notified with new frame or stop event.
   * \return  tuple with the mutex and condition variable for sync.
   */
  std::tuple<std::mutex &, std::condition_variable &> Subscribe() {
    return std::tuple<std::mutex &, std::condition_variable &>{
        mMutex, mConditionVariable};
  }

  /**
   * \brief   Get next frame on the queue to be consumed.
   * \return  Pointer to next frame (generic type).
   * \note    This method is non-thread safe. Consumer must
   *          ensure lock adquisition through Subscribe and wait
   *          for notitication.
   */
  Frame *GetNextFrame() {
    auto nextFrame = mQueue.front();
    mQueue.pop();
    return nextFrame;
  }

  /**
   * \brief   Check whether the queue is stopped (no more
   *          frames will be pushed).
   * \return  true when is stopped, false otherwise.
   * \note    This method is non-thread safe.Consumer must
   *          ensure lock adquisition through Subscribe and wait
   *          for notitication.
   */
  bool IsStopped() const { return mIsStopped; }

  /**
   * \brief   Check whether there are frames on the queue.
   * \return  true when there are frames, false otherwise.
   * \note    This method is non-thread safe.
   */
  bool HasFrames() const { return !mQueue.empty(); }

  /**
   * \brief   Get next frame on the queue to be consumed.
   * \return  Pointer to next frame (generic type).
   * \note    This method is non-thread safe.
   */
  size_t GetNumberFrames() const { return mQueue.size(); }

private:
  /* Queue that stores the frames */
  std::queue<Frame *> mQueue;

  /* Mutex for thread-safe operations */
  std::mutex mMutex;

  /* Condition variable to notify new frames on queue */
  std::condition_variable mConditionVariable;

  /* Boolean to indicate that queue is stopped, no more
     frames will be pushed */
  bool mIsStopped;
};

} // namespace allreadevaluation
