#include <iostream>
#include <stdio.h>
#include <math.h>
#include "../Matrix/matrix.h"
#include <limits>

using namespace std;

void simplex(Matrix Tableau, Matrix non_basic_vars, Matrix basic_vars)
{
    int i,j;
    int most_negative = 0;
    int pivot_row = 0;
    double least = 0;
    for( j = 0; j < Tableau.cols - 1; j++)
    {
        if(Tableau.mat[Tableau.rows - 1][j] <  least)
        {
            least = Tableau.mat[Tableau.rows - 1][j];
            most_negative = j;
        }
    }

    double min_ratio = std::numeric_limits<double>::max();
    for(i = 0; i < Tableau.rows-1; i++)
    {
        if(fabs(Tableau.mat[i][most_negative]) > 0.0001)
        {
            double ratio = Tableau.mat[i][Tableau.cols-1] / Tableau.mat[i][most_negative];
            if(ratio < min_ratio)
            {
                min_ratio = ratio;
                pivot_row = i; 
            }  
        }
    }
    cout << "Pivot element is " << Tableau.mat[pivot_row][most_negative] <<" at ";
    cout <<pivot_row << " " << most_negative <<endl;
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
    // int extra_vars = 0;
    // int *artificial_count = new int[eqs];
    // for(i = 0; i < eqs; i++)
    // {
    //     Matrix tmp(1,1);
    //     cin >> rel.mat[i][0];
    //     if(fabs(rel.mat[i][0]+1) < 0.0001)
    //     {
    //         cout << "Adding slack variable" <<endl;
    //         Matrix slack(eqs, 1);
    //         slack.mat[i][0] = 1;
    //         A = A.horzcat(slack);
    //         Z = Z.horzcat(tmp);
    //         A.display_matrix();
    //         artificial_count[extra_vars ++] = 0;
    //     }
    //     else if(fabs(rel.mat[i][0]) < 0.0001)
    //     {
    //         cout << "Adding artificial variable" <<endl;
    //         Matrix art(eqs, 1);
    //         art.mat[i][0] = 1;
    //         A = A.horzcat(art);
    //         Z = Z.horzcat(tmp);
    //         A.display_matrix();
    //         artificial_count[extra_vars ++] = 1;
    //     }
    //     else if(fabs(rel.mat[i][0] - 1) < 0.0001)
    //     {
    //         cout << "Adding surplus and artificial variable" <<endl;
    //         Matrix surplus(eqs, 1);
    //         surplus.mat[i][0] = -1;
    //         A = A.horzcat(surplus);
    //         Z = Z.horzcat(tmp);
    //         A.display_matrix();
    //         artificial_count[extra_vars ++] = 0;
    //         Matrix art(eqs, 1);
    //         art.mat[i][0] = 1;
    //         A = A.horzcat(art);
    //         Z = Z.horzcat(tmp);
    //         A.display_matrix();
    //         artificial_count[extra_vars ++] = 1;
    //     }
    // }

    // for(i = 0; i < A.rows; i++)
    // {
    //     for(j = 0; j < vars; j++)
    //     {
    //         if(fabs(A.mat[i][j] - Z.mat[0][j]) > 0.0001)
    //             break;
    //     } 
    //     if(j == vars)
    //         cout << "Infinite solutions expected" << endl;
    // }
    Matrix init_tableau = A.horzcat(b);
    Matrix tmp(1,1,-1);
    tmp = tmp.multiply(Z);
    init_tableau = init_tableau.vertcat(tmp.horzcat(Matrix(1,1)));
    cout << "Initial Tableau" <<endl;
    init_tableau.display_matrix();
    Matrix non_basic_vars(1,vars);
    for(i = 0; i < vars; i++)    non_basic_vars.mat[0][i] = -1 * (i+1);
    cout <<"Non basic variables" <<endl;
    non_basic_vars.display_matrix();
    Matrix basic_vars(eqs,1);
    for(i = 0; i < eqs; i++)    basic_vars.mat[i][0] = i+1;    
    cout <<"Basic variables" <<endl;
    basic_vars.display_matrix();

    simplex(init_tableau, non_basic_vars, basic_vars);
}