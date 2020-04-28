/*
 * @Author: your name
 * @Date: 2020-04-28 23:30:30
 * @LastEditTime: 2020-04-29 01:30:23
 * @LastEditors: lemoxit
 * @Description: In User Settings Edit
 * @FilePath: /conjugate_optimizer/include/conjugate_optimizer/line_searcher.h
 */

#ifndef _LINE_SEARCHER_
#define _LINE_SEARCHER_
#include "optimize_object.h"

enum LineSearchType { kWolfe, kArmijo, kStrongWolfe };
template <class StateType>
class LineSearcher {
 public:
  using DirectionType = StateType;
  virtual ~LineSearcher() = default;
  virtual double FindStepSize(const DirectionType& dir,
                              OptimizeObject<StateType>* obj) = 0;
};

template <class StateType>
class WolfeLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  double FindStepSize(const DirectionType& dir,
                      OptimizeObject<StateType>* obj) override {
    return 0.1;  // TODO(lemoxit)
  }
};

template <class StateType>
class StrongWolfeLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  double FindStepSize(const DirectionType& dir,
                      OptimizeObject<StateType>* obj) override {
    return 0.1;  // TODO(lemoxit)
  }
};

template <class StateType>
class ArmijoLineSearcher : public LineSearcher<StateType> {
 public:
  using DirectionType = StateType;
  double FindStepSize(const DirectionType& dir,
                      OptimizeObject<StateType>* obj) override {
    return 0.1;  // TODO(lemoxit)
  }
};
#endif  // _LINE_SEARCHER_
