#include "../include/rtsp_gstreamer_recorder.hpp"
#include "../../common/include/common.hpp"
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <sstream>

using namespace allreadevaluation;
namespace fs = std::filesystem;

RtspGstreamerRecorder::RtspGstreamerRecorder(const std::string &rtspStream,
                                             const std::string &id)
    : mRtspStream(rtspStream), mpGstPipeline(nullptr), mId(id) {}

RtspGstreamerRecorder::~RtspGstreamerRecorder() { Stop(); }

void RtspGstreamerRecorder::Start() {
  /* Creates a folder with id name to store the files */
  if (!fs::exists(mId)) {
    fs::create_directory(mId);
  }
  /* Builds file name with current date and time */
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream currentTime;
  currentTime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%X");
  /* Builds gstreamer command to start recording */
  std::string commandGstreamer = "rtspsrc location=" + mRtspStream +
                                 " latency=0 ! rtph264depay ! h264parse ! "
                                 "mpegtsmux ! filesink location=" +
                                 mId + "/" + currentTime.str();

  mpGstPipeline = gst_parse_launch(commandGstreamer.c_str(), NULL);
  if (!mpGstPipeline) {
    spdlog::get(ID_LOG)->critical("Cannot create pipeline to record stream " +
                                  mRtspStream);
  }
  gst_element_set_state(mpGstPipeline, GST_STATE_PLAYING);
  spdlog::get(ID_LOG)->info("Started recording from " + mRtspStream +
                            " in file " + mId + "/" + currentTime.str());
}

void RtspGstreamerRecorder::Stop() {
  if (mpGstPipeline) {
    gst_element_set_state(mpGstPipeline, GST_STATE_NULL);
    g_object_unref(mpGstPipeline);
    mpGstPipeline = nullptr;
    spdlog::get(ID_LOG)->info("Stopped recording from " + mRtspStream);
  }
}
