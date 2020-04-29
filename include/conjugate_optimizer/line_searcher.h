/*
 * @Author: your name
 * @Date: 2020-04-28 23:30:30
 * @LastEditTime: 2020-04-29 02:23:57
 * @LastEditors: lemoxit
 * @Description: In User Settings Edit
 * @FilePath: /conjugate_optimizer/include/conjugate_optimizer/line_searcher.h
 */

#ifndef _LINE_SEARCHER_
#define _LINE_SEARCHER_
#include "optimize_object.h"

enum LineSearchType { kWolfe, kArmijo, kStrongWolfe, kBackTracing };
template <class StateType>
class LineSearcher {
 public:
  using DirectionType = StateType;
  virtual ~LineSearcher() = default;
  virtual void SearchAndUpdateObject(const DirectionType& dir,
                                     OptimizeObject<StateType>* obj) = 0;
};

template <class StateType>
class WolfeLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  void SearchAndUpdateObject(const DirectionType& dir,
                             OptimizeObject<StateType>* obj) override {
    static constexpr double kInitialStepSize = 1.0;
    static constexpr double kDecayRatio = 0.5;
    static constexpr double kC1 = 0.1;  //(0,c2)
    static constexpr double kC2 = 0.9;  //(c1,1)
    static constexpr double kMinStep = 1.0e-4;
    double step = kInitialStepSize;
    // const double m = InnerProduct(dir,obj->gradient()); //TODO(stan.yang)
    const double dir_innerproduct_gradient = dir * obj->gradient();
    const double m1 = dir_innerproduct_gradient * kC1;
    const double m2 = dir_innerproduct_gradient * kC2;
    auto new_state = obj->state();
    auto new_value = obj->value();
    auto new_gradient = obj->gradient();
    while (step > kMinStep) {
      new_state = obj->state() + step * dir;
      new_value = obj->ComputeValue(new_state);
      new_gradient = obj->ComputeGradient(new_state);
      if (new_value <= obj->value() + step * m1 && new_gradient * dir >= m2) {
        break;
      }
      step *= kDecayRatio;
    }
    obj->set_state(new_state);
    obj->set_value(new_value);
    obj->set_gradient(new_gradient);
  }
};

template <class StateType>
class StrongWolfeLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  void SearchAndUpdateObject(const DirectionType& dir,
                             OptimizeObject<StateType>* obj) override {
    static constexpr double kInitialStepSize = 1.0;
    static constexpr double kDecayRatio = 0.5;
    static constexpr double kC1 = 0.1;  //(0,c2)
    static constexpr double kC2 = 0.9;  //(c1,1)
    static constexpr double kMinStep = 1.0e-4;
    double step = kInitialStepSize;
    // const double m = InnerProduct(dir,obj->gradient()); //TODO(stan.yang)
    const double dir_innerproduct_gradient = dir * obj->gradient();
    const double m1 = dir_innerproduct_gradient * kC1;
    const double m2 = std::fabs(dir_innerproduct_gradient * kC2);
    auto new_state = obj->state();
    auto new_value = obj->value();
    auto new_gradient = obj->gradient();
    while (step > kMinStep) {
      new_state = obj->state() + step * dir;
      new_value = obj->ComputeValue(new_state);
      new_gradient = obj->ComputeGradient(new_state);
      if (new_value <= obj->value() + step * m1 &&
          std::fabs(new_gradient * dir) <= m2) {
        break;
      }
      step *= kDecayRatio;
    }
    obj->set_state(new_state);
    obj->set_value(new_value);
    obj->set_gradient(new_gradient);
  }
};

template <class StateType>
class ArmijoLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  void SearchAndUpdateObject(const DirectionType& dir,
                             OptimizeObject<StateType>* obj) override {
    static constexpr double kInitialStepSize = 1.0;
    static constexpr double kDecayRatio = 0.5;
    static constexpr double kRho = 0.2;  //(0, 0.5)
    static constexpr double kMinStep = 1.0e-4;
    double step = kInitialStepSize;
    // const double m = InnerProduct(dir,obj->gradient()); //TODO(stan.yang)
    const double dir_innerproduct_gradient = dir * obj->gradient();
    const double m1 = dir_innerproduct_gradient * kRho;
    const double m2 = dir_innerproduct_gradient * (1.0 - kRho);
    auto new_state = obj->state();
    auto new_value = obj->value();
    while (step > kMinStep) {
      new_state = obj->state() + step * dir;
      new_value = obj->ComputeValue(new_state);
      if (new_value <= obj->value() + step * m1 &&
          new_value >= obj->value() + step * m2) {
        break;
      }
      step *= kDecayRatio;
    }
    obj->set_state(new_state);
    obj->set_value(new_value);
    const auto gradient = obj->ComputeGradient(new_state);
    obj->set_gradient(gradient);
  }
};

template <class StateType>
class BackTracingLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  void SearchAndUpdateObject(const DirectionType& dir,
                             OptimizeObject<StateType>* obj) override {
    static constexpr double kInitialStepSize = 1.0;
    static constexpr double kDecayRatio = 0.5;
    static constexpr double kC1 = 0.3;
    static constexpr double kMinStep = 1.0e-4;
    double step = kInitialStepSize;
    // const double m = InnerProduct(dir,obj->gradient()); //TODO(stan.yang)
    const double m = dir * obj->gradient() * kC1;
    auto new_state = obj->state();
    auto new_value = obj->value();
    while (step > kMinStep) {
      new_state = obj->state() + step * dir;
      new_value = obj->ComputeValue(new_state);
      if (new_value <= obj->value() + step * m) {
        break;
      }
      step *= kDecayRatio;
    }
    obj->set_state(new_state);
    obj->set_value(new_value);
    const auto gradient = obj->ComputeGradient(new_state);
    obj->set_gradient(gradient);
  }
};
#endif  // _LINE_SEARCHER_
