#pragma once
#include "../../frame_consumer/include/gstreamer_frame_motion_consumer.hpp"
#include "../../frame_producer/include/rtsp_gstreamer_frame_producer.hpp"
#include "../../recorder/include/rtsp_gstreamer_recorder.hpp"
#include <memory>

namespace allreadevaluation {

/**
 * \brief Class which represents a flow that gets an rtsp stream from a camera
 *        and start/stop recording based on motion detection.
 */

class RtspMotionRecorderCamera {
public:
  /* Move and copy not allowed */
  RtspMotionRecorderCamera(RtspMotionRecorderCamera const &) = delete;
  RtspMotionRecorderCamera &
  operator=(RtspMotionRecorderCamera const &) = delete;
  RtspMotionRecorderCamera(RtspMotionRecorderCamera &&) = delete;
  RtspMotionRecorderCamera &operator=(RtspMotionRecorderCamera &&) = delete;

  /**
   * \brief Constructor.
   */
  RtspMotionRecorderCamera(const std::string &rtspStream,
                           const std::string &id);

  /**
   * \brief Destructor.
   */
  ~RtspMotionRecorderCamera();

  /**
   * \brief Start.
   */
  void Start();

  /**
   * \brief Stop.
   */
  void Stop();

  /**
   * \brief Class which represents an observer of motion events
   *        that trigger start/stop recording based on these
   *        events.
   */
  class RecordingMotionObserver : public IEventsMotionObserver {
  public:
    RecordingMotionObserver(const std::string &rtspStream,
                            const std::string &id);
    ~RecordingMotionObserver();
    enum class Status { MOVING, NOT_MOVING };

    /* Interface from IEventsMotionObserver */
    void NotifyMotionEvent() override;
    void NotifyNoMotionEvent() override;

  private:
    Status mCurrentStatus = Status::NOT_MOVING;
    std::unique_ptr<RtspGstreamerRecorder> mpRecorder;
  };

private:
  /* Queue for producer-consumer flow */
  FrameQueue<GstSample> mFrameQueue;

  /* Frames producer from rstp stream */
  std::unique_ptr<RtspGStreamerFrameProducer> mpProducer;

  /* Frames consumer to detect motion */
  std::unique_ptr<GStreamerFrameMotionConsumer> mpConsumer;

  /* Motion observer to start/stop recording */
  std::unique_ptr<RecordingMotionObserver> mpRecordingMotionObserver;
};

} // namespace allreadevaluation
