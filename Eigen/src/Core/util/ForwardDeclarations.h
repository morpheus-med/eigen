// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2007-2010 Benoit Jacob <jacob.benoit.1@gmail.com>
// Copyright (C) 2008-2009 Gael Guennebaud <gael.guennebaud@inria.fr>
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

#ifndef EIGEN_FORWARDDECLARATIONS_H
#define EIGEN_FORWARDDECLARATIONS_H

namespace internal {

template<typename T> struct traits;

// here we say once and for all that traits<const T> == traits<T>
// When constness must affect traits, it has to be constness on template parameters on which T itself depends.
// For example, traits<Map<const T> > != traits<Map<T> >, but
//              traits<const Map<T> > == traits<Map<T> >
template<typename T> struct traits<const T> : traits<T> {};

template<typename Derived> struct has_direct_access
{
  enum { ret = (traits<Derived>::Flags & DirectAccessBit) ? 1 : 0 };
};

template<typename Derived> struct accessors_level
{
  enum { has_direct_access = (traits<Derived>::Flags & DirectAccessBit) ? 1 : 0,
         has_write_access = (traits<Derived>::Flags & LvalueBit) ? 1 : 0,
         value = has_direct_access ? (has_write_access ? DirectWriteAccessors : DirectAccessors)
                                   : (has_write_access ? WriteAccessors       : ReadOnlyAccessors)
  };
};

} // end namespace internal

template<typename T> struct NumTraits;

template<typename Derived> struct EigenBase;
template<typename Derived> class DenseBase;
template<typename Derived> class PlainObjectBase;


template<typename Derived,
         int Level = internal::accessors_level<Derived>::value >
class DenseCoeffsBase;

template<typename _Scalar, int _Rows, int _Cols,
         int _Options = AutoAlign |
                          ( (_Rows==1 && _Cols!=1) ? RowMajor
                          : (_Cols==1 && _Rows!=1) ? ColMajor
                          : EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION ),
         int _MaxRows = _Rows,
         int _MaxCols = _Cols
> class Matrix;

template<typename Derived> class MatrixBase;
template<typename Derived> class ArrayBase;

template<typename ExpressionType, unsigned int Added, unsigned int Removed> class Flagged;
template<typename ExpressionType, template <typename> class StorageBase > class NoAlias;
template<typename ExpressionType> class NestByValue;
template<typename ExpressionType> class ForceAlignedAccess;
template<typename ExpressionType> class SwapWrapper;

template<typename XprType, int BlockRows=Dynamic, int BlockCols=Dynamic, bool InnerPanel = false,
         bool HasDirectAccess = internal::has_direct_access<XprType>::ret> class Block;

template<typename MatrixType, int Size=Dynamic> class VectorBlock;
template<typename MatrixType> class Transpose;
template<typename MatrixType> class Conjugate;
template<typename NullaryOp, typename MatrixType>         class CwiseNullaryOp;
template<typename UnaryOp,   typename MatrixType>         class CwiseUnaryOp;
template<typename ViewOp,    typename MatrixType>         class CwiseUnaryView;
template<typename BinaryOp,  typename Lhs, typename Rhs>  class CwiseBinaryOp;
template<typename BinOp,     typename Lhs, typename Rhs>  class SelfCwiseBinaryOp;
template<typename Derived,   typename Lhs, typename Rhs>  class ProductBase;
template<typename Lhs, typename Rhs, int Mode>            class GeneralProduct;
template<typename Lhs, typename Rhs, int NestingFlags>    class CoeffBasedProduct;

template<typename Derived> class DiagonalBase;
template<typename _DiagonalVectorType> class DiagonalWrapper;
template<typename _Scalar, int SizeAtCompileTime, int MaxSizeAtCompileTime=SizeAtCompileTime> class DiagonalMatrix;
template<typename MatrixType, typename DiagonalType, int ProductOrder> class DiagonalProduct;
template<typename MatrixType, int Index = 0> class Diagonal;
template<int SizeAtCompileTime, int MaxSizeAtCompileTime = SizeAtCompileTime, typename IndexType=int> class PermutationMatrix;
template<int SizeAtCompileTime, int MaxSizeAtCompileTime = SizeAtCompileTime, typename IndexType=int> class Transpositions;
template<typename Derived> class PermutationBase;
template<typename Derived> class TranspositionsBase;
template<typename _IndicesType> class PermutationWrapper;
template<typename _IndicesType> class TranspositionsWrapper;

