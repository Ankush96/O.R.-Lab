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
Matrix solve(Matrix augmented, int vars, int eqs, int *zeros)
{
    int i,j;
	Matrix a = augmented.copy();
	for( i = eqs; i < vars; i++)
		a.mat[i][zeros[i-eqs]-1] = 1;

    int *leading_0s = a.row_reduced(a);
    a.readjust();

    Matrix res(vars, 1);
    for(i = a.rows-1; i >= 0; i--)
    {
        if(leading_0s[i] == a.cols -1)
        {
            cout<< "Inconsistent System. No basic solution!" << endl;
            Matrix res(vars, 1, -1);
            return res;
        }
        if(i == a.rows) res.mat[i][0] = a.mat[i][a.cols-1];
        else
        {
            double sum = 0;
            for(j = i+1; j < a.cols - 1; j++) sum += res.mat[j][0]*a.mat[i][j];
            res.mat[i][0] = a.mat[i][a.cols-1] - sum;
        }

    }

	return res;	
}


//------------------------------------------------------------------
//           Function to remove linear dependency and return
//           a row reduced echelon form of the augmented equation    
//-----------------------------------------------------------------
Matrix remove_linear_dependency(Matrix A, Matrix b)
{
    Matrix augmented = A.horzcat(b);
    int *leading_0s = A.row_reduced(augmented);
    augmented.readjust();

    return augmented; 
}

//------------------------------------------------------------------
//           Function to find all solutions and then eliminate
//			 all non-feasible solutions and to finally obtain 
//			 Basic Feasible solutions and find optimal solution             
//-----------------------------------------------------------------
void find_optimum(Matrix augmented, int **combinations, int vars, int extra_vars, 
                    int eqs, int sols, Matrix Z, bool max, int* artificial_count)
{
	int i, j, flag;

	double min_z = std::numeric_limits<double>::max();
	double max_z = std::numeric_limits<double>::min();
	double z;

	Matrix basic(vars, 1);

	for(i = 0; i < sols; i++)
	{	
        // cout << " Go to next solution ? " <<endl;
        // cin >> j;
		flag = 0;

		basic = solve(augmented, vars, eqs, combinations[i]).copy();
		basic.readjust();
		cout << endl << "Solution is " << endl;
		basic.transpose().display_matrix();

		for(j = 0; j < vars - extra_vars; j++)
        {
            if(basic.mat[j][0] < 0)
            {
                flag = 1;
            }
        }
        for(j = vars - extra_vars; j < vars; j++)
        {
            if(basic.mat[j][0] < 0)   flag = 1;
    
            if(fabs(basic.mat[j][0] ) > 0.0001 && artificial_count[j - vars + extra_vars])
            {
                flag = 1;
            }

        }
		
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
    int vars, eqs, i, j;
    double max_z, min_z;
    double *optimal_sol;
    cout<<"Enter number of variables"<<endl;
    cin>> vars;
    cout<<"Enter number of equations"<<endl;
    cin>> eqs;

    Matrix A(eqs, vars);
    Matrix b(eqs, 1);
    Matrix rel(eqs, 1);
    Matrix Z(1, vars);

    cout <<"Enter the coefficients of the "<<vars<<" variables in the objective function Z in order"<<endl;
    Z.read_matrix();
    cout<< "Objective function is "<<endl;
    Z.display_matrix();

    cout << "Enter 1 if it is a maximization problem else 0" << endl;
    bool max_or_min;
    cin >> max_or_min;
    cout <<"Enter the data of matrix A"<<endl;
    A.read_matrix();
    cout <<"Matrix A :-"<<endl;
    A.display_matrix();

    cout <<"Enter the data of matrix b"<<endl;
    b.read_matrix();    	
    cout <<"Matrix b :-"<<endl;
    b.display_matrix();

    cout << " For each of the "<< eqs <<" equations, specify the relation by -1 (<=), 0 (=) and 1 (>=)" <<endl;
    int extra_vars = 0;
    int *artificial_count = new int[eqs];
    for(i = 0; i < eqs; i++)
    {
        Matrix tmp(1,1);
        cin >> rel.mat[i][0];
        if(fabs(rel.mat[i][0]+1) < 0.0001)
        {
            cout << "Adding slack variable" <<endl;
            Matrix slack(eqs, 1);
            slack.mat[i][0] = 1;
            A = A.horzcat(slack);
            Z = Z.horzcat(tmp);
            A.display_matrix();
            artificial_count[extra_vars ++] = 0;
        }
        else if(fabs(rel.mat[i][0]) < 0.0001)
        {
            cout << "Adding artificial variable" <<endl;
            Matrix art(eqs, 1);
            art.mat[i][0] = 1;
            A = A.horzcat(art);
            Z = Z.horzcat(tmp);
            A.display_matrix();
            artificial_count[extra_vars ++] = 1;
        }
        else if(fabs(rel.mat[i][0] - 1) < 0.0001)
        {
            cout << "Adding surplus and artificial variable" <<endl;
            Matrix surplus(eqs, 1);
            surplus.mat[i][0] = -1;
            A = A.horzcat(surplus);
            Z = Z.horzcat(tmp);
            A.display_matrix();
            artificial_count[extra_vars ++] = 0;
            Matrix art(eqs, 1);
            art.mat[i][0] = 1;
            A = A.horzcat(art);
            Z = Z.horzcat(tmp);
            A.display_matrix();
            artificial_count[extra_vars ++] = 1;
        }
    }

    for(i = 0; i < A.rows; i++)
    {
        for(j = 0; j < vars; j++)
        {
            if(fabs(A.mat[i][j] - Z.mat[0][j]) > 0.0001)
                break;
        } 
        if(j == vars)
            cout << "Infinite solutions expected" << endl;
    }
    Matrix A_square(vars + extra_vars, vars + extra_vars);
    Matrix b_big(vars + extra_vars, 1);

    for(i = 0; i < eqs; i++)
    {
    	for(j = 0; j < vars + extra_vars; j++) A_square.mat[i][j] = A.mat[i][j];
    	b_big.mat[i][0] = b.mat[i][0];
    }

    Matrix augmented = remove_linear_dependency(A_square, b_big);
    int *leading_0s = new int[augmented.rows];
    augmented.update_leading_0s(leading_0s, augmented);

    int rank = 0;
    for(i = 0; i < augmented.rows; i++)
    {
        if(leading_0s[i] == augmented.cols -1)
        {
            cout<< "Inconsistent System. Aborting !" << endl;
            return -1;
        }
        if(leading_0s[i] != augmented.cols) ++rank;
    }

    cout << endl << "Beginning Basic Feasible Solution Method . . ." <<endl;
    cout << "We set " << vars + extra_vars- rank << " variables to be 0 at a time " <<endl;
    optimal_sol = new double[vars + extra_vars];

    int sols = factorial(vars + extra_vars) / (factorial(vars + extra_vars - rank) * factorial(rank));
    cout << "Number of solutions possible is " << sols <<endl;

    int **combinations = new int*[sols];
    for(i = 0; i < sols; i++)
    	combinations[i] = new int[vars + extra_vars - rank];

    int *arr = new int[vars + extra_vars];
    for(i = 0; i < vars + extra_vars; i++) arr[i] = i+1;

    int* data = new int[rank];
    combination(arr, data, 0, vars + extra_vars-1, 0, vars+extra_vars - rank, combinations);

    find_optimum(augmented, combinations, vars + extra_vars, extra_vars, rank, sols, Z, max_or_min,artificial_count);




    return 0;
}