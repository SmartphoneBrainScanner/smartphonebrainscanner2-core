/**
* Smartphone Brain Scanner 2 License Agreement (MIT License)
*
* Copyright (c) 2012 Arkadiusz Stopczynski, Jakob Eg Larsen, Carsten Stahlhut, Michael Kai Petersen, Lars Kai Hansen.
* Technical University of Denmark, DTU Informatics, Cognitive Systems Section. http://code.google.com/p/smartphonebrainscanner2
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
* modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom
* the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/




#ifndef DTU_ARRAY_2D_H
#define DTU_ARRAY_2D_H

#include <math.h> /** for pow(float, float) */
#include "jama125/tnt_array2d.h"
#include "jama125/tnt_array2d_utils.h"
#include "jama125/jama_svd.h"



namespace DTU
{

template <class T>
class DtuArray2D : public TNT::Array2D<T>
{

    public:
        typedef         T   value_type;
        DtuArray2D() : TNT::Array2D<T>(){}
        DtuArray2D(int m, int n) : TNT::Array2D<T>(m,n) {}
        DtuArray2D(int m, int n,  T *a) : TNT::Array2D<T>(m, n, a) {}
        DtuArray2D(int m, int n, const T &a) : Array2D<T>( m,  n,   a) {}
        inline DtuArray2D(const TNT::Array2D<T> &A) : Array2D<T>(A) {}
        inline DtuArray2D & operator=(const T &a);
		
		
        /** basic maths */
        int add(const DtuArray2D<T>* B, DtuArray2D<T>* out);
        int subtract(const DtuArray2D<T>* B, DtuArray2D<T>* out);
        void multiply(double scalar, DtuArray2D<T>* out);
        int multiply(const DtuArray2D<T>* B, DtuArray2D<T>* out);
        int multiplyR(const DtuArray2D<T>& B, DtuArray2D<T>& out);
        int multiply(const DtuArray2D<T>* B, double scalar, DtuArray2D<T>* out);

        /** array operations */
        void transpose(DtuArray2D<T>* A); // write to given variable
        void transpose(DtuArray2D<T>& A); // write to given variable
        int transpose_insitu(); // transpose self, in place
        int getSVD(JAMA::SVD<T> &A); // write to given variable
        void pinv(DtuArray2D<T>* A); // write to given variable
        inline T trace();
        void toIdentityMatrix();
		
        inline int dim1() const { return this->m_; } //rows
        inline int dim2() const { return this->n_; } //columns
		
