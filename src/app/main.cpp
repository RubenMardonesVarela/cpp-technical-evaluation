
#include "../common/include/common.hpp"
#include "../rtsp_motion_recorder_camera/include/rtsp_motion_camera.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"
#include <gst/gst.h>
#include <iostream>
#include <unistd.h>

using namespace allreadevaluation;

/**
 * \brief Allread app evaluation. This app gets the rtsp stream from
 *        some cameras, and start/stop recording based on motion
 *        detection. The motion detection is all mocked.
 *        GStreamer is used to get the frames from rtsp streams, and
 *        also for recording, taking advantage of its threading
 *        implementation.
 */
int main(int argc, char *argv[]) {

  /* Creates a multi sink log file with output to console and file */
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
  sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
      "logs/allreadEvaluation.log"));
  auto loggerAllread =
      std::make_shared<spdlog::logger>(ID_LOG, begin(sinks), end(sinks));
  spdlog::register_logger(loggerAllread);
  spdlog::get(ID_LOG)->info("AllreadEvaluation starts");

  /* Init gstreamer */
  gst_init(0, NULL);

  RtspMotionRecorderCamera cam1{"rtsp://localhost:61250/cam01", "cam01"};
  RtspMotionRecorderCamera cam2{"rtsp://localhost:61250/cam02", "cam02"};
  cam1.Start();
  cam2.Start();

  std::string consoleIn = "";
  while (consoleIn != "exit") {
    std::cin >> consoleIn;
  }

  cam1.Stop();
  cam2.Stop();
  return 0;
}