template<typename Derived,
         int Level = internal::accessors_level<Derived>::has_write_access ? WriteAccessors : ReadOnlyAccessors
> class MapBase;
template<int InnerStrideAtCompileTime, int OuterStrideAtCompileTime> class Stride;
template<typename MatrixType, int MapOptions=Unaligned, typename StrideType = Stride<0,0> > class Map;

template<typename Derived> class TriangularBase;
template<typename MatrixType, unsigned int Mode> class TriangularView;
template<typename MatrixType, unsigned int Mode> class SelfAdjointView;
template<typename MatrixType> class SparseView;
template<typename ExpressionType> class WithFormat;
template<typename MatrixType> struct CommaInitializer;
template<typename Derived> class ReturnByValue;
template<typename ExpressionType> class ArrayWrapper;

namespace internal {
template<typename DecompositionType, typename Rhs> struct solve_retval_base;
template<typename DecompositionType, typename Rhs> struct solve_retval;
template<typename DecompositionType> struct kernel_retval_base;
template<typename DecompositionType> struct kernel_retval;
template<typename DecompositionType> struct image_retval_base;
template<typename DecompositionType> struct image_retval;
} // end namespace internal

namespace internal {
template<typename _Scalar, int Rows=Dynamic, int Cols=Dynamic, int Supers=Dynamic, int Subs=Dynamic, int Options=0> class BandMatrix;
}

namespace internal {
template<typename Lhs, typename Rhs> struct product_type;
}

template<typename Lhs, typename Rhs,
         int ProductType = internal::product_type<Lhs,Rhs>::value>
struct ProductReturnType;

// this is a workaround for sun CC
template<typename Lhs, typename Rhs> struct LazyProductReturnType;

namespace internal {

// Provides scalar/packet-wise product and product with accumulation
// with optional conjugation of the arguments.
template<typename LhsScalar, typename RhsScalar, bool ConjLhs=false, bool ConjRhs=false> struct conj_helper;

template<typename Scalar> struct scalar_sum_op;
template<typename Scalar> struct scalar_difference_op;
template<typename LhsScalar,typename RhsScalar> struct scalar_conj_product_op;
template<typename Scalar> struct scalar_quotient_op;
template<typename Scalar> struct scalar_opposite_op;
template<typename Scalar> struct scalar_conjugate_op;
template<typename Scalar> struct scalar_real_op;
template<typename Scalar> struct scalar_imag_op;
template<typename Scalar> struct scalar_abs_op;
template<typename Scalar> struct scalar_abs2_op;
template<typename Scalar> struct scalar_sqrt_op;
template<typename Scalar> struct scalar_exp_op;
template<typename Scalar> struct scalar_log_op;
template<typename Scalar> struct scalar_cos_op;
template<typename Scalar> struct scalar_sin_op;
template<typename Scalar> struct scalar_pow_op;
template<typename Scalar> struct scalar_inverse_op;
template<typename Scalar> struct scalar_square_op;
template<typename Scalar> struct scalar_cube_op;
template<typename Scalar, typename NewType> struct scalar_cast_op;
template<typename Scalar> struct scalar_multiple_op;
template<typename Scalar> struct scalar_quotient1_op;
template<typename Scalar> struct scalar_min_op;
template<typename Scalar> struct scalar_max_op;
template<typename Scalar> struct scalar_random_op;
template<typename Scalar> struct scalar_add_op;
template<typename Scalar> struct scalar_constant_op;
template<typename Scalar> struct scalar_identity_op;

template<typename LhsScalar,typename RhsScalar=LhsScalar> struct scalar_product_op;
template<typename LhsScalar,typename RhsScalar> struct scalar_multiple2_op;

} // end namespace internal

struct IOFormat;

