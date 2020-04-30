/*
 * File: optimize_object.h
 * Created Date: 2020-04-29
 * Author: Lemoxit
 * -----
 * Last Modified: Wednesday April 29th 2020 12:52:59 pm
 * Modified By: Lemoxit at <yangshuai93@hotmail.com>
 * -----
 * Copyright (c) 2020
 * License: BSD
 */

#ifndef _OPTIMIZE_OBJECT_
#define _OPTIMIZE_OBJECT_
#include <string>
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

  inline void set_state(const StateType& s) { state_ = s; }

  inline void set_value(const ObjectValue& v) { value_ = v; }

  inline void set_gradient(const GradientType& g) { gradient_ = g; }

  virtual const GradientType ComputeGradient(const StateType&) const = 0;
  virtual const ObjectValue ComputeValue(const StateType&) const = 0;
  virtual const double InnerProduct(const DirectionType&,
                                    const DirectionType&) const = 0;

  const std::string DebugString() const {
    return "state: " + std::to_string(state_) + "\nvalue: " +
           std::to_string(value_) + "\ngradient : " + std::to_string(gradient_);
  }

 protected:
  StateType state_;
  ObjectValue value_;
  GradientType gradient_;
};
#endif  // _OPTIMIZE_OBJECT_
