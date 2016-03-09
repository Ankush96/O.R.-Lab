#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int m,n,option,*s;
double **ST,*objfunc;

int mostnegative()
{
  int i,j,flag=-1;
  double minneg=0;
  for(i=0;i<n;i++)
    if(minneg>ST[m][i])
      {
	minneg=ST[m][i];
	flag=i;
      }

  return flag;
}
int dualmostnegative()
{
  int i,j,flag=-1;
  double minneg=0;
  for(i=0;i<m;i++)
    {
      if(minneg>ST[i][n])
	{
	  minneg=ST[i][n];
	  flag=i;
	}
    }

  return flag;
}

int findpivelement(int col)
{
  int i,j,flag=0;
  double min=1000;

  for(i=0;i<m;i++)
    {
      if(ST[i][col]>0)
	{
	  if((min>(ST[i][n]/ST[i][col])) && (ST[i][n]/ST[i][col]>0))
	    {
	      min=(ST[i][n]/ST[i][col]); flag=i;
	    }
	}
    }
  return flag;
}

int dualfindpivelement(int row)
{
  int i,j,flag;
  double min=1000;
  for(i=0;i<m;i++)
    {
      if(ST[row][i]<0)
	{
	  if(fabs(ST[m][i]/ST[row][i])<min)
	    {
	      min=fabs(ST[m][i]/ST[row][i]);flag=i;
	    }
	}
    }
  return flag;
}

void modifyrest(int x,int y)
{
  int i,j;
  double piv=ST[x][y];
  for(i=0;i<m+1;i++)
    for(j=0;j<n+1;j++)
      if(i!=x && j!=y)
	ST[i][j]= ((ST[i][j]*piv)-(ST[i][y]*ST[x][j]))/piv;
}

void modifypivrowcol(int x,int y)
{ int i,j;
  double piv=ST[x][y];

  ST[x][y]= (1/piv);
  for(i=0;i<m+1;i++)
    if(i!=x)
      ST[i][y]/= (-1*piv);

  for(j=0;j<n+1;j++)
    if(j!=y)
      ST[x][j]/=(piv);
}

void printST()
{
  int i,j;
  for(i=0;i<m+1;i++)
    {
      for(j=0;j<n+1;j++)
	printf("%lf ",ST[i][j]);
      printf("\n");
    }
  printf("\n");
}

double impactvalue(int x,int y)
{
  double impactvalue;
  impactvalue=fabs((ST[m][y]*ST[x][n])/ST[x][y]);
  return impactvalue;
}
void input()
{
  int i,j;

  printf("Enter the number of variables\n");
  scanf("%d",&n);
  printf("Enter the number of equations\n");
  scanf("%d",&m);
  int * a=(int*)malloc(m*sizeof(int));
  objfunc=(double*)malloc(n*sizeof(double));

  ST=(double**)malloc((m+1)*sizeof(double*));
  for(i=0;i<m+1;i++)
    ST[i]=(double*)malloc((n+1)*sizeof(double));


  for(i=0;i<m;i++)
    {
      printf("Enter the type of equation %d\n",i+1);
      printf("1: <=,2: >=\n");
      scanf("%d",&a[i]);
    }
  for(i=0;i<m;i++)
    {
      printf("let X%d be a slack variable for equation %d\n",i+n+1,i+1);
    }
  printf("Enter the coefficient of variables in objective function\n");
  for(i=0;i<n;i++)
    {
      printf("Enter the coefficient of X%d\n",i+1);
      scanf("%lf",&objfunc[i]);
    }
  printf("Enter the constant value in objective function\n");
  scanf("%lf",&ST[m][n]);

  for(i=0;i<n;i++)
    {
      ST[m][i]=-objfunc[i];
    }
  for(i=0;i<m;i++)
    {
      printf("Enter coefficients for equation %d\n",i+1);
      for(j=0;j<n;j++)
	{
	  printf("Coefficient of X%d\n",j+1);
	  scanf("%lf",&ST[i][j]);
	}
      printf("\n");
    }
  for(i=0;i<m;i++)
    {
      printf("Enter the constant for equation %d\n",i+1);
      scanf("%lf",&ST[i][n]);
    }

  for(i=0;i<m;i++)
    {
      if(a[i]==2)
	{
	  for(j=0;j<n+1;j++)
	    ST[i][j]=-1*ST[i][j];
	}
    }

}

