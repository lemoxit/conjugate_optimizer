/*
 * @Author:lemoxit
 * @Date: 2020-04-28 23:30:30
 * @LastEditTime: 2020-04-29 02:02:05
 * @LastEditors: lemoxit
 */

#ifndef _CONJUGATE_OPTIMIZER_
#define _CONJUGATE_OPTIMIZER_
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include "line_searcher.h"
struct OptimizePara {
  double converge_tolerance = 1.0e-5;
  int max_iteration_num = 50;
  LineSearchType line_search_type = LineSearchType::kWolfe;
  bool debug = true;
};

enum ErrorCode : int {
  kSuccess,
  kTimeOut,
  kError,
};

struct Status {
  int iteration = 0;
  ErrorCode error_code = ErrorCode::kError;
  // TODO(lemoxit) impl
  const std::string DebugString() const {
    return "Iteration: " + std::to_string(iteration) +
           "\nErrorCode: " + std::to_string(static_cast<int>(error_code));
  }
};

template <class StateType>
class ConjugateOptimizer {
 public:
  typedef typename OptimizeObject<StateType>::DirectionType DirectionType;
  typedef typename OptimizeObject<StateType>::GradientType GradientType;

  explicit ConjugateOptimizer(const OptimizePara& para) : para_{para} {
    switch (para_.line_search_type) {
      case LineSearchType::kWolfe: {
        line_searcher_.reset(new WolfeLineSearcher<StateType>());
        break;
      }
      case LineSearchType::kArmijo: {
        line_searcher_.reset(new ArmijoLineSearcher<StateType>());
        break;
      }
      case LineSearchType::kStrongWolfe: {
        line_searcher_.reset(new StrongWolfeLineSearcher<StateType>());
        break;
      }
      default:
        line_searcher_.reset(new WolfeLineSearcher<StateType>());
    }
  }

  Status Optimize(OptimizeObject<StateType>* obj) {
    Status status;
    DirectionType direction = -obj->gradient();

    while (status.iteration < para_.max_iteration_num) {
      if (/*direction.InnerProduct(direction) */ direction * direction <
          para_.converge_tolerance) {  // TODO(lemoxit) converge conditions
        status.error_code = ErrorCode::kSuccess;
        break;
      }
      if (para_.debug) {
        std::cout << obj->DebugString() << std::endl;
      }
      GradientType gradient = obj->gradient();
      const double step_size = line_searcher_->FindStepSize(direction, obj);
      obj->UpdateByStep(direction, step_size);
      GradientType gradient_new = obj->gradient();
      // TODO(lemoxit) inner_product solution
      //   double beta = gradient_new.InnerProduct(gradient_new - gradient) /
      //                 gradient.InnerProduct(gradient);
      double beta =
          gradient_new * (gradient_new - gradient) / (gradient * gradient);
      beta = std::max(beta, 0.0);
      direction = -gradient_new + beta * direction;
      ++status.iteration;
    }
    std::cout << status.DebugString() << std::endl;
    return status;
  }

 private:
  OptimizePara para_;
  std::unique_ptr<LineSearcher<StateType>> line_searcher_ = nullptr;
};
#endif  // _CONJUGATE_OPTIMIZER_
