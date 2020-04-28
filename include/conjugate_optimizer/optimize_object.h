/*
 * @Author: lemoxit
 * @Date: 2020-04-28 23:30:30
 * @LastEditTime: 2020-04-29 01:28:36
 * @LastEditors: lemoxit
 */

#ifndef _OPTIMIZE_OBJECT_
#define _OPTIMIZE_OBJECT_

template <class StateType>
class OptimizeObject {
 public:
  using ObjectValue = double;
  using DirectionType = StateType;
  using GradientType = StateType;
  using InitialGuessType = StateType;

  explicit OptimizeObject(const InitialGuessType& initial_guess)
      : state_(initial_guess) {}
  virtual ~OptimizeObject() = default;

  inline const StateType state() const { return state_; }

  inline const ObjectValue value() const { return value_; }

  inline const GradientType gradient() const { return gradient_; }

  void UpdateByStep(const DirectionType& dir, const double step_size) {
    state_ = state_ + step_size * dir;
    value_ = ComputeValue(state_);
    gradient_ = ComputeGradient(state_);
  }

 protected:
  virtual const GradientType ComputeGradient(const StateType&) const = 0;
  virtual const ObjectValue ComputeValue(const StateType&) const = 0;
  StateType state_;
  ObjectValue value_;
  GradientType gradient_;
};
#endif  // _OPTIMIZE_OBJECT_