void exchangeval(int x,int y)
{
  int temp;
  temp=s[y];
  s[y]=s[n+x];
  s[n+x]=temp;
}
int primalconsistent()
{
  int i;
  int con=1;
  for(i=0;i<m;i++)
    {
      if(ST[i][n]<0)con=0;
    }
  if(con==0)return con;
  for(i=0;i<n;i++)
    {
      if(ST[m][i]<0)
	{
	  con=1;
	  break;
	}
      else con=0;
    }
  printf("\nprimal consistent=%d\n",con);
  return con;
}
int dualconsistent()
{
  int i;
  int con=1;
  for(i=0;i<n;i++)
    {
      if(ST[m][i]<0)con=0;
    }
  if(con==0)return con;
  for(i=0;i<m;i++)
    {
      if(ST[i][n]<0)
	{
          con=1;
          break;
	}
      else con=0;
    }
  printf("\n dual consistent=%d\n",con);
  return con;
}

int main()
{

  double opt=0,piv;
  int i,j,flagr,flagc;

  input();
  s=(int*)malloc((m+n)*sizeof(int));
  for(i=0;i<m+n;i++)
    s[i]=i;

  printf("\n");
  printf("\nSimplex table at start:\n");
  printST();
  flagc=mostnegative();
  flagr=dualmostnegative();
  while((flagc>=0||flagr>=0))
    {

      if(primalconsistent()==1)
  {
    flagc=mostnegative();
    int index=1;
    flagr=findpivelement(flagc);
    printf("\nThe pivot value is %lf in row %d column %d\n",ST[flagr][flagc],flagr+1,flagc+1);
    modifyrest(flagr,flagc);
    modifypivrowcol(flagr,flagc);
    exchangeval(flagr,flagc);
    flagc=mostnegative();
    flagr=dualmostnegative();
    printf("\n\tSimplex table after change %d\n",index);
    index++;
    printST();
  }
      else if(dualconsistent()==1)
  {
    flagr=dualmostnegative();
    int index=1;
    flagc=dualfindpivelement(flagr);
    printf("\nThe pivot value is %lf in row %d column %d\n",ST[flagr][flagc],flagr+1,flagc+1);
    modifyrest(flagr,flagc);
    modifypivrowcol(flagr,flagc);
    exchangeval(flagr,flagc);
    flagr=dualmostnegative();
    flagc=mostnegative();
    printf("\n\tSimplex table after change%d\n",index);
    index++;
    printST();
  }
      else
  {
    flagc=mostnegative();
    flagr=findpivelement(flagc);
    double impactvalue1=impactvalue(flagr,flagc);
    printf("\nPrimal impactvalue=%lf\n",impactvalue1);
    flagr=dualmostnegative();
    flagc=dualfindpivelement(flagr);
    double impactvalue2=impactvalue(flagr,flagc);
    printf("\nDual impactvalue=%lf \n",impactvalue2);
    if(fabs(impactvalue2)<=fabs(impactvalue1))
      {
        flagc=mostnegative();
        int index=1;
        flagr=findpivelement(flagc);
        printf("\nThe pivot value is %lf in row %d column %d\n",ST[flagr][flagc],flagr+1,flagc+1);
        modifyrest(flagr,flagc);
        modifypivrowcol(flagr,flagc);
        exchangeval(flagr,flagc);
        flagc=mostnegative();
        flagr=dualmostnegative();
        printf("\n\tSimplex table after change%d\n",index);
        index++;
        printST();
      }
    else if(fabs(impactvalue2)>fabs(impactvalue1))
      {
        flagr=dualmostnegative();
        int index=1;
        flagc=dualfindpivelement(flagr);
        printf("\nThe pivot value is %lf in row %d column %d\n",ST[flagr][flagc],flagr+1,flagc+1);
        modifyrest(flagr,flagc);
        modifypivrowcol(flagr,flagc);
        exchangeval(flagr,flagc);
        flagr=dualmostnegative();
        flagc=mostnegative();
        printf("\n\tSimplex table after change%d\n",index);
        index++;
        printST();
      }
  }
    }

  printf("\nOptimal solution is %lf \n",ST[m][n]);
  printf("\nAt the set of values of variables \n");
  for(i=0;i<m;i++)
    {
      printf("\nX%d=%lf\n",s[n+i]+1,ST[i][n]);
    }
  return 0;
}


