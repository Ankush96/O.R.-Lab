#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "../Matrix/matrix.h"
using namespace std;

Matrix single_col_inverse(Matrix Binv, Matrix c, int r)
{
    c = Binv.multiply(c).copy();
    double pivot = c.mat[r][0];
    int vars = Binv.rows;
    Matrix E(vars,vars);
    for(int i = 0; i < vars; i++)
    {
        E.mat[i][i] = 1;
        if(i != r)
        {
            c.mat[i][0] = (-1.0 * c.mat[i][0] ) / pivot; 
        }
        else
            c.mat[r][0] = 1.0 / pivot;  
        E.mat[i][r] = c.mat[i][0];
    }
    return E.multiply(Binv).copy();
}
Matrix poi_inv(Matrix A)
{
    Matrix inv(A.rows, A.cols);
    Matrix C(A.rows, 1);
    for(int i = 0; i < A.rows; i++) inv.mat[i][i] = 1;
    for(int col = 0; col < A.cols; col++)
    {
        for(int i = 0; i < A.rows; i++) C.mat[i][0] = A.mat[i][col];
        inv = single_col_inverse(inv, C, col);
    }
    return inv;
}

int main()
{
	int vars, eqs, i, j, opt_type;
	double alpha = 0.5;

	cout<<"Enter number of variables"<<endl;
    cin>> vars;
    cout<<"Enter number of equations"<<endl;
    cin>> eqs;
    Matrix A(eqs, vars);
    Matrix I(eqs+vars, eqs+vars);
    for(i=0; i<vars+eqs; i++)	I.mat[i][i] = 1;

    cout << "Enter the coefficients corresponding to the variables, 1 equation at a time" <<endl;
    A.read_matrix();

    cout << "Enter b" << endl;
    Matrix b(eqs,1);
    b.read_matrix();

    cout << "Enter the inequality signs in order. 1 for =, 2 for <=, 3 for >=" << endl;
    Matrix artificial(eqs,1);
    for(i = 0; i < eqs; i++)
    {
    	int s;
    	cin >> s;
    	Matrix p(eqs,1,0);
    	p.mat[i][0] = 1;
    	A = A.horzcat(p);
    	if(s == 2)	{} 
    	else if(s == 1)
    	{
    		artificial.mat[i][0] = 1;
    	}
    	else if(s == 3)
    	{
            for(int j = 0; j < vars; j++) A.mat[i][j] *= -1;
            b.mat[i][0] *= -1;    		
    	}
    }

    cout << "Enter 1 if it is a maximization problem else -1" << endl;
    cin>> opt_type;
    cout << "Enter the coefficients of the variables in the objective function in order" << endl;
    Matrix Z(vars+eqs, 1, 0);
    for(i = 0; i < vars; i++)
    {
    	int t;
    	cin >> t;
    	Z.mat[i][0] = opt_type*t;
    }
    cout <<"===============================================" << endl;
    cout << "Enter a trial solution" << endl;
    Matrix X(vars+eqs,1);
    for(int i = 0; i < vars; i++) cin >> X.mat[i][0];
    for(int i = 0; i < eqs; i++)
    {
        double sum = 0;
        double t = 0;
        for(int j = 0; j < vars; j++)   sum += X.mat[j][0]*A.mat[i][j] ;
        t = b.mat[i][0] - sum;
        cout << "Eqt "<<i+1<<" -> " << t << endl;
        if ( t < 0)
            {
                cout << "Trial solution is infeasible." <<endl;
                return -1;
            }
        else  X.mat[vars + i][0] = t;
        

    }
    X.transpose().display_matrix();
    int iter = 1;
    while(1)
    {
        Matrix D(vars+eqs, vars+eqs);
        for(i=0; i<vars+eqs; i++) D.mat[i][i] = X.mat[i][0];
        Matrix A_ = A.multiply(D);
        Matrix c_ = D.multiply(Z);
        Matrix P = A_.multiply(A_.transpose()).copy();
        P = poi_inv(P);
        P = A_.transpose().multiply(P).copy();
        P = P.multiply(A_).copy();
        
        for(i = 0; i < vars + eqs; i++)
        {
            for (j = 0; j < vars+eqs; ++j)
            {
                P.mat[i][j] *= -1;
                if(i == j) P.mat[i][j] += 1;
            }
        }
        Matrix cp = P.multiply(c_);
        double v = 1, l_neg = 0;
        for(i=0; i<vars+eqs; i++)
        {
            if(cp.mat[i][0] < l_neg )
            {   
                l_neg = cp.mat[i][0];
                v = fabs(l_neg);
            }
        }
        Matrix X_(vars+eqs, 1);
        for (int i = 0; i < vars+eqs; ++i)  X_.mat[i][0] = 1 + (alpha/v) * cp.mat[i][0];
        X_ = D.multiply(X_);
        double err= 0;
        for(int i=0; i<vars+eqs; i++) err += pow(fabs(X.mat[i][0] - X_.mat[i][0]), 2);
        err = sqrt(err) / (vars + eqs);
        cout << "Iteration number - " << iter++ << "\tImprovement - " << err << endl;
        if(err < 1e-6 || iter > 500) break;
        X = X_.copy();
    }

    cout << "================================================================="<<endl;
    
    if(iter > 500)
    {
        cout << "Problem is unbounded" <<endl;
        return -1;
    }
    X.readjust();
    for(int i = 0; i < eqs; i++)
    {
        if(fabs(artificial.mat[i][0] -1) < 1e-6)
        {
            if(fabs(X.mat[vars+i][0]) > 1e-6) 
            {
                cout << "Problem is infeasible" << endl;
                return -1;
            }
        }
    }
    double z = X.transpose().multiply(Z).mat[0][0];
    cout << "Optimal value of Z = \t" << opt_type*z << endl;
    cout << "Optimal solution is" << endl;
    
    for(int i=0; i<vars; i++)
    {
        cout << "X" <<i+1 << "\t=\t" << X.mat[i][0] <<"\t";
    }
    cout << endl;


    return 0;
}