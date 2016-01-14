#include <iostream>
#include <stdio.h>
#include <math.h>
#include "../Matrix/matrix.h"
#include <limits>

using namespace std;

int count = 0;

int factorial(int n)
{
	if(n == 0 || n == 1) return 1;
	return n * factorial(n-1);
}

/* arr[]  ---> Input Array
   data[] ---> Temporary array to store current combination
   start & end ---> Staring and Ending indexes in arr[]
   index  ---> Current index in data[]
   r ---> Size of a combination to be printed */
void combination(int *arr, int *data, int start, int end,
                     int index, int r, int **comb)
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
void find_optimum(Matrix A, Matrix b, int **combinations, int vars, int eqs, int sols, Matrix Z)
{
	int i, j, flag;

	double min_z = std::numeric_limits<double>::max();
	double max_z = std::numeric_limits<double>::min();
	double z;

	Matrix basic(vars, 1);
	for(i = 0; i < sols; i++)
	{	
		flag = 0;
		for(j = 0; j < 2; j++)
			cout<<combinations[i][j]<<"\t";
		cout<<endl;
		basic = solve(A, b, vars, eqs, combinations[i]).copy();
		basic.readjust();
		basic.display_matrix();
		for(j = 0; j < vars; j++)
			if(basic.mat[j][0] < 0) flag = 1;
		
		if(flag) continue;
		else
		{
			cout << "Basic feasible solution found"<<endl;
			Z.multiply(basic).display_matrix();
			cout << "Z is ";
		}


	}
}
int main()
{
    int vars, eqs, i, j;
    double max_z, min_z;
    double *optimal_sol;
    cout<<"Enter number of variables"<<endl;
    cin>> vars;
    cout<<" Enter number of equations"<<endl;
    cin>> eqs;

    Matrix A(eqs, vars);
    Matrix b(eqs, 1);
    Matrix Z(1, vars);
    Matrix A_square(vars, vars);
    Matrix b_big(vars, 1);

    cout <<"Enter the data of matrix A"<<endl;
    A.read_matrix();
    cout <<"Matrix A :-"<<endl;
    A.display_matrix();

    cout <<"Enter the data of matrix b"<<endl;
    b.read_matrix();
    cout <<"Matrix b :-"<<endl;
    b.display_matrix();

    cout <<"Enter the coefficients of the "<<vars<<" variables in the objective function Z in order"<<endl;
    Z.read_matrix();
    cout<< "Objective function is "<<endl;
    Z.display_matrix();



    for(i = 0; i < eqs; i++)
    {
    	for(j = 0; j < vars; j++)	A_square.mat[i][j] = A.mat[i][j];
    	b_big.mat[i][0] = b.mat[i][0];
    }

    int rank = eqs;
    // int rank = A.rank();
    // Set vars - rank variables to be 0
    optimal_sol = new double[vars];

    int sols = factorial(vars) / (factorial(vars - rank) * factorial(rank));
    cout << "Number of sols possible is " << sols <<endl;

    int **combinations = new int*[sols];
    for(i = 0; i < sols; i++)
    	combinations[i] = new int[vars - rank];

    int *arr = new int[vars];
    for(i = 0; i < vars; i++) arr[i] = i+1;

    int* data = new int[rank];
    combination(arr, data, 0, vars-1, 0, vars - rank, combinations);

    for(i = 0; i < sols; i++)
    {
    	for(j = 0; j < (vars-rank); j++) cout << combinations[i][j] << "\t";
    	cout << endl;	
    }
    find_optimum(A_square, b_big, combinations, vars, eqs, sols, Z);




    return 0;
}