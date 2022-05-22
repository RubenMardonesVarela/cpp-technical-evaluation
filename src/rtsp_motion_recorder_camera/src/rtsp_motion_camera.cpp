#include "../include/rtsp_motion_camera.hpp"
#include "../../common/include/common.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

using namespace allreadevaluation;

RtspMotionRecorderCamera::RtspMotionRecorderCamera(
    const std::string &rtspStream, const std::string &id) {
  mpProducer =
      std::make_unique<RtspGStreamerFrameProducer>(mFrameQueue, rtspStream);
  mpConsumer = std::make_unique<GStreamerFrameMotionConsumer>(mFrameQueue);
  mpRecordingMotionObserver =
      std::make_unique<RecordingMotionObserver>(rtspStream, id);
}

RtspMotionRecorderCamera::~RtspMotionRecorderCamera() {}

void RtspMotionRecorderCamera::Start() {
  mpConsumer->AddMotionEventsObserver(mpRecordingMotionObserver.get());
  mpProducer->Start();
  mpConsumer->Start();
}

void RtspMotionRecorderCamera::Stop() {
  mpProducer->Stop();
  mpConsumer->Stop();
}

RtspMotionRecorderCamera::RecordingMotionObserver::RecordingMotionObserver(
    const std::string &rtspStream, const std::string &id) {
  mpRecorder = std::make_unique<RtspGstreamerRecorder>(rtspStream, id);
}

RtspMotionRecorderCamera::RecordingMotionObserver::~RecordingMotionObserver() {
  mpRecorder->Stop();
  mCurrentStatus = Status::NOT_MOVING;
}

void RtspMotionRecorderCamera::RecordingMotionObserver::NotifyMotionEvent() {
  if (mCurrentStatus == Status::NOT_MOVING) {
    mCurrentStatus = Status::MOVING;
    mpRecorder->Start();
  }
}

void RtspMotionRecorderCamera::RecordingMotionObserver::NotifyNoMotionEvent() {
  if (mCurrentStatus == Status::MOVING) {
    mCurrentStatus = Status::NOT_MOVING;
    mpRecorder->Stop();
  }
}