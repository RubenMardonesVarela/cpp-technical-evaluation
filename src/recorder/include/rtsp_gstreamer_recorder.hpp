#pragma once
#include <gst/app/gstappsink.h>
#include <gst/gst.h>
#include <gst/video/video.h>
#include <string>

namespace allreadevaluation {

/**
 * \brief Class which represents a recorder which stores an
 *        rtsp stream to a file using gstreamer.
 */

class RtspGstreamerRecorder {
public:
  /* Move and copy not allowed */
  RtspGstreamerRecorder(RtspGstreamerRecorder const &) = delete;
  RtspGstreamerRecorder &operator=(RtspGstreamerRecorder const &) = delete;
  RtspGstreamerRecorder(RtspGstreamerRecorder &&) = delete;
  RtspGstreamerRecorder &operator=(RtspGstreamerRecorder &&) = delete;

  /**
   * \brief Constructor.
   */
  RtspGstreamerRecorder(const std::string &rtspStream, const std::string &id);

  /**
   * \brief Destructor.
   */
  ~RtspGstreamerRecorder();

  /**
   * \brief Starts recording.
   */
  void Start();

  /**
   * \brief Stops recording.
   */
  void Stop();

private:
  /* String that contains the rtsp stream to record  */
  std::string mRtspStream;

  /* Gstreamer pipeline to record the stream to a file */
  GstElement *mpGstPipeline;

  /* Identifier of recorder. It is used to create the location
     and the base name for record files */
  std::string mId;
};

} // namespace allreadevaluation
