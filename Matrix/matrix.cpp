#include <stdio.h>
#include <iostream>
#include <math.h>
#include "matrix.h"

using namespace std;


//------------------------------------------------------------------
//           Constructor for the matrix class             
//-----------------------------------------------------------------
Matrix::Matrix(int a=0, int b=0, int val)
{
    rows = a;
    cols = b;
    mat = new double*[rows];
    for(int i = 0; i < rows; ++i)
    {
        mat[i] = new double[cols];
        for(int j = 0; j < cols; j++)
        {
            mat[i][j] = val;
        }
    }
}

//------------------------------------------------------------------
//           Function to read a matrix             
//-----------------------------------------------------------------
void Matrix::read_matrix()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j<cols; j++)
        {
            cin>>mat[i][j];
        }
    }
}

//------------------------------------------------------------------
//           Function to display a matrix             
//-----------------------------------------------------------------
void Matrix::display_matrix()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j<cols; j++)
        {
            cout<<mat[i][j]<<"\t";
        }
        cout<<endl;
    }
    cout<<endl;
}

//------------------------------------------------------------------
//           Function to perform deep copy of matrix             
//-----------------------------------------------------------------
Matrix Matrix::copy()
{
    int i,j;
    Matrix clone(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j) clone.mat[i][j] = this->mat[i][j];
    }
    return clone;
}

//------------------------------------------------------------------
//           Function to find transpose of a matrix             
//-----------------------------------------------------------------
Matrix Matrix::transpose()
{
    Matrix result(cols, rows);
    int i, j;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            result.mat[j][i]  = this->mat[i][j] ;
        }
    }
    return result;
}
//------------------------------------------------------------------
//           Function to concatenate a matrix horizontally            
//-----------------------------------------------------------------
Matrix Matrix::horzcat(Matrix b)
{
    Matrix result(rows, this->cols + b.cols);
    int i, j;
    for(i = 0; i < this->rows; i++)
    {
        for(j = 0; j < this->cols; j++)
        {
            result.mat[i][j] = this->mat[i][j];            
        }
    }
    for(i = 0; i < this->rows; i++)
    {
        for(j = 0; j < b.cols; j++)
        {
            result.mat[i][cols+j]  = b.mat[i][j] ;
        }
    }
    return result;
}

//------------------------------------------------------------------
//           Function to find the first non-zero element              
//-----------------------------------------------------------------
void Matrix::update_leading_0s(int *leading_0s, Matrix a)
{
    int i, j; 
    for(i = 0; i < a.rows; ++i)
    {
        leading_0s[i] = 0;
        for(j=0; (fabs(a.mat[i][j]) < 0.00001) && (j < a.cols) ;++j) leading_0s[i]++;
    }
}


//-------------------------------------------------------------------
// Function to rearrange arrange A such that pivot positions have 1               
//-------------------------------------------------------------------
void Matrix::pivot_rearrange(int *leading_0s, Matrix a)
{
    int l, remrow, i, k, lastrow, large;
    double *rowtemn = new double[a.cols];
    lastrow = rows-1;

    for(l = 0; l < a.rows; ++l)
    {
        large = leading_0s[0];
        for(i = 0; i < a.rows; ++i)
        {
            if( large <= leading_0s[i])
            {
                large=leading_0s[i];
                remrow=i;
            }
        }

    leading_0s[remrow] = leading_0s[lastrow];
    leading_0s[lastrow] = -1;

    for(k = 0; k < a.cols; ++k)   rowtemn[k] = a.mat[lastrow][k];
    for(k = 0; k < a.cols; ++k)   a.mat[lastrow][k] = a.mat[remrow][k];
    for(k = 0; k < a.cols; ++k)   a.mat[remrow][k] = rowtemn[k];
 
    lastrow--;
    }
}

//---------------------------------------------------------------------------
//           Function definition to scale a A                              
//---------------------------------------------------------------------------
void Matrix::scale_A(int *leading_0s, Matrix a)
{
    int i,j;
    double divisor;
    for(i = 0; i < a.rows; ++i)
    {  
        if(leading_0s[i] == a.cols) continue;
        divisor = a.mat[i][leading_0s[i]];
        for(j = leading_0s[i]; j < a.cols; ++j)   a.mat[i][j] = a.mat[i][j]/ divisor;
    }
}


