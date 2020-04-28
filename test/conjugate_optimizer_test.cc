/*
 * @Author: lemoxit
 * @Date: 2020-04-28 23:30:30
 * @LastEditTime: 2020-04-29 01:29:21
 * @LastEditors: lemoxit
 * @Description:
 */

#include "conjugate_optimizer/conjugate_optimizer.h"

#include <iostream>


class QuardOptimizeObject : public OptimizeObject<double> {
 public:
  QuardOptimizeObject(const double& ini_guess)
      : OptimizeObject<double>{ini_guess} {
    value_ = ComputeValue(state_);
    gradient_ = ComputeGradient(state_);
  }
  virtual ~QuardOptimizeObject() = default;
  const double ComputeGradient(const double& x) const override {
    return x - 1.0;
  }
  const double ComputeValue(const double& x) const override {
    return (x - 1.0) * (x - 1.0) * 0.5;
  }
};

int main() {
  std::cout << "Test Begin" << std::endl;
  QuardOptimizeObject obj{3.0};
  return 0;
}