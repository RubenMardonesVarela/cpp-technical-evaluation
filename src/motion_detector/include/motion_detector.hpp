#pragma once
#include <vector>

namespace allreadevaluation {

enum class Motion { MOTION, NO_MOTION };

template <typename A> class MockMotionDetector final {
public:
  Motion detect(const A &previous_frame, const A &current_frame) {
    ++index;
    return mock_motion[index % mock_motion.size()];
  }

private:
  const std::vector<Motion> mock_motion = {
      Motion::NO_MOTION, Motion::NO_MOTION, Motion::NO_MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::MOTION,    Motion::MOTION,    Motion::MOTION,
      Motion::NO_MOTION, Motion::NO_MOTION, Motion::NO_MOTION};

  std::uint64_t index = 0;
};

} // namespace allreadevaluation