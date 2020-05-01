
/*
 * File: optimize_state.h
 * Created Date: 2020-04-30
 * Author: Lemoxit
 * -----
 * Last Modified: Thursday April 30th 2020 12:47:41 pm
 * Modified By: Lemoxit at <yangshuai93@hotmail.com>
 * -----
 * Copyright (c) 2020
 * License: BSD
 */

#ifndef OPTIMIZE_STATE_H
#define OPTIMIZE_STATE_H
#include <array>
#include <sstream>

template <size_t N>
struct OptimizeState : public std::array<double, N> {
  using DataType = std::array<double, N>;
  OptimizeState<N>() = default;
  explicit OptimizeState<N>(const DataType& data) : DataType(data) {}
  virtual ~OptimizeState<N>() = default;

  const OptimizeState<N> operator+(const OptimizeState<N>& other) const {
    DataType ret;
    for (size_t i = 0; i < N; ++i) {
      ret[i] = (*this)[i] + other[i];
    }
    return OptimizeState<N>{ret};
  }

  const OptimizeState<N> operator-(const OptimizeState<N>& other) const {
    DataType ret;
    for (size_t i = 0; i < N; ++i) {
      ret[i] = (*this)[i] - other[i];
    }
    return OptimizeState<N>{ret};
  }

  const OptimizeState<N> operator*(double s) const {
    DataType ret;
    for (size_t i = 0; i < N; ++i) {
      ret[i] = (*this)[i] * s;
    }
    return OptimizeState<N>{ret};
  }

  const double InnerProduct(const OptimizeState<N>& other) const {
    double ret = 0.0;
    for (size_t i = 0; i < N; ++i) {
      ret += (*this)[i] * other[i];
    }
    return ret;
  }

  const std::string DebugString() const {
    std::ostringstream str;
    str << "[ ";
    for (size_t i = 0; i < N; ++i) {
      str << (*this)[i] << " ";
    }
    str << "]";
    return str.str();
  }
};

#endif /* OPTIMIZE_STATE_H */