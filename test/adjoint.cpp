// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006-2008 Benoit Jacob <jacob@math.jussieu.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#include "main.h"

template<typename MatrixType> void adjoint(const MatrixType& m)
{
  /* this test covers the following files:
     Transpose.h Conjugate.h Dot.h
  */

  typedef typename MatrixType::Scalar Scalar;
  typedef Matrix<Scalar, MatrixType::RowsAtCompileTime, 1> VectorType;
  int rows = m.rows();
  int cols = m.cols();

  MatrixType m1 = MatrixType::random(rows, cols),
             m2 = MatrixType::random(rows, cols),
             m3(rows, cols),
             mzero = MatrixType::zero(rows, cols),
             identity = Matrix<Scalar, MatrixType::RowsAtCompileTime, MatrixType::RowsAtCompileTime>
                              ::identity(rows, rows),
             square = Matrix<Scalar, MatrixType::RowsAtCompileTime, MatrixType::RowsAtCompileTime>
                              ::random(rows, rows);
  VectorType v1 = VectorType::random(rows),
             v2 = VectorType::random(rows),
             v3 = VectorType::random(rows),
             vzero = VectorType::zero(rows);

  Scalar s1 = ei_random<Scalar>(),
         s2 = ei_random<Scalar>();

  // check basic compatibility of adjoint, transpose, conjugate
  VERIFY_IS_APPROX(m1.transpose().conjugate().adjoint(),    m1);
  VERIFY_IS_APPROX(m1.adjoint().conjugate().transpose(),    m1);

  // check multiplicative behavior
  std::cout << (m1.adjoint() * m2).adjoint() << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::cout << m2.adjoint() * m1 << std::endl;
  VERIFY_IS_APPROX((m1.adjoint() * m2).adjoint(),           m2.adjoint() * m1);
  VERIFY_IS_APPROX((s1 * m1).adjoint(),                     ei_conj(s1) * m1.adjoint());

  // check basic properties of dot, norm, norm2
  typedef typename NumTraits<Scalar>::Real RealScalar;
  VERIFY_IS_APPROX((s1 * v1 + s2 * v2).dot(v3),      s1 * v1.dot(v3) + s2 * v2.dot(v3));
  VERIFY_IS_APPROX(v3.dot(s1 * v1 + s2 * v2),        ei_conj(s1)*v3.dot(v1)+ei_conj(s2)*v3.dot(v2));
  VERIFY_IS_APPROX(ei_conj(v1.dot(v2)),                 v2.dot(v1));
  VERIFY_IS_APPROX(ei_abs(v1.dot(v1)),                  v1.norm2());
  if(NumTraits<Scalar>::HasFloatingPoint)
    VERIFY_IS_APPROX(v1.norm2(),                     v1.norm() * v1.norm());
  VERIFY_IS_MUCH_SMALLER_THAN(ei_abs(vzero.dot(v1)),    static_cast<RealScalar>(1));
  if(NumTraits<Scalar>::HasFloatingPoint)
    VERIFY_IS_MUCH_SMALLER_THAN(vzero.norm(),        static_cast<RealScalar>(1));

  // check compatibility of dot and adjoint
  VERIFY_IS_APPROX(v1.dot(square * v2),              (square.adjoint() * v1).dot(v2));

  // like in testBasicStuff, test operator() to check const-qualification
  int r = ei_random<int>(0, rows-1),
      c = ei_random<int>(0, cols-1);
  VERIFY_IS_APPROX(m1.conjugate()(r,c), ei_conj(m1(r,c)));
  VERIFY_IS_APPROX(m1.adjoint()(c,r), ei_conj(m1(r,c)));

}

void test_adjoint()
{
  for(int i = 0; i < g_repeat; i++) {
    CALL_SUBTEST( adjoint(Matrix<float, 1, 1>()) );
    CALL_SUBTEST( adjoint(Matrix4d()) );
    CALL_SUBTEST( adjoint(MatrixXcf(3, 3)) );
    CALL_SUBTEST( adjoint(MatrixXi(8, 12)) );
    CALL_SUBTEST( adjoint(MatrixXcd(20, 20)) );
  }
  // test a large matrix only once
  CALL_SUBTEST( adjoint(Matrix<float, 100, 100>()) );
}

