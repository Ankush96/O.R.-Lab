#include <iostream>
#include <stdio.h>
#include <math.h>
#include "../Matrix/matrix.h"
using namespace std;

int main()
{
    int vars;
    cout<<"Enter number of rows"<<endl;
    cin>> vars;

    Matrix A(vars, vars);
    Matrix c(vars, 1);
    Matrix Bk(vars, vars);
    for(int i = 0; i < vars; i++)
    {
        for (int j = 0; j < vars; ++j)
        {
            if(i==j)  Bk.mat[i][j] = 1;
        }
    }

    
    cout <<"Enter the matrix whose inverse we want to find"<<endl;
    A.read_matrix();
    cout<< "Entered matrix is "<<endl;
    A.display_matrix();

    if(fabs(A.determinant()) < 0.0001)
    {
        cout << "Singular matrix. Aborting!" <<endl;
        return -1;
    }
    cout << "================================================================="<<endl;
    for(int r = 0; r < vars; r++)
    {

        Matrix E(vars, vars);
        for(int i = 0; i < vars; i++)
        {
            for (int j = 0; j < vars; ++j)
            {
                if(i==j)   E.mat[i][j] = 1;
            }
        }
        // Get rth column
        for(int i = 0; i < vars; i++)
        {
            c.mat[i][0] = A.mat[i][r];
        }
        cout << "The known inverse is : - "<< endl;
        Bk.display_matrix();

        cout << "C is " << endl;
        c.display_matrix();

        c = Bk.multiply(c);
        cout << "inv(B)*C is " << endl;
        c.display_matrix();


        // Get eta

        double pivot = c.mat[r][0];
        for(int i = 0; i < vars; i++)
        {
            if(i != r)
            {
                c.mat[i][0] = (-1.0 * c.mat[i][0] ) / pivot; 
            }
            else
                c.mat[r][0] = 1.0 / pivot;  
            E.mat[i][r] = c.mat[i][0];  
        }

        cout << "E" << r+1 << " is" << endl;
        E.display_matrix();
        Bk = E.multiply(Bk);
        cout << "Inverse at this step is "<< endl;
        Bk.display_matrix();
        cout << "================================================================="<<endl;
    }
    Bk.multiply(A).display_matrix();

    return 0;
}