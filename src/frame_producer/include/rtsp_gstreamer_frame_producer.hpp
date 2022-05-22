#pragma once
#include "../../frame_queue/include/frame_queue.hpp"
#include <gst/app/gstappsink.h>
#include <gst/gst.h>
#include <gst/video/video.h>
#include <string>

namespace allreadevaluation {

/**
 * \brief Class which represents a producer of frames extracted
 *        from a rtsp stream. These frames are in Gstreamer format,
 *        that is, represented by object GstSample.
 */

class RtspGStreamerFrameProducer {
public:
  /* Move and copy not allowed */
  RtspGStreamerFrameProducer(RtspGStreamerFrameProducer const &) = delete;
  RtspGStreamerFrameProducer &
  operator=(RtspGStreamerFrameProducer const &) = delete;
  RtspGStreamerFrameProducer(RtspGStreamerFrameProducer &&) = delete;
  RtspGStreamerFrameProducer &operator=(RtspGStreamerFrameProducer &&) = delete;

  /**
   * \brief Constructor.
   */
  RtspGStreamerFrameProducer(FrameQueue<GstSample> &frameQueue,
                             const std::string &rtspStream);

  /**
   * \brief Destructor.
   */
  ~RtspGStreamerFrameProducer();

  /**
   * \brief Starts producing frames from rtsp.
   */
  void Start();

  /**
   * \brief Stops producing frames from rtsp.
   */
  void Stop();

private:
  /* Callback that is called when a new sample is extracted */
  static GstFlowReturn CallbackNewSample(GstAppSink *appsink, gpointer data);

  /* Reference to the queue to store the frames */
  FrameQueue<GstSample> &mFrameQueue;

  /* String that contains the rtsp stream to extract the frames  */
  std::string mRtspStream;

  /* Gstreamer pipeline to extract the frames */
  GstElement *mpGstPipeline;

  /* Sink */
  GstElement *mpSink;

  /* Total number of frames produced */
  unsigned long long mNumFramesProduced;
};

} // namespace allreadevaluation
