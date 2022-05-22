#pragma once

namespace allreadevaluation {

/**
 * \brief Abstrat class which represents an observer of motion events.
 */
class IEventsMotionObserver {

public:
  /**
   * \brief     Notify a motion event.
   */
  virtual void NotifyMotionEvent() = 0;

  /**
   * \brief     Notify a no motion event.
   */
  virtual void NotifyNoMotionEvent() = 0;
};

} // namespace allreadevaluation