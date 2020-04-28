/*
 * @Author:lemoxit
 * @Date: 2020-04-28 23:30:30
 * @LastEditTime: 2020-04-29 01:29:56
 * @LastEditors: lemoxit
 */

#ifndef _CONJUGATE_OPTIMIZER_
#define _CONJUGATE_OPTIMIZER_
#include <memory>
#include <string>

#include "line_searcher.h"

template <class StateType>
class ConjugateOptimizer {
 public:
  typedef typename OptimizeObject<StateType>::DirectionType DirectionType;
  typedef typename OptimizeObject<StateType>::GradientType GradientType;
  struct OptimizePara {
    double converge_tolerance = 1.0e-5;
    int max_iteration_num = 50;
    LineSearchType line_search_type = LineSearchType::kWolfe;
  };

  enum ErrorCode {
    kSuccess,
    kTimeOut,
    kError,
  };

  struct Status {
    int iteration = 0;
    ErrorCode error_code;
    const std::string DebugString() const { return "todo\n"; }
  };

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
      if (direction.InnerProduct(direction) <
          para_.converge_tolerance) {  // TODO(lemoxit) add convergence
                                       // conditions
        status.error_code = ErrorCode::kSuccess;
        break;
      }

      GradientType gradient = obj->gradient();
      const double step_size = line_searcher_->FindStepSize(direction, obj);
      GradientType gradient_new = obj->gradient();

      double beta = gradient_new.InnerProduct(gradient_new - gradient) /
                    gradient.InnerProduct(gradient);
      beta = std::max(beta, 0.0);
      direction = -gradient_new + beta * direction;
      ++status.iteration;
    }
    return status;
  }

 private:
  OptimizePara para_;
  std::unique_ptr<LineSearcher<StateType>> line_searcher_ = nullptr;
};
#endif  // _CONJUGATE_OPTIMIZER_
