#pragma once
#include <array>
#include <vector>

namespace allreadevaluation {

enum class Motion { MOTION, NO_MOTION };

/* Emulates motion every 10 seconds in a video with 25 fps */
template <typename A> class MockMotionDetector final {
public:
  MockMotionDetector() {
    /* No motion in first 250 frames (corresponds to
       10 seconds in a video with 25fps) */
    for (unsigned int i = 0; i < 250; ++i) {
      mock_motion[i] = Motion::NO_MOTION;
    }
    /* Motion after 10 seconds with a duration of 10 seconds */
    for (unsigned int i = 250; i < 500; ++i) {
      mock_motion[i] = Motion::MOTION;
    }
  }

  Motion detect(const A &previous_frame, const A &current_frame) {
    ++index;
    return mock_motion[index % mock_motion.size()];
  }

private:
  std::array<Motion, 500> mock_motion;

  std::uint64_t index = 0;
};

} // namespace allreadevaluation