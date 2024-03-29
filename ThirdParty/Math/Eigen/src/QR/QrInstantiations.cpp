// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2008 Gael Guennebaud <g.gael@free.fr>
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

#ifndef EIGEN_EXTERN_INSTANTIATIONS
#define EIGEN_EXTERN_INSTANTIATIONS
#endif
#include "../../Core"
#undef EIGEN_EXTERN_INSTANTIATIONS

#include "../../QR"

namespace Eigen
{

template static void ei_tridiagonal_qr_step( float* , float* , int, int, float* , int );
template static void ei_tridiagonal_qr_step( double* , double* , int, int, double* , int );
template static void ei_tridiagonal_qr_step( float* , float* , int, int, std::complex<float>* , int );
template static void ei_tridiagonal_qr_step( double* , double* , int, int, std::complex<double>* , int );

EIGEN_QR_MODULE_INSTANTIATE();

}