        inline TNT::Array2D<T> toTntArray2D();
        void print();

};

template <class T>
inline void DtuArray2D<T>::print()
{
    for (int row = 0; row<this->dim1(); ++row)
    {
        for (int column = 0; column< this->dim2(); ++column)
        {
            std::cout << (*this)[row][column] << " ";
        }
        std::cout << std::endl;
    }
}

template <class T>
inline void DtuArray2D<T>::toIdentityMatrix()
{
    if (this->m_ == this->n_)
    {
        for (int i=0; i<this->m_; i++)
            for (int j=0; j<this->n_; j++)
                this->v_[i][j] = (j==i)*(T)1; /* 1 when j == i (diagonal), 0 otherwise */
    }
    else
    {
        std::cerr << "Only square matrices can be identity matrices!"<<std::endl;
        return;
    }
	
}

template <class T>
inline T DtuArray2D<T>::trace()
{
    if (dim1() == dim2())
    {
        T trace = (T)0;
        for (int i=0; i < dim1(); i++)
        {
            trace += this->v_[i][i];
        }
        return trace;
    }
    else
    {
        std::cerr << "Trace can only be calculated for square matrices!"<<std::endl;
    }
}

template <class T>
void DtuArray2D<T>::transpose(DtuArray2D<T>* A)
{
    //A = DtuArray2D<T>(this->n_, this->m_);
    for (int mi = 0; mi < this->m_; mi++)
    {
        for (int ni = 0; ni < this->n_; ni++)
        {
            (*A)[ni][mi] = this->v_[mi][ni];
        }
    }
}
template <class T>
void DtuArray2D<T>::transpose(DtuArray2D<T>& A)
{
    A = DtuArray2D<T>(this->n_, this->m_);
    for (int mi = 0; mi < this->m_; mi++)
    {
        for (int ni = 0; ni < this->n_; ni++)
        {
            A[ni][mi] = this->v_[mi][ni];
        }
    }
}

/* this function only works with square matrices. It's more memory efficient
   if you do not need to store a copy of the matrix before transposition, as it overwrites
   the matrix being transformed and does not create more matrices */
template <class T>
int DtuArray2D<T>::transpose_insitu()
{
    if (dim1() == dim2())
    {
        for (int mi = 0; mi < this->m_; mi++)
        {
            for (int ni = mi+1; ni < this->n_; ni++) {
                T temp = this->v_[ni][mi];
                this->v_[ni][mi] = this->v_[mi][ni];
                this->v_[mi][ni] = temp;
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

/* There are several ways to implement the multiplication. All the methods were tested
  and the most efficient JAMA implementation has been left uncommented for further use.
  You can uncomment other methods and try them out. */
template <class T>
int DtuArray2D<T>::multiply(const DtuArray2D<T>* B, DtuArray2D<T>* out)
{

    if (dim2() == B->dim1())
    {

        int M = dim1();
        int N = dim2();
        int K = B->dim2();


//        // --------------- JAMA implementation
        T* bcolj = new T[N];
        for (int j = 0; j < K; j++)
        {
            for (int k = 0; k < N; k++)
            {
                bcolj[k] = (*B)[k][j];

            }
            for (int i = 0; i < M; i++)
            {
                T* arowi = this->v_[i];
                T sum = 0.0;
                for (int k = 0; k < N; k++)
                {
                    sum += arowi[k] * bcolj[k];
                }

                (*out)[i][j] = sum;
            }
        }

        delete[] bcolj;
        return 0;
        // --------------- end of JAMA implementation


        // --------------- naive implementation
//         for (int j=0; j<K; j++)
//              for (int k=0; k<N; k++)
//                  for (int i=0; i<M; i++)
//                      (*out)[i][j] += this->v_[i][k] * (*B) [k][j];

//         return 0;
        // --------------- end of naive implementation
		

        // --------------- Arek's implementation
//         T* Bcolj = new T[N];

//         for (int j=0; j<K; ++j)
//         {
//              for (int k=0; k < N; ++k)
//              {
//                  Bcolj[k] = (*B)[k][j];
//              }

//              for (int i=0; i< M; ++i)
//              {
//                  T s = 0;
//                  for (int k=0; k < N; ++k)
//                  {
//                      s += this->v_[i][k] *  Bcolj[k];
//                  }
//                  (*out)[i][j] = s;
//              }

//         }

//         delete[] Bcolj;
//         return 0;
        // --------------- end of Arek's implementation

        // --------------- implementation by Tomas Gudmundsson (using blocking)
//         int b_sz = 256;
//         // Matrix A is of size M X N
//         // Matrix B is of size N X K
//         // Matrix C is of size M X K
//         for (int i = 0; i < M; i += b_sz) {
//              for (int j = 0; j < N; j += b_sz) {
//                  for (int h = 0; h < K; h += b_sz) {
//                      for (int h2 = 0; h2 < min(K - h, b_sz); h2++) {
//                          for (int j2 = 0; j2 < min(N - j, b_sz); j2++) {
//                              for (int i2 = 0; i2 < min(M - i, b_sz); i2++) {
//                                  //out[(j+j2)*K+(h+h2)] += this->v_[(j+j2)*M+(i+i2)] * B[(i+i2)*K+(h+h2)];
//                                  (*out)[(j+j2)][(h+h2)] += this->v_[(j+j2)][(i+i2)] * (*B)[(i+i2)][(h+h2)];
//                              }
//                          }
//                      }
//                  }
//              }
//          }
        // --------------- end of implementation by Tomas Gudmundsson (using blocking)
		 

	}
	else
	{
                //std::cout << "Dimensions mismatch" << std::endl;
		return -1;
	}
}

template <class T>
int DtuArray2D<T>::multiplyR(const DtuArray2D<T>& B, DtuArray2D<T>& out)
{

    if (dim2() == B.dim1())
    {
        int M = dim1();
        int N = dim2();
        int K = B.dim2();

        out = DtuArray2D<T>(M,K)=0;

        // --------------- JAMA implementation
        T* bcolj = new T[N];
        for (int j = 0; j < K; j++)
        {

            for (int k = 0; k < N; k++)
            {
                bcolj[k] = B[k][j];

            }

            for (int i = 0; i < M; i++)
            {
                T* arowi = this->v_[i];
                T sum = 0.0;
                for (int k = 0; k < N; k++)
                {
                    sum += arowi[k] * bcolj[k];
                }

                out[i][j] = sum;

            }
        }
        delete[] bcolj;
        return 0;


    }
    else
    {
                //std::cout << "Dimensions mismatch" << std::endl;
        return -1;
    }
}

/**
  see int DtuArray2D<T>::multiply(const Array2D<T> &B, DtuArray2D<T> &out)
  for alternative implementations of multiplication
*/
template <class T>
int DtuArray2D<T>::multiply(const DtuArray2D<T>* B, double constant, DtuArray2D<T>* out)
{
    if (dim2() == B->dim1())
    {
        int M = dim1();
        int N = dim2();
        int K = B->dim2();

		
        T* bcolj = new T[N];
        for (int j = 0; j < K; j++)
        {
            for (int k = 0; k < N; k++)
            {
                bcolj[k] = (*B)[k][j];
            }
            for (int i = 0; i < M; i++) 
            {
                T* arowi = this->v_[i];
                T sum = 0.0;
                for (int k = 0; k < N; k++)
                {
                    sum += arowi[k] * bcolj[k];
                }
		(*out)[i][j] = sum*constant;
            }
        }
        delete[] bcolj;
        return 0;
    }
    else
    {
        return -1;
    }

}

template <class T>
void DtuArray2D<T>::multiply(double scalar, DtuArray2D<T>* out)
{
   // if ((this->m_ != out->dim1()) || (this->n_ != out->dim2())) // if the size of the output array is different, reinitialize it
    //    (*out) = DtuArray2D<T>(this->m_, this->n_);

    //std::cout << "HERE!" << this->m_ <<  " "<<this->n_  << " "<<scalar << std::endl;
    for (int i = 0; i < this->m_; i++)
    {
        for (int j = 0; j < this->n_; j++)
        {
            (*out)[i][j] = this->v_[i][j] * scalar;
            //std::cout << this->v_[i][j];
        }
    }
     //std::cout << std::endl;
}

template <class T>
int DtuArray2D<T>::add(const DtuArray2D<T>* B, DtuArray2D<T>* out)
{
    if ((this->m_ == B->dim1()) && (this->n_ == B->dim2()))
    {
        if ((this->m_ != out->dim1()) || (this->n_ != out->dim2())) // if the size of the output array is different, reinitialize it
            (*out) = DtuArray2D<T>(this->m_, this->n_);

        for (int i = 0; i < this->m_; i++)
            for (int j = 0; j < this->n_; j++)
                (*out)[i][j] = this->v_[i][j] + (*B)[i][j];


        return 0;
    }
    else
    {
        return -1;
    }
}

template <class T>
int DtuArray2D<T>::subtract(const DtuArray2D<T>* B, DtuArray2D<T>* out)
{
    if ((this->m_ == B->dim1()) && (this->n_ == B->dim2()))
    {
        if ((this->m_ != out->dim1()) || (this->n_ != out->dim2())) // if the size of the output array is different, reinitialize it
            (*out) = DtuArray2D<T>(this->m_, this->n_);

        for (int i = 0; i < this->m_; i++)
            for (int j = 0; j < this->n_; j++)
                (*out)[i][j] = this->v_[i][j] - (*B)[i][j];


        return 0;
    }
    else
    {
        return -1;
    }
	
	
}

template <class T>
int DtuArray2D<T>::getSVD(JAMA::SVD<T> &out)
{
    out = JAMA::SVD<T>(this->toTntArray2D());
}

template <class T>
void DtuArray2D<T>::pinv(DtuArray2D<T>* out)
{	

    int min_dim = min(dim1(), dim2());
    int max_dim = max(dim1(), dim2());

    bool transpose = false;

    DtuArray2D<T> S = DtuArray2D<T>(max_dim, min_dim) = 0.0;
    DtuArray2D<T> U;
    DtuArray2D<T> V;



    // the JAMA::SVD works ok only for matrices in which
    // there number of columns is greater or equal to the number of rows
    // Therefore, if the current matrix has more rows, it needs to be transposed
    // and its pseudo inverse is transposed before returning
    if (this->m_ < this->n_)
    {
        DtuArray2D temp = Array2D<T>(this->n_, this->m_);
        this->transpose(temp);
        JAMA::SVD<T>svd = JAMA::SVD<T>(temp);
        transpose = true;

        svd.getU(U);
        svd.getV(V);

        TNT::Array2D<double> Sp = TNT::Array2D<double>();
        svd.getS(Sp);
        // JAMA::SVD returns S as a square matrix while for the pseudo inverse
        // we need a matrix of the same size as the input matrix. Therefore, the
        // S we get from JAMA is copied to a matrix of correct size.
        for (int i=0; i<min_dim; i++)
        {
            S[i][i] = Sp[i][i];
        }
    }
    else
    {
        JAMA::SVD<T>svd = JAMA::SVD<T>(*(this));
        svd.getU(U);
        svd.getV(V);
        if (max_dim == min_dim)
            svd.getS(S);
        else
        {
            TNT::Array2D<double> Sp = TNT::Array2D<double>();
            svd.getS(Sp);
            for (int i=0; i<min_dim; i++)
            {
                S[i][i] = Sp[i][i];
            }
        }
    }


    // invert the sigma matrix,
    // see http://en.wikipedia.org/wiki/Moore%E2%80%93Penrose_pseudoinverse#Singular_value_decomposition_.28SVD.29

    // values below epsilon will be treated as zero.
    // if you want to tune the results to be closer to what MATLAB returns, play with this parameter
    double epsilon = pow(2.0, -24.0);
    //double epsilon = 0.2;

    T sigma_max = S[0][0];
    for (int i = 1; i < min_dim; i++)
    {
        if (S[i][i] > sigma_max)
        sigma_max = S[i][i];
    }
    //T tolerance = epsilon * max_dim * sigma_max;
    T tolerance = epsilon * sigma_max;
    for (int i = 0; i < min_dim; i++)
    {

        if (S[i][i] > tolerance)
	{
            S[i][i] = 1/S[i][i];

	}
        else
            S[i][i] = 0;
    }


    DtuArray2D V_times_S_transposed = DtuArray2D<T>();
    // if sigma matrix is square, transpose it in place

    if (S.dim1() == S.dim2())
    {
        S.transpose_insitu();
        V.multiplyR(S,V_times_S_transposed);
    }
    else // if sigma matrix is not square, it must be transposed into a separate variable
    {
        DtuArray2D<T>S_transposed = DtuArray2D<T>();
        S.transpose(S_transposed);
        V.multiplyR(S_transposed,V_times_S_transposed);
    }


    U.transpose_insitu(); // U matrix is always square


    DtuArray2D<T> pre_out = DtuArray2D<T>();
    V_times_S_transposed.multiplyR(U,pre_out);
    if (transpose)
        pre_out.transpose(out);
    else
    {
        for (int row=0; row<pre_out.dim1(); ++row)
        {
            for (int column = 0; column<pre_out.dim2(); ++column)
            {
                (*out)[row][column] = pre_out[row][column];
            }
        }

    }
}

// this transforms the given DtuArray into a TntArray with the same content.
// this should not be used usually, as the DtuArray2D IS a TntArray2D is thus
// accepted as input argument by functions using the TntArrays.
template <class T>
TNT::Array2D<T> DtuArray2D<T>::toTntArray2D()
{
        TNT::Array2D<T> out = TNT::Array2D<T>(this->m_, this->n_);
	for (int i = 0; i < this->m_; i ++)
            for (int j = 0; j < this->n_; j++)
                out[i][j] = this->v_[i][j];
	return out;
}

// assigning a constant value to all fields in the table
template <class T>
DtuArray2D<T> & DtuArray2D<T>::operator=(const T &a)
{
	/* non-optimzied, but will work with subarrays in future verions */
	for (int i=0; i<this->m_; i++)
		for (int j=0; j<this->n_; j++)
			this->v_[i][j] = a;
        return *this;
}


} // DTU
#endif // DTU_ARRAY_2D_H
