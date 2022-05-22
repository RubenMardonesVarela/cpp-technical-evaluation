#pragma once
#include "../../frame_queue/include/frame_queue.hpp"
#include "../../motion_detector/include/motion_detector.hpp"
#include "../../motion_observer/include/motion_observer.hpp"
#include <gst/gst.h>
#include <thread>

namespace allreadevaluation {

/**
 * \brief Class which represents a consumer of frames to detect motion.
 *        Frames are in Gstreamer format, that is,represented by object
 *        GstSample.
 * \note  Consumption takes place in a separate thread.
 *
 */

class GStreamerFrameMotionConsumer {
public:
  /* Move and copy not allowed */
  GStreamerFrameMotionConsumer(GStreamerFrameMotionConsumer const &) = delete;
  GStreamerFrameMotionConsumer &
  operator=(GStreamerFrameMotionConsumer const &) = delete;
  GStreamerFrameMotionConsumer(GStreamerFrameMotionConsumer &&) = delete;
  GStreamerFrameMotionConsumer &
  operator=(GStreamerFrameMotionConsumer &&) = delete;

  /**
   * \brief Constructor.
   */
  GStreamerFrameMotionConsumer(FrameQueue<GstSample> &frameQueue);

  /**
   * \brief Destructor.
   */
  ~GStreamerFrameMotionConsumer();

  /**
   * \brief Starts consuming frames.
   */
  void Start();

  /**
   * \brief Stops consuming frames.
   */
  void Stop();

  /**
   * \brief Add a motion events observer.
   */
  void AddMotionEventsObserver(IEventsMotionObserver *pEventsMotionObserver);

private:
  /**
   * \brief     Consume one frame.
   * \param[in] frame pointer to the frame to consume.
   */
  void ConsumeFrame(GstSample *frame);

  /* Reference to the queue to consume the frames */
  FrameQueue<GstSample> &mFrameQueue;

  /* Mock motion detector */
  MockMotionDetector<GstSample> mMotionDetector;

  /* Thread used for consuming frames */
  std::thread mConsumerThread;

  /* Total number of frames consumed */
  unsigned long long mNumFramesConsumed;

  /* Previous frame */
  GstSample *mpPreviousFrame;

  /* Current frame */
  GstSample *mpCurrentFrame;

  /* Observers to notify motion events */
  std::vector<IEventsMotionObserver *> mMotionObservers;
};

} // namespace allreadevaluation
