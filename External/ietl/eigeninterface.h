// Copyright 2018 Alexander Wietek - All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef EXTERNAL_IETL_EIGENINTERFACE_H_
#define EXTERNAL_IETL_EIGENINTERFACE_H_

#include <ietl/complex.h>
#include <Eigen/SparseCore>

namespace ietl {

  template <class Gen>
  void generate(Eigen::Matrix<std::complex<double>, Eigen::Dynamic, 1>& c,
                Gen& gen) {
    for (size_t i = 0; i < c.rows(); ++i)
      c(i, 0) = std::complex<double>(gen(), gen());
  }


  template <class Gen>
  void generate(Eigen::Matrix<double, Eigen::Dynamic, 1>& c, Gen& gen) {
    for (size_t i = 0; i < c.rows(); ++i)
      c(i, 0) = static_cast<double>(gen());
  }


  template <class TCoeffs>
  void mult(const Eigen::SparseMatrix<TCoeffs>& a,
            const Eigen::Matrix<TCoeffs, Eigen::Dynamic, 1>& x,
            Eigen::Matrix<TCoeffs, Eigen::Dynamic, 1>& y)
  { y = a*x; }


  template <class TCoeffs>
  TCoeffs dot(const Eigen::Matrix<TCoeffs, Eigen::Dynamic, 1>& x,
              const Eigen::Matrix<TCoeffs, Eigen::Dynamic, 1>& y)
  { return y.dot(x); }


  template <class TCoeffs>
  typename real_type<TCoeffs>::type two_norm
  (const Eigen::Matrix<TCoeffs, Eigen::Dynamic, 1>& c)
  { return std::sqrt(ietl::real<typename real_type<TCoeffs>::type>(c.dot(c))); }
}

#endif  // EXTERNAL_IETL_EIGENINTERFACE_H_
