
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <gst/app/gstappsink.h>
#include <gst/gst.h>
#include <gst/video/video.h>
#include <iostream>
#include <unistd.h>

/**
 * \brief Allread app evaluation. This app gets the rtsp stream from
 *        some cameras, and start/stop recording based on motion
 *        detection. The motion detection is all mocked.
 *        GStreamer is used to get the frames from rtsp streams, and
 *        also for recording, taking advantage of its threading
 *        implementation.
 */
int main(int argc, char *argv[]) {

  auto logger = spdlog::basic_logger_mt("allreadEvaluation",
                                        "logs/allreadEvaluation.log");

  logger->info("AllReadEvaluation starts");
  return 0;
}