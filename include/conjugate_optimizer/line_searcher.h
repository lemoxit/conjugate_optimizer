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
#include <algorithm>

enum LineSearchType { kWolfe, kArmijo, kStrongWolfe, kBackTracing };
struct LineSearchPara {
  int max_iteration_num = 50;
  double min_step_size = 1.0e-4;
  double step_decay_ratio = 0.5;
  double initial_step_size = 1.0;
  double c1 = 0.1;   ///< for wolfe condition range:(0,c2)
  double c2 = 0.9;   ///< for wolfe confition range:(c1,1)
  double rho = 0.3;  ///< for armijo condition range:(0, 0.5)
};
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
  explicit WolfeLineSearcher(const LineSearchPara& para) : para_(para) {}
  virtual ~WolfeLineSearcher() = default;
  void SearchAndUpdateObject(const DirectionType& dir,
                             OptimizeObject<StateType>* obj) override {
    double step = para_.initial_step_size;
    const double m = dir.InnerProduct(obj->gradient());
    const double m1 = m * para_.c1;
    const double m2 = m * para_.c2;
    auto new_state = obj->state();
    auto new_value = obj->value();
    auto new_gradient = obj->gradient();
    while (step > para_.min_step_size) {
      new_state = obj->state() + dir * step;
      new_value = obj->ComputeValue(new_state);
      new_gradient = obj->ComputeGradient(new_state);
      if (new_value <= obj->value() + step * m1 &&
          new_gradient.InnerProduct(dir) >= m2) {
        break;
      }
      step *= para_.step_decay_ratio;
    }
    obj->set_state(new_state);
    obj->set_value(new_value);
    obj->set_gradient(new_gradient);
  }

 private:
  LineSearchPara para_;
};

template <class StateType>
class StrongWolfeLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  explicit StrongWolfeLineSearcher(const LineSearchPara& para) : para_(para) {}
  virtual ~StrongWolfeLineSearcher() = default;
  void SearchAndUpdateObject(const DirectionType& dir,
                             OptimizeObject<StateType>* obj) override {
    double step = para_.initial_step_size;
    const double m = dir.InnerProduct(obj->gradient());
    const double m1 = m * para_.c1;
    const double m2 = std::abs(m * para_.c2);
    auto new_state = obj->state();
    auto new_value = obj->value();
    auto new_gradient = obj->gradient();
    while (step > para_.min_step_size) {
      new_state = obj->state() + dir * step;
      new_value = obj->ComputeValue(new_state);
      new_gradient = obj->ComputeGradient(new_state);
      if (new_value <= obj->value() + step * m1 &&
          std::abs(new_gradient.InnerProduct(dir)) <= m2) {
        break;
      }
      step *= para_.step_decay_ratio;
    }
    obj->set_state(new_state);
    obj->set_value(new_value);
    obj->set_gradient(new_gradient);
  }

 private:
  LineSearchPara para_;
};

template <class StateType>
class ArmijoLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  explicit ArmijoLineSearcher(const LineSearchPara& para) : para_(para) {}
  virtual ~ArmijoLineSearcher() = default;
  void SearchAndUpdateObject(const DirectionType& dir,
                             OptimizeObject<StateType>* obj) override {
    double step = para_.initial_step_size;
    const double m = dir.InnerProduct(obj->gradient());
    const double m1 = m * para_.rho;
    const double m2 = m * (1.0 - para_.rho);
    auto new_state = obj->state();
    auto new_value = obj->value();
    while (step > para_.min_step_size) {
      new_state = obj->state() + dir * step;
      new_value = obj->ComputeValue(new_state);
      if (new_value <= obj->value() + step * m1 &&
          new_value >= obj->value() + step * m2) {
        break;
      }
      step *= para_.step_decay_ratio;
    }
    obj->set_state(new_state);
    obj->set_value(new_value);
    const auto gradient = obj->ComputeGradient(new_state);
    obj->set_gradient(gradient);
  }

 private:
  LineSearchPara para_;
};

template <class StateType>
class BackTracingLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  explicit BackTracingLineSearcher(const LineSearchPara& para) : para_(para) {}
  virtual ~BackTracingLineSearcher() = default;
  void SearchAndUpdateObject(const DirectionType& dir,
                             OptimizeObject<StateType>* obj) override {
    double step = para_.initial_step_size;
    const double m = dir.InnerProduct(obj->gradient()) * para_.c1;
    auto new_state = obj->state();
    auto new_value = obj->value();
    while (step > para_.min_step_size) {
      new_state = obj->state() + dir * step;
      new_value = obj->ComputeValue(new_state);
      if (new_value <= obj->value() + step * m) {
        break;
      }
      step *= para_.step_decay_ratio;
    }
    obj->set_state(new_state);
    obj->set_value(new_value);
    const auto gradient = obj->ComputeGradient(new_state);
    obj->set_gradient(gradient);
  }

 private:
  LineSearchPara para_;
};
#endif  // _LINE_SEARCHER_
