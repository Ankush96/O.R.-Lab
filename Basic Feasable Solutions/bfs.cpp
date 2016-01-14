#include <iostream>
#include <stdio.h>
#include <math.h>
#include "../Matrix/matrix.h"
#include <limits>

using namespace std;

int count = 0;

//------------------------------------------------------------------
//           Function to find the factorial of a number              
//-----------------------------------------------------------------
int factorial(int n)
{
	if(n == 0 || n == 1) return 1;
	return n * factorial(n-1);
}


//------------------------------------------------------------------
//           Function to find al nCr combinations             
//-----------------------------------------------------------------
void combination(int *arr, int *data, int start, int end, int index, int r, int **comb)
{
    // Current combination is ready to be printed, print it
    if (index == r)
    {
        for (int j = 0; j < r; j++)
            comb[::count][j] = data[j];
        ::count++;

        return;
    }
 
    // replace index with all possible elements. The condition
    // "end-i+1 >= r-index" makes sure that including one element
    // at index will make a combination with remaining elements
    // at remaining positions
    for (int i = start; i <= end && end-i+1 >= r-index; i++)
    {
        data[index] = arr[i];
        combination(arr, data, i+1, end, index+1, r, comb);
    }
}

//------------------------------------------------------------------
//           Function to solve a system of equations of form AX =b             
//-----------------------------------------------------------------
Matrix solve(Matrix A, Matrix b, int vars, int eqs, int *zeros)
{
	Matrix a = A.copy();
	int i,j;
	for( i = eqs; i < vars; i++)
		a.mat[i][zeros[i-eqs]-1] = 1;
	if(fabs(a.determinant()) > 0.01)
		return a.inverse().multiply(b);
	else
	{
		Matrix res(vars, 1);
		for( i = 0; i < vars; i++) res.mat[i][0] = -1;
		return res;	
	}
}

//------------------------------------------------------------------
//           Function to find all solutions and then eliminate
//			 all non-feasible solutions and to finally obtain 
//			 Basic Feasible solutions and find optimal solution             
//-----------------------------------------------------------------
void find_optimum(Matrix A, Matrix b, int **combinations, int vars, int eqs, int sols, Matrix Z, bool max)
{
	int i, j, flag;

	double min_z = std::numeric_limits<double>::max();
	double max_z = std::numeric_limits<double>::min();
	double z;

	Matrix basic(vars, 1);

	for(i = 0; i < sols; i++)
	{	
		flag = 0;

		basic = solve(A, b, vars, eqs, combinations[i]).copy();
		basic.readjust();
		cout << endl << "Solution is " << endl;
		basic.transpose().display_matrix();

		for(j = 0; j < vars; j++)
			if(basic.mat[j][0] < 0) flag = 1;
		
		if(flag)
		{
			cout << "Not a feasible solution" << endl;
			continue;
		}	
		else
		{
			cout << "Basic feasible solution found" << endl;
			z = Z.multiply(basic).mat[0][0];
			if(z > max_z) max_z = z;
			if(z < min_z) min_z = z;

			cout << "Value of Z is " << z << endl;
		}


	}
	if(max)
		cout << endl << "Max value of Z is " << max_z << endl;
	else
		cout << endl << "Min value of Z is " << min_z << endl; 
}
int main()
{
    int vars, eqs, i, j, flag;
    double max_z, min_z;
    double *optimal_sol;
    cout<<"Enter number of variables"<<endl;
    cin>> vars;
    cout<<"Enter number of equations"<<endl;
    cin>> eqs;

    Matrix A(eqs, vars);
    Matrix Z(1, vars);
    Matrix A_square(vars, vars);
    Matrix b_big(vars, 1);

    cout <<"Enter the data of matrix A"<<endl;
    A.read_matrix();
    cout <<"Matrix A :-"<<endl;
    A.display_matrix();
    flag = 0;

    while(A.rank() != eqs)
    {
    	flag = 1;
    	cout << "Rank is less than the number of equations" << endl;
    	cout << "Check for linear dependency in constraints " << endl;
    	eqs = A.rank();
    	cout << "You should have " << eqs;
    	cout << " independent equations " << endl;

    	Matrix A(eqs, vars);
    	cout << "Enter matrix A with independent equations" << endl;
    	A.read_matrix();
    }

    Matrix b(eqs, 1);
    cout <<"Enter the data of matrix b"<<endl;
    b.read_matrix();
    	
    cout <<"Matrix b :-"<<endl;
    b.display_matrix();

    cout <<"Enter the coefficients of the "<<vars<<" variables in the objective function Z in order"<<endl;
    Z.read_matrix();
    cout<< "Objective function is "<<endl;
    Z.display_matrix();
    cout << "Enter 1 if it is a maximization problem else 0" << endl;
    bool max_or_min;
    cin >> max_or_min;


    for(i = 0; i < eqs; i++)
    {
    	for(j = 0; j < vars; j++)	A_square.mat[i][j] = A.mat[i][j];
    	b_big.mat[i][0] = b.mat[i][0];
    }

    int rank = A.rank();
    cout << endl << "Beginning Basic Feasible Solution Method . . ." <<endl;
    cout << "We set " << vars - rank << " variables to be 0 at a time " <<endl;
    optimal_sol = new double[vars];

    int sols = factorial(vars) / (factorial(vars - rank) * factorial(rank));
    cout << "Number of solutions possible is " << sols <<endl;

    int **combinations = new int*[sols];
    for(i = 0; i < sols; i++)
    	combinations[i] = new int[vars - rank];

    int *arr = new int[vars];
    for(i = 0; i < vars; i++) arr[i] = i+1;

    int* data = new int[rank];
    combination(arr, data, 0, vars-1, 0, vars - rank, combinations);

    find_optimum(A_square, b_big, combinations, vars, eqs, sols, Z, max_or_min);




    return 0;
}