#include<stdio.h>

void readA (double A[][10],int[]); 	 				//function to read a A
void printA (double A[][10],int n[]);	 			//function to nrint a A
void update_leading_0s(int leading_0s[],double A[][10],int n[]);	//function to update the leading_0s array
void arrange_A(double A[][10],int n[],int leading_0s[]);		//function to arrange A
void scale_A(double A[][10],int n[],int leading_0s[]);		//Function to scale A

//----------------------------------------------------------------
//                  main function                                 
//---------------------------------------------------------------

main()
{

//nrogram to take a matrix as innut and find its rank.
//x and y are rows and columns
int n[2],i,retest=1,grp,p,r,j,leading_0s[10],t,m,rank;
double A[10][10];
readA(A,n);

printf("\tThe A you have entered is shown below\n\n");
printA(A,n);
update_leading_0s(leading_0s,A,n);
arrange_A(A,n,leading_0s);
if(A[0][0]==0)
{
printf("\n\tError: Invalid marix \n\n");
}
update_leading_0s(leading_0s,A,n);
scale_A(A,n,leading_0s);
while(retest==1)
{
	grp=0;
	for(i=0;i<n[0];++i)
	{

		p=0;
		while(leading_0s[i+p]==leading_0s[i+p+1]&&(i+p+1)<n[0])
		{

			grp=grp+1;
			p=p+1;
		}

		if(grp!=0)
		{
			while(grp!=0)
			{
				for(j=0;j<n[1];++j)
				{
					A[i+grp][j]=A[i+grp][j]-A[i][j];
				}
				grp=grp-1;
			}
			break;
			}
		}	

	update_leading_0s(leading_0s,A,n);
	arrange_A(A,n,leading_0s);
	update_leading_0s(leading_0s,A,n);
	scale_A(A,n,leading_0s);
	retest=0;
	for(r=0;r<n[0];++r)
	{
		if(leading_0s[r]==leading_0s[r+1]&&r+1<n[0])
		{
			if(leading_0s[r]!=n[1])
			retest=1;
		}
	}

}
rank =0;
for (i=0;i<n[0];++i)
{
	if (leading_0s[i]!=n[1])
	{
		++rank;
	}
}

printf("\n Rank Of The A = %d\n\n", rank); 
int RankofA = rank;

}

//----------------------------------------------------------------
//           Function definition to read a A    
//----------------------------------------------------------------
int x,y;
void readA (double mat[][10],int n[])
{

int i,j;
printf("\tEnter the no: rows in the A : ");
scanf("%d",&n[0]);
x=n[0];
printf("\tEnter the no: columns in the A : ");
scanf("%d",&n[1]);
y=n[1];
printf("\tEnter The A Elements Row wise  ");
scanf("\n");

for(i=0;i<n[0];++i)
{

for(j=0;j<n[1];++j)
{
scanf("%lf",&mat[i][j]);
}

}
}

//----------------------------------------------------------------
//               Function definition to nrint a A
//----------------------------------------------------------------


void printA (double mat[][10],int n[])

{
int i,j;
for(i=0;i<n[0];++i)
{
for(j=0;j<n[1];++j)
{
printf("\t%f",mat[i][j]);
}
printf("\n");
}
printf("\n");
}


//------------------------------------------------------------------
//               Function definition to update leading_0s array                   
//-----------------------------------------------------------------

void update_leading_0s ( int leading_0s[],double A[][10],int n[])
{

int zcount,i,j; 
for(i=0;i<n[0];++i)
{
zcount=0;
for(j=0; (A[i][j]==0) && (j<n[1]) ;++j)
{
++zcount;
}
leading_0s[i]=zcount;
}

}


//-------------------------------------------------------------------
//               Function definition to arrange A              
//-------------------------------------------------------------------

void arrange_A(double A[][10],int n[],int leading_0s[])
{
int l,remrow,i,k,lastrow,temnvar,large;
double rowtemn[10];
lastrow=n[0]-1;

for(l=0;l<n[0];++l)
{
large=leading_0s[0];
for(i=0;i<n[0];++i)
{
if(large<=leading_0s[i])
{
large=leading_0s[i];
remrow=i;
}
}

leading_0s[remrow]=leading_0s[lastrow];
leading_0s[lastrow]=-1;

for(k=0;k<n[1];++k)
{
rowtemn[k]=A[lastrow][k];
}
for(k=0;k<n[1];++k)
{
A[lastrow][k]=A[remrow][k];
}
for(k=0;k<n[1];++k)
{
A[remrow][k]=rowtemn[k];
}
lastrow=lastrow-1;
}
}

//---------------------------------------------------------------------------
//                        Function definition to scale a A                              
//---------------------------------------------------------------------------


void scale_A(double A[][10],int n[],int leading_0s[])
{
int i,j;
double divisor;
for(i=0;i<n[0];++i)
{  
divisor=A[i][leading_0s[i]];

for(j=leading_0s[i];j<n[1];++j)
{
A[i][j]=A[i][j]/divisor;
}
}
}

