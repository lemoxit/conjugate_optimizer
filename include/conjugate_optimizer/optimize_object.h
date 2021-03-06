/*
 * File: optimize_object.h
 * Created Date: 2020-04-29
 * Author: Lemoxit
 * -----
 * Last Modified: Wednesday April 29th 2020 20:52:59 pm
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
  using ObjectType = double;
  using DirectionType = StateType;
  using GradientType = StateType;
  using InitialGuessType = StateType;

  explicit OptimizeObject(const InitialGuessType& initial_guess)
      : state_(initial_guess) {}
  virtual ~OptimizeObject() = default;

  inline const StateType state() const { return state_; }

  inline const ObjectType value() const { return value_; }

  inline const GradientType gradient() const { return gradient_; }

  inline void set_state(const StateType& s) { state_ = s; }

  inline void set_value(const ObjectType& v) { value_ = v; }

  inline void set_gradient(const GradientType& g) { gradient_ = g; }

  virtual const GradientType ComputeGradient(const StateType&) const = 0;
  virtual const ObjectType ComputeValue(const StateType&) const = 0;

  const std::string DebugString() const {
    return "value: " + std::to_string(value_) + "\nstate: " +
           state_.DebugString() + "\ngradient : " + gradient_.DebugString();
  }

 protected:
  StateType state_;
  ObjectType value_;
  GradientType gradient_;
};
#endif  // _OPTIMIZE_OBJECT_
