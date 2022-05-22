#include "../include/rtsp_gstreamer_frame_producer.hpp"
#include "../../common/include/common.hpp"
#include <spdlog/spdlog.h>

using namespace allreadevaluation;

/* Number of frames to log an alive trace from producer */
static const unsigned int NUM_FRAMES_TO_LOG_ALIVE = 100;

RtspGStreamerFrameProducer::RtspGStreamerFrameProducer(
    FrameQueue<GstSample> &frameQueue, const std::string &rtspStream)
    : mFrameQueue(frameQueue), mRtspStream(rtspStream), mpGstPipeline(nullptr),
      mpSink(nullptr), mNumFramesProduced(0) {}

RtspGStreamerFrameProducer::~RtspGStreamerFrameProducer() { Stop(); }

void RtspGStreamerFrameProducer::Start() {
  std::string commandGstreamer = "rtspsrc location=" + mRtspStream +
                                 " latency=0 ! rtph264depay ! h264parse ! "
                                 "decodebin !appsink name = sink";

  mpGstPipeline = gst_parse_launch(commandGstreamer.c_str(), NULL);
  if (!mpGstPipeline) {
    spdlog::get(ID_LOG)->critical("Cannot create pipeline from " + mRtspStream);
    exit(1);
  }
  mpSink = gst_bin_get_by_name(GST_BIN(mpGstPipeline), "sink");
  if (!mpSink) {
    spdlog::get(ID_LOG)->critical("Cannot create sink from " + mRtspStream);
    exit(1);
  }

  gst_app_sink_set_emit_signals((GstAppSink *)mpSink, true);
  GstAppSinkCallbacks callbacks = {NULL, NULL, CallbackNewSample};
  gst_app_sink_set_callbacks(GST_APP_SINK(mpSink), &callbacks, this, NULL);
  gst_element_set_state(mpGstPipeline, GST_STATE_PLAYING);
  spdlog::get(ID_LOG)->info("Started frame producer from " + mRtspStream);
}

void RtspGStreamerFrameProducer::Stop() {
  if (mpGstPipeline) {
    gst_element_set_state(mpGstPipeline, GST_STATE_NULL);
    g_object_unref(mpGstPipeline);
    mpGstPipeline = nullptr;
  }
  if (mpSink) {
    g_object_unref(mpSink);
    mpSink = nullptr;
  }
  mFrameQueue.StopAndNotify();
  spdlog::get(ID_LOG)->info("Stopped frame producer from " + mRtspStream);
}

GstFlowReturn RtspGStreamerFrameProducer::CallbackNewSample(GstAppSink *appsink,
                                                            gpointer data) {
  RtspGStreamerFrameProducer *producer = (RtspGStreamerFrameProducer *)(data);
  GstSample *sample = gst_app_sink_pull_sample(appsink);
  producer->mFrameQueue.PushFrameAndNotify(sample);
  producer->mNumFramesProduced++;
  if (producer->mNumFramesProduced % NUM_FRAMES_TO_LOG_ALIVE == 0) {
    spdlog::get(ID_LOG)->info(
        "Producer from " + producer->mRtspStream +
        " alive -> frames: " + std::to_string(producer->mNumFramesProduced));
  }
  return GST_FLOW_OK;
}