//---------------------------------------------------------------------------
//           Function definition to convert to row reduced form                               
//---------------------------------------------------------------------------
int* Matrix::row_reduced(Matrix a)
{
    int i, next_row = 1, grp, p, r, j, *leading_0s, t, m, rank;
    leading_0s = new int[a.rows];

    update_leading_0s(leading_0s, a);
    pivot_rearrange(leading_0s, a);

    if(fabs(a.mat[0][0]) < 0.00001)
    {
        cout << "Not a valid matrix as pivot element is 0" << endl;
        return NULL; 
    }

    update_leading_0s(leading_0s, a);
    scale_A(leading_0s, a);

    while(next_row == 1)
    {
        grp = 0;
        for( i = 0; i < rows; ++i)
        {
            p = 0;
            while(leading_0s[i+p] == leading_0s[i+p+1] && (i+p+1) < a.rows)
            {
                grp++;
                p++;
            }

            if(grp != 0)
            {
                while(grp != 0)
                {
                    for(j = 0; j < a.cols; ++j) a.mat[i+grp][j] = a.mat[i+grp][j] - a.mat[i][j];
                    grp--;
                }
                break;
            }
        }   

        update_leading_0s(leading_0s, a);
        pivot_rearrange(leading_0s, a);
        update_leading_0s(leading_0s, a);
        scale_A(leading_0s, a);

        next_row = 0;

        for(r = 0; r < a.rows ; ++r)
        {
            if(leading_0s[r] == leading_0s[r+1] && r+1 < a.rows)
            {
                if(leading_0s[r] != a.cols) next_row = 1;
            }
        }

    }
    return leading_0s;
}

//------------------------------------------------------------------
//           Function to find trank of a matrix             
//-----------------------------------------------------------------
int Matrix::rank()
{
    Matrix a = this->copy();
    int *leading_0s = row_reduced(a);
    int rank = 0;
    for (int i = 0; i < a.rows; ++i)
    {
        if (leading_0s[i] != a.cols)  ++rank;
    }
    return rank;
}

//------------------------------------------------------------------
//           Function to find the determinant              
//-----------------------------------------------------------------
double Matrix::determinant()
{

    if(rows != cols) { cout<<" Not a square matrix !!"; return 0;}

    int j,p,q;
    double det =0;

    if(rows == 2){ return (mat[0][0]*mat[1][1]) - (mat[0][1]*mat[1][0]); }
    Matrix b(rows-1, rows-1);
    for(j = 0; j < cols; j++)
    {
        int r = 0, s = 0;
        for(p = 0; p < rows; p++)
        {
            for(q = 0; q < cols; q++)
            {
                if(p !=0 && q != j)
                {
                    b.mat[r][s] = mat[p][q];
                    s++;
                    if(s > cols-2)
                        {
                            r++;
                            s = 0;
                        }
                }
            }
        }
        det += (j%2 ? -1:1) * mat[0][j] * b.determinant();
    }
    return det;
}

//------------------------------------------------------------------
//           Function to find the inverse              
//-----------------------------------------------------------------
Matrix Matrix::inverse()
{
    double det = this->determinant();

    if( fabs(det) < 0.01)
    {
        cout << "Matrix is not invertible!" << endl;
    }

    int i,j, q, p, sign, r, s;
    double cofdet;
    Matrix inv( rows, cols);
    Matrix cof( rows - 1, cols - 1);

    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            sign = ((i+j)%2 ? -1 : 1);
            r = 0, s = 0;
            for(p = 0; p < rows; p++)
            {
                for(q = 0; q < cols; q++)
                {
                    if(p != i && q != j)
                    {
                        cof.mat[r][s] = mat[p][q];
                        s++;
                        if(s > cols-2)
                            {
                                r++;
                                s = 0;
                            }
                    }
                }
            }
            cofdet = cof.determinant();
            inv.mat[i][j] = (fabs(cofdet) < 0.1 ? 0 : sign) * cofdet / det;
        }
    }
    return inv.transpose();
}

//------------------------------------------------------------------
//           Function to multiply 2 matrices             
//-----------------------------------------------------------------
Matrix Matrix::multiply(Matrix other)
{
    if(cols != other.rows) { cout<< " Invalid dimensions !"; return *this;}

    Matrix product(this-> rows, other.cols);
    int i, j, k;

    for(i = 0; i < this-> rows; i++)
    {
        for(j = 0; j < other.cols; j++)
        {
            for(k = 0; k < this-> cols; k++)
            {
                product.mat[i][j] += this->mat[i][k] * other.mat[k][j];
            }
        }
    }
    return product;
}

//------------------------------------------------------------------
//           Function to truncate extremely small float values to 0             
//------------------------------------------------------------------
Matrix Matrix::readjust()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j<cols; j++)
        {
            if(fabs(mat[i][j]) < 1e-5) mat[i][j] = 0;
        }
    }
    return *this;
}

//------------------------------------------------------------------
//           Function to concatenate a matrix vertically           
//-----------------------------------------------------------------
Matrix Matrix::vertcat(Matrix b)
{
    Matrix result(this->rows + b.rows, this->cols);
    int i, j;
    for(i = 0; i < this->rows; i++)
    {
        for(j = 0; j < this->cols; j++)
        {
            result.mat[i][j] = this->mat[i][j];            
        }
    }
    for(i = 0; i < b.rows; i++)
    {
        for(j = 0; j < b.cols; j++)
        {
            result.mat[i+this->rows][j]  = b.mat[i][j] ;
        }
    }
    return result;
}
