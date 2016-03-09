//-----------------------------------------------------------------
//  Code for 2 phase Simplex Method     
//-----------------------------------------------------------------
#include <stdio.h>

int minpos(float a[],int n)
{
  int i,min_pos;
  float min=9999;
  for(i=0;i<n;i++){
    if(a[i]<min){
      min=a[i];
      min_pos=i;
    }
  }
  return min_pos;
}

int main()
{
    int surplus_var,artificial_var,slack_var,var;
    int M=1000;
    int n,d,j,i;

//-----------------------------------------------------------------
//       Read the necessary parameters         
//-----------------------------------------------------------------
    printf("Enter the number of variables\n");
    scanf("%d",&d);
    printf("Enter the number of equal constraints\n");
    scanf("%d",&artificial_var);
    printf("Enter the number of greater than constraints\n");
    scanf("%d",&surplus_var);
    printf("Enter the number of less than constraints\n");
    scanf("%d",&slack_var);
    var=d;

//-----------------------------------------------------------------
//       Read the necessary coefficients          
//-----------------------------------------------------------------
    float mat[artificial_var+surplus_var+slack_var+1][d+surplus_var+1],maineqn[d+1];
    for(i=0;i<artificial_var+surplus_var+slack_var+1;i++)
      for(j=0;j<d+surplus_var+1;j++)
  	     mat[i][j]=0;
    printf("Enter coefficients in the following order\n\t1. Equality\n\t2. Greater than\n\t3. Less than\n");
    for(i=0;i<artificial_var+surplus_var+slack_var;i++)
  	{
  	  for(j=0;j<d;j++)
  	    {
  	      scanf("%f",&mat[i][j]);
  	    }
  	  scanf("%f",&mat[i][d+surplus_var]);
  	}
    printf("Enter coefficients of objective function in order inlcuding the constant. Use -ve sign for minimization problem\n");
    
//-----------------------------------------------------------------
//       Assigning the extra variables         
//-----------------------------------------------------------------
    for(j=0;j<d+1;j++){
    	scanf("%f",&maineqn[j]);
    }
    for(i=0;i<artificial_var;i++)
    {
      for(j=d;j<d+surplus_var;j++)
    	 mat[i][j]=0;
    }
    int c=0,z=0;
    for(i=artificial_var;i<artificial_var+surplus_var;i++)
    {
    	mat[i][d+c]=-1;
    	c++;
    }
    for(j=0;j<d+surplus_var+1;j++){
      for(i=0;i<artificial_var+surplus_var;i++)
        z=z+mat[i][j];
      mat[artificial_var+surplus_var+slack_var][j]=-z;
      z=0;
    }
    printf("\n=========================================================================\n");
    printf("Initial Matrix\n\n");
    for(i=0;i<artificial_var+surplus_var+slack_var+1;i++)
    {
  	  for(j=0;j<d+surplus_var+1;j++)
  	     printf("%f\t",mat[i][j]);
      printf("\n");
    }
    printf("\n=========================================================================\n");
    n=artificial_var+surplus_var+slack_var;
    d=d+surplus_var;
    float temp[d],temp1[n],pivot;
    int nbv[d],bv[n],tp,pivot_pos,most_neg=-1,k,m;
    for(i=0;i<d;i++)
      {
        nbv[i]=i+1;
      }
    for(i=0;i<n;i++)
      {
        bv[i]=i+d+1;
      }
    printf("\nNon basic - \n");
    for(i=0;i<d;i++)
      printf("%d\t",nbv[i]);
    printf("\nBasic - \n");
    for(i=0;i<n;i++)
      printf("%d\t",bv[i]);
    printf("\n");
    while(most_neg==-1)
    {
    for(i=0;i<d;i++)
    {
      temp[i]=mat[n][i];
    }
    most_neg=minpos(temp,d);
    if(mat[n][most_neg]>=0){
      if(mat[n][most_neg]==0){
        for(i=0;i<n;i++){
          if(mat[i][most_neg]>0)
            {
               temp1[i]=mat[i][d]/mat[i][most_neg];
              }
              else
              temp1[i]=9999;
  	    }
       pivot_pos=minpos(temp1,n);
    printf("\n=========================================================================\n");
       if(temp1[pivot_pos]!=9999)
         printf("Infinite solutions exist !!\n");
      }
      break;
    }
    for(i=0;i<n;i++){
      if(mat[i][most_neg]>0)
      {
        temp1[i]=mat[i][d]/mat[i][most_neg];
      }
      else
        temp1[i]=9999;
    }
    pivot_pos=minpos(temp1,n);
    for(i=0;i<n+1;i++)
      {
        if(i!=pivot_pos)
        {
  	for(j=0;j<d+1;j++)
  	{
  	  if(j!=most_neg)
  	    {
  	      mat[i][j]=((mat[i][j]*mat[pivot_pos][most_neg])-(mat[pivot_pos][j]*mat[i][most_neg]))/mat[pivot_pos][most_neg];
  	    }
  	}
        }
        }
    pivot=1/mat[pivot_pos][most_neg];
    tp=nbv[most_neg];
    nbv[most_neg]=bv[pivot_pos];
    bv[pivot_pos]=tp;
    for(i=0;i<n+1;i++)
      {
        if(i!=pivot_pos)
        mat[i][most_neg]=-mat[i][most_neg]/mat[pivot_pos][most_neg];
      }
    for(j=0;j<d+1;j++)
      {
        if(j!=most_neg)
  	mat[pivot_pos][j]=mat[pivot_pos][j]/mat[pivot_pos][most_neg];
      }
    mat[pivot_pos][most_neg]=pivot;
    printf("\n=========================================================================\n");
    printf("Intermediate matrix\n\n");
  for(i=0;i<n+1;i++)
    {
      for(j=0;j<d+1;j++)
        {
  	printf("%f\t",mat[i][j]);
        }
      printf("\n");
      }
   most_neg=-1;
    }

    for(i=0;i<d;i++)
      printf("%d\t",nbv[i]);
    printf("\n");
    for(i=0;i<n;i++)
      printf("%d\t",bv[i]);
    printf("\n");
    printf("\n=========================================================================\n");
 
    if(mat[n][d]!=0)
      printf("Infeasible solution found\n");   //phase2
    else{

      int count=0,k,k1,nbv1[d],flag=0;
      float mat1[n+1][d+1];
      for(k=0;k<d;k++){
        for(j=d+1;j<d+artificial_var+surplus_var+1;j++){
  	if(nbv[k]==j){
  	  flag=1;
  	}
        }
        if(flag!=1){
  	nbv1[count]=nbv[k];
  	count++;
        }
        flag=0;
      }
    int ct=0;
    for(i=0;i<n+1;i++){
      for(j=0;j<d;j++){
        if(nbv[j]==nbv1[ct]){
  	mat1[i][ct]=mat[i][j];
  	ct++;
        }
      }
      k1=ct;
      ct=0;
    }
    for(i=0;i<n+1;i++)
      mat1[i][k1]=mat[i][d];

    for(j=0;j<k1;j++){
      if(nbv1[j]<=var){
        mat1[n][j]=maineqn[nbv1[j]-1];
      }
      else{
        mat1[n][j]=0;
      }
    }
    mat1[n][k1]=maineqn[var];
    float sum=0;
    for(j=0;j<d+1;j++){
      for(i=0;i<n;i++){
        if(bv[i]<=var){
  	sum=sum+maineqn[bv[i]-1]*mat1[i][j];
        }
      }
      mat1[n][j]=mat1[n][j]+sum;
      sum=0;
    }

  for(i=0;i<n+1;i++)
    {
      for(j=0;j<k1+1;j++)
        {
  	printf("%f\t",mat1[i][j]);
        }
      printf("\n");
      }
    d=k1;
    most_neg=-1;
    while(most_neg==-1){
    for(i=0;i<d;i++)
      {
      temp[i]=mat1[n][i];
    }
    most_neg=minpos(temp,d);
    if(mat1[n][most_neg]>=0){
      if(mat1[n][most_neg]==0){
        for(i=0;i<n;i++){
          if(mat1[i][most_neg]>0)
            {
               temp1[i]=mat1[i][d]/mat1[i][most_neg];
              }
              else
              temp1[i]=9999;
  	    }
       pivot_pos=minpos(temp1,n);
       if(temp1[pivot_pos]!=9999)
         printf("There are infinite solution\n");

    printf("\n=========================================================================\n");
      }
      break;
    }
    int flag1=0;
    for(i=0;i<n;i++){
      for(j=var+surplus_var+1;j<var+surplus_var+artificial_var+surplus_var+1;j++){
        if(bv[i]==j){
  	flag1=1;
  	pivot_pos=i;
  	break;
        }
      }
      if(flag1==1)
        break;
    }
    if(flag1!=1){
    for(i=0;i<n;i++){
      if(mat1[i][most_neg]>0)
      {
        temp1[i]=mat1[i][d]/mat1[i][most_neg];
      }
      else
        temp1[i]=9999;
    }
    pivot_pos=minpos(temp1,n);
    }
    for(i=0;i<n+1;i++)
      {
        if(i!=pivot_pos)
        {
  	for(j=0;j<d+1;j++)
  	{
  	  if(j!=most_neg)
  	    {
  	      mat1[i][j]=((mat1[i][j]*mat1[pivot_pos][most_neg])-(mat1[pivot_pos][j]*mat1[i][most_neg]))/mat1[pivot_pos][most_neg];
  	    }
  	}
        }
        }
    pivot=1/mat1[pivot_pos][most_neg];
    tp=nbv1[most_neg];
    nbv1[most_neg]=bv[pivot_pos];
    bv[pivot_pos]=tp;
    for(i=0;i<n+1;i++)
      {
        if(i!=pivot_pos)
        mat1[i][most_neg]=-mat1[i][most_neg]/mat1[pivot_pos][most_neg];
      }
    for(j=0;j<d+1;j++)
      {
        if(j!=most_neg)
  	mat1[pivot_pos][j]=mat1[pivot_pos][j]/mat1[pivot_pos][most_neg];
      }
    mat1[pivot_pos][most_neg]=pivot;

    printf("\n=========================================================================\n");
   printf("Intermediate Matrix\n");
  for(i=0;i<n+1;i++)
    {
      for(j=0;j<d+1;j++)
        {
  	printf("%f\t",mat1[i][j]);
        }
      printf("\n");
      }
   most_neg=-1;
   }

   printf("\n=========================================================================\n");
   printf("Solution is \n");
    for(i=0;i<n;i++)
      printf("x%d=%f\t",bv[i],mat1[i][d]);
    printf("\n\nz=%f\n",mat1[n][d]);
    printf("\n=========================================================================\n");
  for(i=0;i<n+1;i++)
    {
      for(j=0;j<k1+1;j++)
        {
  	printf("%f\t",mat1[i][j]);
        }
      printf("\n");
      }
  }

  printf("\n=========================================================================\n");
  return 0;
 }
