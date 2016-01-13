#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;

class Matrix{
public:

    int rows;
    int cols;
    double** mat;

    Matrix(int a=0, int b=0)
    {
        rows = a;
        cols = b;
        mat = new double*[rows];
        for(int i = 0; i < rows; ++i)
        {
            mat[i] = new double[cols];
            for(int j = 0; j < cols; j++)
            {
                mat[i][j] = 0;
            }
        }
    }

    void read_matrix(void);
    void display_matrix(void);
    Matrix transpose();
    void rank(void);
    double determinant();
    Matrix inverse();
    Matrix multiply(Matrix);

};   

void Matrix::read_matrix()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j<cols; j++)
        {
            cin>>mat[i][j];
        }
        cout<<" Next row "<<endl;
    }
}

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

Matrix Matrix::transpose()
{
    Matrix result(rows, cols);
    int i, j;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            result.mat[i][j]  = this->mat[j][i] ;
        }
    }
    return result;
}
void Matrix::rank()
{

}

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

int main()
{
    int vars, eqs;
    cout<<"Enter number of variables"<<endl;
    cin>> vars;
    cout<<" Enter number of equations"<<endl;
    cin>> eqs;

    Matrix A(eqs, vars);
    Matrix b(eqs, 1);

    cout <<"Enter the data of matrix A"<<endl;
    A.read_matrix();
    cout <<"Matrix A"<<endl;
    A.display_matrix();

    cout <<"Enter the data of matrix b"<<endl;
    b.read_matrix();
    cout <<"Matrix b"<<endl;
    b.display_matrix();
    cout<<"Det of A is "<<A.determinant()<<endl;
    cout << " Inverse * Self is "<< endl;
    A.inverse().multiply(A).display_matrix();

    return 0;
}