/* OUTPUT

sanket@ubuntu:~/Desktop$ cc prog6sanket.c -lm
sanket@ubuntu:~/Desktop$ ./a.out
Enter the number of variables
4
Enter the number of equations
4
Enter the type of equation 1
1: <=,2: >=
1
Enter the type of equation 2
1: <=,2: >=
1
Enter the type of equation 3
1: <=,2: >=
2
Enter the type of equation 4
1: <=,2: >=
2
let X5 be a slack variable for equation 1
let X6 be a slack variable for equation 2
let X7 be a slack variable for equation 3
let X8 be a slack variable for equation 4
Enter the coefficient of variables in objective function
Enter the coefficient of X1
3
Enter the coefficient of X2
4
Enter the coefficient of X3
-5
Enter the coefficient of X4
-2
Enter the constant value in objective function
0
Enter coefficients for equation 1
Coefficient of X1
1
Coefficient of X2
2
Coefficient of X3
2
Coefficient of X4
5

Enter coefficients for equation 2
Coefficient of X1
4
Coefficient of X2
3
Coefficient of X3
1
Coefficient of X4
1

Enter coefficients for equation 3
Coefficient of X1
2
Coefficient of X2
1
Coefficient of X3
2
Coefficient of X4
3

Enter coefficients for equation 4
Coefficient of X1
1
Coefficient of X2
6
Coefficient of X3
3
Coefficient of X4
2

Enter the constant for equation 1
20
Enter the constant for equation 2
55
Enter the constant for equation 3
24
Enter the constant for equation 4
36


Simplex table at start:
1.000000 2.000000 2.000000 5.000000 20.000000
4.000000 3.000000 1.000000 1.000000 55.000000
-2.000000 -1.000000 -2.000000 -3.000000 -24.000000
-1.000000 -6.000000 -3.000000 -2.000000 -36.000000
-3.000000 -4.000000 5.000000 2.000000 0.000000


Primal impactvalue=40.000000

Dual impactvalue=24.000000

The pivot value is 2.000000 in row 1 column 2

	Simplex table after change1
0.500000 0.500000 1.000000 2.500000 10.000000
2.500000 -1.500000 -2.000000 -6.500000 25.000000
-1.500000 0.500000 -1.000000 -0.500000 -14.000000
2.000000 3.000000 3.000000 13.000000 24.000000
-1.000000 2.000000 9.000000 12.000000 40.000000


Primal impactvalue=10.000000

Dual impactvalue=9.333333

The pivot value is 2.500000 in row 2 column 1

	Simplex table after change1
-0.200000 0.800000 1.400000 3.800000 5.000000
0.400000 -0.600000 -0.800000 -2.600000 10.000000
0.600000 -0.400000 -2.200000 -4.400000 1.000000
-0.800000 4.200000 4.600000 18.200000 4.000000
0.400000 1.400000 8.200000 9.400000 50.000000

Optimal solution is 50.000000

At the set of values of variables

X2=5.000000

X1=10.000000

X7=1.000000

X8=4.000000

*/
