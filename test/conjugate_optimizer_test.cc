/*
 * @Author: lemoxit
 * @Date: 2020-04-28 23:30:30
 * @LastEditTime: 2020-04-29 01:47:34
 * @LastEditors: lemoxit
 * @Description:
 */

#include "conjugate_optimizer/conjugate_optimizer.h"
#include "conjugate_optimizer/optimize_state.h"

#include <iostream>

using State1d = OptimizeState<1>;
class QuardOptimizeObject : public OptimizeObject<State1d> {
 public:
  explicit QuardOptimizeObject(const State1d& ini_guess)
      : OptimizeObject<State1d>(ini_guess) {
    value_ = ComputeValue(state_);
    gradient_ = ComputeGradient(state_);
  }
  virtual ~QuardOptimizeObject() = default;
  const State1d ComputeGradient(const State1d& x) const override {
    return State1d({x[0] - 1.0});
  }
  const double ComputeValue(const State1d& x) const override {
    return (x[0] - 1.0) * (x[0] - 1.0) * 0.5;
  }
};

using State3d = OptimizeState<3>;
class QuardOptimizeObject3d : public OptimizeObject<State3d> {
 public:
  explicit QuardOptimizeObject3d(const State3d& ini_guess)
      : OptimizeObject<State3d>(ini_guess) {
    value_ = ComputeValue(state_);
    gradient_ = ComputeGradient(state_);
  }
  virtual ~QuardOptimizeObject3d() = default;
  const State3d ComputeGradient(const State3d& x) const override {
    return State3d({x[0] - 1.0, x[1] - 2.0, x[2] - 3.0});
  }
  const double ComputeValue(const State3d& x) const override {
    double ret = (x[0] - 1.0) * (x[0] - 1.0) + (x[1] - 2.0) * (x[1] - 2.0) +
                 (x[2] - 3.0) * (x[2] - 3.0);
    return ret * 0.5;
  }
};

int main() {
  std::cout << "Test Begin" << std::endl;
  OptimizePara para;
  para.line_search_type = LineSearchType::kBackTracing;

  State1d ini_guess({3.3});
  QuardOptimizeObject obj(ini_guess);
  ConjugateOptimizer<State1d> optimizer(para);
  optimizer.Optimize(&obj);
  std::cout << "Result: " << obj.state().DebugString() << std::endl;

  State3d ini_guess3({3.3, 3.0, -3.0});
  QuardOptimizeObject3d obj3(ini_guess3);
  ConjugateOptimizer<State3d> optimizer3(para);
  optimizer3.Optimize(&obj3);
  std::cout << "Result: " << obj3.state().DebugString() << std::endl;

  // OptimizeState<3> st({1, 2, 3});
  // std::cout << "st: " << st.DebugString() << std::endl;
  // std::cout << "+: " << (st + st).DebugString() << std::endl;
  // std::cout << "-: " << (st - st).DebugString() << std::endl;
  // std::cout << "*: " << (st * 2.0).DebugString() << std::endl;
  // std::cout << "product: " << st.InnerProduct(st) << std::endl;

  return 0;
}