// Array module
template<typename _Scalar, int _Rows, int _Cols,
         int _Options = AutoAlign |
                          ( (_Rows==1 && _Cols!=1) ? RowMajor
                          : (_Cols==1 && _Rows!=1) ? ColMajor
                          : EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION ),
         int _MaxRows = _Rows, int _MaxCols = _Cols> class Array;
template<typename ConditionMatrixType, typename ThenMatrixType, typename ElseMatrixType> class Select;
template<typename MatrixType, typename BinaryOp, int Direction> class PartialReduxExpr;
template<typename ExpressionType, int Direction> class VectorwiseOp;
template<typename MatrixType,int RowFactor,int ColFactor> class Replicate;
template<typename MatrixType, int Direction = BothDirections> class Reverse;

template<typename MatrixType> class FullPivLU;
template<typename MatrixType> class PartialPivLU;
namespace internal {
template<typename MatrixType> struct inverse_impl;
}
template<typename MatrixType> class HouseholderQR;
template<typename MatrixType> class ColPivHouseholderQR;
template<typename MatrixType> class FullPivHouseholderQR;
template<typename MatrixType, int QRPreconditioner = ColPivHouseholderQRPreconditioner> class JacobiSVD;
template<typename MatrixType, int UpLo = Lower> class LLT;
template<typename MatrixType, int UpLo = Lower> class LDLT;
template<typename VectorsType, typename CoeffsType, int Side=OnTheLeft> class HouseholderSequence;
template<typename Scalar>     class JacobiRotation;

// Geometry module:
template<typename Derived, int _Dim> class RotationBase;
template<typename Lhs, typename Rhs> class Cross;
template<typename Derived> class QuaternionBase;
template<typename Scalar, int Options = AutoAlign> class Quaternion;
template<typename Scalar> class Rotation2D;
template<typename Scalar> class AngleAxis;
template<typename Scalar,int Dim> class Translation;

#ifdef EIGEN2_SUPPORT
template<typename Derived, int _Dim> class eigen2_RotationBase;
template<typename Lhs, typename Rhs> class eigen2_Cross;
template<typename Scalar> class eigen2_Quaternion;
template<typename Scalar> class eigen2_Rotation2D;
template<typename Scalar> class eigen2_AngleAxis;
template<typename Scalar,int Dim> class eigen2_Transform;
template <typename _Scalar, int _AmbientDim> class eigen2_ParametrizedLine;
template <typename _Scalar, int _AmbientDim> class eigen2_Hyperplane;
template<typename Scalar,int Dim> class eigen2_Translation;
template<typename Scalar,int Dim> class eigen2_Scaling;
#endif

#if EIGEN2_SUPPORT_STAGE < STAGE20_RESOLVE_API_CONFLICTS
template<typename Scalar,int Dim> class Transform;
template <typename _Scalar, int _AmbientDim> class ParametrizedLine;
template <typename _Scalar, int _AmbientDim> class Hyperplane;
template<typename Scalar,int Dim> class Scaling;
#endif

#if EIGEN2_SUPPORT_STAGE > STAGE20_RESOLVE_API_CONFLICTS
template<typename Scalar,int Dim,int Mode,int _Options=AutoAlign> class Transform;
template <typename _Scalar, int _AmbientDim, int Options=AutoAlign> class ParametrizedLine;
template <typename _Scalar, int _AmbientDim, int Options=AutoAlign> class Hyperplane;
template<typename Scalar> class UniformScaling;
template<typename MatrixType,int Direction> class Homogeneous;
#endif

// MatrixFunctions module
template<typename Derived> struct MatrixExponentialReturnValue;
template<typename Derived> class MatrixFunctionReturnValue;

namespace internal {
template <typename Scalar>
struct stem_function
{
  typedef std::complex<typename NumTraits<Scalar>::Real> ComplexScalar;
  typedef ComplexScalar type(ComplexScalar, int);
};
}


#ifdef EIGEN2_SUPPORT
template<typename ExpressionType> class Cwise;
template<typename MatrixType> class Minor;
template<typename MatrixType> class LU;
template<typename MatrixType> class QR;
template<typename MatrixType> class SVD;
namespace internal {
template<typename MatrixType, unsigned int Mode> struct eigen2_part_return_type;
}
#endif

#endif // EIGEN_FORWARDDECLARATIONS_H
