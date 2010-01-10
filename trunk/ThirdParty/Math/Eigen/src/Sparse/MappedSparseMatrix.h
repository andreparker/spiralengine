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

#ifndef EIGEN_MAPPED_SPARSEMATRIX_H
#define EIGEN_MAPPED_SPARSEMATRIX_H

/** \class MappedSparseMatrix
  *
  * \brief Sparse matrix
  *
  * \param _Scalar the scalar type, i.e. the type of the coefficients
  *
  * See http://www.netlib.org/linalg/html_templates/node91.html for details on the storage scheme.
  *
  */
template<typename _Scalar, int _Flags>
struct ei_traits<MappedSparseMatrix<_Scalar, _Flags> > : ei_traits<SparseMatrix<_Scalar, _Flags> >
    {};

template<typename _Scalar, int _Flags>
class MappedSparseMatrix
            : public SparseMatrixBase<MappedSparseMatrix<_Scalar, _Flags> >
{
public:
    EIGEN_SPARSE_GENERIC_PUBLIC_INTERFACE( MappedSparseMatrix )

protected:
    enum { IsRowMajor = Base::IsRowMajor };

    int m_outerSize;
    int m_innerSize;
    int m_nnz;
    int* m_outerIndex;
    int* m_innerIndices;
    Scalar* m_values;

public:

    inline int rows() const
    {
        return IsRowMajor ? m_outerSize : m_innerSize;
    }
    inline int cols() const
    {
        return IsRowMajor ? m_innerSize : m_outerSize;
    }
    inline int innerSize() const
    {
        return m_innerSize;
    }
    inline int outerSize() const
    {
        return m_outerSize;
    }
    inline int innerNonZeros( int j ) const
    {
        return m_outerIndex[j+1] - m_outerIndex[j];
    }

    //----------------------------------------
    // direct access interface
    inline const Scalar* _valuePtr() const
    {
        return m_values;
    }
    inline Scalar* _valuePtr()
    {
        return m_values;
    }

    inline const int* _innerIndexPtr() const
    {
        return m_innerIndices;
    }
    inline int* _innerIndexPtr()
    {
        return m_innerIndices;
    }

    inline const int* _outerIndexPtr() const
    {
        return m_outerIndex;
    }
    inline int* _outerIndexPtr()
    {
        return m_outerIndex;
    }
    //----------------------------------------

    inline Scalar coeff( int row, int col ) const
    {
        const int outer = RowMajor ? row : col;
        const int inner = RowMajor ? col : row;

        int start = m_outerIndex[outer];
        int end = m_outerIndex[outer+1];
        if ( start == end )
            return Scalar( 0 );
        else
            if ( end > 0 && inner == m_innerIndices[end-1] )
                return m_values[end-1];
        // ^^  optimization: let's first check if it is the last coefficient
        // (very common in high level algorithms)

        const int* r = std::lower_bound( &m_innerIndices[start], &m_innerIndices[end-1], inner );
        const int id = r - &m_innerIndices[0];
        return ( ( *r == inner ) && ( id < end ) ) ? m_values[id] : Scalar( 0 );
    }

    inline Scalar& coeffRef( int row, int col )
    {
        const int outer = RowMajor ? row : col;
        const int inner = RowMajor ? col : row;

        int start = m_outerIndex[outer];
        int end = m_outerIndex[outer+1];
        ei_assert( end >= start && "you probably called coeffRef on a non finalized matrix" );
        ei_assert( end > start && "coeffRef cannot be called on a zero coefficient" );
        int* r = std::lower_bound( &m_innerIndices[start], &m_innerIndices[end], inner );
        const int id = r - &m_innerIndices[0];
        ei_assert( ( *r == inner ) && ( id < end ) && "coeffRef cannot be called on a zero coefficient" );
        return m_values[id];
    }

    class InnerIterator;

    /** \returns the number of non zero coefficients */
    inline int nonZeros() const
    {
        return m_nnz;
    }

    inline MappedSparseMatrix( int rows, int cols, int nnz, int* outerIndexPtr, int* innerIndexPtr, Scalar* valuePtr )
            : m_outerSize( IsRowMajor ? rows : cols ), m_innerSize( IsRowMajor ? cols : rows ), m_nnz( nnz ), m_outerIndex( outerIndexPtr ),
            m_innerIndices( innerIndexPtr ), m_values( valuePtr )
    {}

#ifdef EIGEN_TAUCS_SUPPORT
    explicit MappedSparseMatrix( taucs_ccs_matrix& taucsMatrix );
#endif

#ifdef EIGEN_CHOLMOD_SUPPORT
    explicit MappedSparseMatrix( cholmod_sparse& cholmodMatrix );
#endif

#ifdef EIGEN_SUPERLU_SUPPORT
    explicit MappedSparseMatrix( SluMatrix& sluMatrix );
#endif

    /** Empty destructor */
    inline ~MappedSparseMatrix() {}
};

template<typename Scalar, int _Flags>
class MappedSparseMatrix<Scalar, _Flags>::InnerIterator
{
public:
    InnerIterator( const MappedSparseMatrix& mat, int outer )
            : m_matrix( mat ),
            m_outer( outer ),
            m_id( mat._outerIndexPtr()[outer] ),
            m_start( m_id ),
            m_end( mat._outerIndexPtr()[outer+1] )
    {}

    template<unsigned int Added, unsigned int Removed>
    InnerIterator( const Flagged<MappedSparseMatrix, Added, Removed>& mat, int outer )
            : m_matrix( mat._expression() ), m_id( m_matrix._outerIndexPtr()[outer] ),
            m_start( m_id ), m_end( m_matrix._outerIndexPtr()[outer+1] )
    {}

    inline InnerIterator& operator++()
    {
        m_id++;
        return *this;
    }

    inline Scalar value() const
    {
        return m_matrix._valuePtr()[m_id];
    }
    inline Scalar& valueRef()
    {
        return const_cast<Scalar&>( m_matrix._valuePtr()[m_id] );
    }

    inline int index() const
    {
        return m_matrix._innerIndexPtr()[m_id];
    }
    inline int row() const
    {
        return IsRowMajor ? m_outer : index();
    }
    inline int col() const
    {
        return IsRowMajor ? index() : m_outer;
    }

    inline operator bool() const
    {
        return ( m_id < m_end ) && ( m_id >= m_start );
    }

protected:
    const MappedSparseMatrix& m_matrix;
    const int m_outer;
    int m_id;
    const int m_start;
    const int m_end;
};

#endif // EIGEN_MAPPED_SPARSEMATRIX_H
