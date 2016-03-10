#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "../Matrix/matrix.h"
using namespace std;

Matrix poi_inverse(Matrix Binv, Matrix c, int r)
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

int main()
{
	int vars, eqs, i, j, opt_type;

	cout<<"Enter number of variables"<<endl;
    cin>> vars;
    cout<<"Enter number of equations"<<endl;
    cin>> eqs;
    vector<Matrix> P;
    Matrix nbv(1,vars);
    Matrix bv(1,eqs);

    for(i = 0; i < vars; i++)
    {
    	cout << "Enter the coefficients of variable X" << i+1 << " in all of the equations in order" <<endl;
    	Matrix p(eqs,1);
    	p.read_matrix();
    	P.push_back(p);
    	nbv.mat[0][i] = i;
    }

    cout << "Enter the inequality signs in order. 1 for =, 2 for <=, 3 for >=" << endl;
    for(i = 0; i < eqs; i++)
    {
    	int s;
    	cin >> s;
    	Matrix p(eqs,1,0);
    	bv.mat[0][i] = vars + i;
    	p.mat[i][0] = 1;

    	if(s == 2)	{} 
    	else if(s == 1)
    	{
    		// @todo Artificial Var count
    	}
    	else { cout << " Revised Simplex method is not applicable here" << endl; return -1; } 
    	P.push_back(p);
    }

    cout << "Enter b" << endl;
    Matrix b(eqs,1);
    b.read_matrix();

    cout << "Enter 1 if it is a maximization problem else -1" << endl;
    cin>> opt_type;
    cout << "Enter the coefficients of the variables in the objective function in order" << endl;
    Matrix Z(1,vars+eqs,0);
    for(i = 0; i < vars; i++)
    {
    	int t;
    	cin >> t;
    	Z.mat[0][i] = opt_type*t;
    }

    // Printing initial table
    Matrix A = P[0].copy();
    for(i = 1; i < vars + eqs +1; i++)
    	A = A.horzcat(P[i]);
    A = A.horzcat(b);
    cout << "Initial Table" << endl;
    A.display_matrix();

    cout << "Non-Basic variable numbers are \t";
    nbv.display_matrix();
    cout << "Basic variable numbers are \t";
    bv.display_matrix();

    cout << "Objective Function is " << endl;
    Z.display_matrix();

    cout << "================================================================="<<endl;
    int stop = 0;

    Matrix B(eqs,eqs,0);
    for(i=0; i<eqs; i++) B.mat[i][i] = 1;
    Matrix Binv = B.copy();
	Matrix xb = Binv.multiply(b);

	cout << "Initial Solution is "<<endl;
	xb.display_matrix();
	Matrix cb(1,eqs);
    cout << "================================================================="<<endl;
    while(!stop)
    {
    	cout << "New iteration beginning...\n" <<endl;
    	
	    for(i = 0; i < eqs; i++)
	    	cb.mat[0][i] = Z.mat[0][(int)bv.mat[0][i]];
	  	cout << "Cb is \t";
	  	cb.display_matrix();

	  	int entering = -1;
	  	double largest_neg = 1;
	  	Matrix Y =cb.multiply(Binv);
	  	cout << "Z - C" << endl;
	  	for(i = 0; i < vars; i++)
	  	{
	  		int n = (int)nbv.mat[0][i];
	  		cout << "Variable X" << n+1 << " : ";
	  		double t = Y.multiply(P[n]).mat[0][0] - Z.mat[0][n];
	  		cout << t <<endl;
	  		if(t < largest_neg)
	  		{
	  			largest_neg = t;
	  			entering = n;
	  		}
	  	}
	  	if(largest_neg >= 0)
	  	{
	  		stop =1;
	  		break;
	  	}
	  	cout << "Entering variable is X" << entering+1 << endl;

    	cout << "================================================================="<<endl;

    	Matrix alp = Binv.multiply(P[entering]);
    	alp.readjust();
    	cout << "Alpha is" << endl;
    	alp.display_matrix();
    	int leaving = -1;
    	double min_ratio = 1e10;
    	for( i = 0; i < eqs; i++)
    	{	
    		if(alp.mat[i][0] <0.00001) continue;
    		double ratio = xb.mat[i][0]/alp.mat[i][0];
    		cout << "ratio "<<i+1<< " is " <<ratio<<endl;
    		if(ratio < min_ratio)
    		{
    			min_ratio = ratio;
    			leaving = i;
    		}
    	}
    	if(leaving == -1)
    	{
    		cout << "Problem is unbounded" << endl;
    		return -1;
    	}
    	leaving= (int)bv.mat[0][leaving];

	  	cout << "Leaving variable is X" << leaving+1 << endl;

    	cout << "================================================================="<<endl;

    	for(i = 0; i < eqs; i++)
    	{
    		if((int)bv.mat[0][i] == leaving)	
    		{
    			bv.mat[0][i] = entering;
    			break;
    		}	
    	}
    	for(i = 0; i < vars; i++)
    	{
    		if((int)nbv.mat[0][i] == entering)	
    		{
    			nbv.mat[0][i] = leaving;
    			break;
    		}	
    	}

	    cout << "Non-Basic variable numbers are \t";
	    nbv.display_matrix();
	    cout << "Basic variable numbers are \t";
	    bv.display_matrix();
	    B = P[(int)bv.mat[0][0]].copy();
	    for(i = 1; i < eqs; i++)
	    {
	    	int b = (int)bv.mat[0][i];
	    	B = B.horzcat(P[b]);
	    }
	    cout << "New B is" <<endl;
	    B.display_matrix();

	    Binv = poi_inverse(Binv.copy(), P[entering].copy(), leaving - vars);
	    cout << "New inverse calculated is" <<endl;
	    Binv.display_matrix();

	    xb = Binv.multiply(b);
		cout << "Present Solution is "<<endl;
		xb.display_matrix();
    	cout << "================================================================="<<endl;

    }
    cout << "================================================================="<<endl;

    cout << "Stopping iterations because no negative values found" << endl;

    cout << "Optimal value of Z is \t" << opt_type*cb.multiply(xb).mat[0][0]<<endl;
    cout << "Optimal solution is "<<endl;
    for(i=0; i<vars; i++)
    {
    	int flag = 0;
    	for(j = 0; j < eqs; j++)
    	{	
    		if(i == (int)bv.mat[0][j])
    		{	
    			cout << "X" << i+1 <<" = \t" << xb.mat[j][0] <<endl;
    			flag = 1;
    			break;
    		}
    	}
    	if(!flag)
    		cout << "X" << i+1 <<" = \t" << 0 <<endl;
    }
    cout << "================================================================="<<endl;


    return 0;
}