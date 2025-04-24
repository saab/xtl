//////////////////////////////////////////////////////////////////////////
// boost_ublas.h
//
// Composite functions for boost::numeric::ublas::vector and matrix.
// Supports any storage organization and storage array type (second and
// third template parameters of ublas::vector and ublas::matrix).
// 
// Feel free to add more ublas conversions as needed
// /steyjo 2008-05
//////////////////////////////////////////////////////////////////////////

// Note: The input and output functions are separated since we have to 
// create the ublas objects via their constructors when reconstructing 
// them. I found no other way to do that..

#pragma once

#include "objio.h"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>



//////////////////////////////////////////////////////////////////////////
// For ublas::vector

template<class T1, class T2, class TV, class TA>
void composite(
	obj_output<T1, T2>& s, 
	boost::numeric::ublas::vector<TV, TA>& vect)
{
	s.simple(vect.size());

	// Create a pointer to the data and store using the "vector" 
	// method, since that is what seems most convenient when
	// reconstructing the matrix
	TV* pData = 0; 
	
	if(vect.size()!=0)
	{
		// Not allowed to access element 0 if size is 0!
		pData = &( vect.data()[0] );
	}

	s.vector(pData, vect.size());
}



template<class T1, class T2, class TV, class TA>
void composite(
	obj_input<T1, T2>& s, 
	boost::numeric::ublas::vector<TV, TA>& vect)
{
	// Read the size of the vector
	int size;
	s.simple(size);

	// Create a new ublas-vector of the given size and get a 
	// pointer to the data
	vect = boost::numeric::ublas::vector<TV>(size);
	TV* pData = 0;
	
	if(vect.size()!=0)
	{
		// Not allowed to access element 0 if size is 0
		pData = &( vect.data()[0] );
	}

	// 'vector' means "read to an already allocated buffer"
	s.vector(pData, size);
}



//////////////////////////////////////////////////////////////////////////
// For ublas::matrix

template<class T1, class T2, class TV, class TF, class TA>
void composite(
	obj_output<T1, T2>& s, 
	boost::numeric::ublas::matrix<TV, TF, TA>& mat)
{
	s.simple(mat.size1());
	s.simple(mat.size2());
	
	// Create a pointer to the data and store using the "vector" 
	// method, since that is what seems most convenient when
	// reconstructing the matrix
	TV* pData = 0;
	if(0!=mat.size1() && 0!=mat.size2())
	{
		// Unless it is empty, get the data pointer, otherwize just
		// store a null pointer
		
		pData = &( mat.data()[0] );
	}
	s.vector(pData, mat.size1()*mat.size2());
}



template<class T1, class T2, class TV, class TF, class TA>
void composite(
	obj_input<T1, T2>& s, 
	boost::numeric::ublas::matrix<TV, TF, TA>& mat)
{
	// Read the size of the vector
	int size1;
	int size2;
	s.simple(size1);
	s.simple(size2);

	// Create a new ublas-vector of the given size and get a 
	// pointer to the data
	mat = boost::numeric::ublas::matrix<TV>(size1, size2);
	TV* pData = 0;
	
	if(0!=size1 && 0!=size2)
	{
		// Unless it's empty, get a pointer to the data buffer
		pData = &( mat.data()[0] );
	}

	// 'vector' means "read to an already allocated buffer"
	s.vector(pData, size1*size2);
}

