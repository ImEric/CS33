#include "func.h"
#include "omp.h"
#define BLOCKSIZE 64

void Func1(int c[][TSIZE], int a[][TSIZE], int b[][TSIZE])
{
  
  int i, j, k, r;
#pragma omp parallel shared(a,b,c) private(i,j,k,r)
  {
#pragma omp for schedule(static)
	for (i=0; i<TSIZE; i++) {
   		for (k=0; k<TSIZE; k++) {
		        r = a[i][k];
   			for (j=0; j<TSIZE; j++) {
   				c[i][j]+=r*b[k][j];
			}
		}
	}

  }
}

void Func2(int d[][MSIZE], int c[][MSIZE])
{  
  int blockrow,blockcolumn,i,j;
#pragma omp parallel shared(c,d) private(blockrow,blockcolumn,i,j)
  {
#pragma omp for
  
    for (blockrow = 0; blockrow < MSIZE; blockrow += BLOCKSIZE) {
      for (blockcolumn = 0; blockcolumn < MSIZE; blockcolumn += BLOCKSIZE) {
	for (i = blockrow; i < blockrow + BLOCKSIZE; i++) {
	  for (j = blockcolumn; j < blockcolumn + BLOCKSIZE; j++) {
	    d[j][i] = c[i][j];
	  }
	}
      }
    }
  }
}


void Func3(int z[][MSIZE], int d[][MSIZE])
{
  int y, x,blockrow,blockcolumn;
  int near = 1;  // The weight of neighbor
  int itself = 42; // The weight of center



#pragma omp parallel shared(z,d) private(blockrow,blockcolumn,x,y)
  {
    #pragma omp for

    for (blockrow = 0; blockrow < MSIZE; blockrow += BLOCKSIZE) {
      for (blockcolumn = 0; blockcolumn < MSIZE; blockcolumn += BLOCKSIZE) {
	for (y = blockrow; y < blockrow + BLOCKSIZE; y++) {
	  for (x = blockcolumn; x < blockcolumn + BLOCKSIZE; x++) {
	    int current = itself * d[y][x];

	    if (y!=0 && y!=MSIZE-1) {

	      if (x==0) {
		current +=  d[y-1][x] +
		   d[y-1][x+1] +
		   d[y+1][x] +
		   d[y+1][x+1] +
		   d[y][x] +
		   d[y][x+1] +
		   d[y-1][x] +
		  d[y+1][x];
	      } else if (x==MSIZE-1) {
		current +=  d[y-1][x-1] +
		   d[y-1][x] +
		   d[y+1][x-1] +
		   d[y+1][x] +
		   d[y][x-1] +
		   d[y][x] +
		   d[y-1][x] +
		  d[y+1][x];
	      } else {
		current +=  d[y-1][x-1] +
		   d[y-1][x+1] +
		   d[y+1][x-1] +
		   d[y+1][x+1] +
		   d[y][x-1] +
		   d[y][x+1] +
		   d[y-1][x] +
		  d[y+1][x];
	      }

	    } else if (y==MSIZE-1) {
	      if (x==0) {
		current +=  d[y-1][x] +
		   d[y-1][x+1] +
		   d[y][x] +
		   d[y][x+1] +
		   d[y][x] +
		   d[y][x+1] +
		   d[y-1][x] +
		  d[y][x];
	      } else if (x==MSIZE-1) {
		current +=  d[y-1][x-1] +
		   d[y-1][x] +
		   d[y][x-1] +
		   d[y][x] +
		   d[y][x-1] +
		   d[y][x] +
		   d[y-1][x] +
		  d[y][x];
	      } else {
		current +=  d[y-1][x-1] +
		   d[y-1][x+1] +
		   d[y][x-1] +
		   d[y][x+1] +
		   d[y][x-1] +
		   d[y][x+1] +
		   d[y-1][x] +
		  d[y][x];
	      }
	    } else {

	      if (x==0) {
		current +=  d[y][x] +
		   d[y][x+1] +
		   d[y+1][x] +
		   d[y+1][x+1] +
		   d[y][x] +
		   d[y][x+1] +
		   d[y][x] +
		  d[y+1][x];
	      } else if (x==MSIZE-1) {
		current +=  d[y][x-1] +
		   d[y][x] +
		   d[y+1][x-1] +
		   d[y+1][x] +
		   d[y][x-1] +
		   d[y][x] +
		   d[y][x] +
		  d[y+1][x];
	      } else {
		current +=  d[y][x-1] +
		   d[y][x+1] +
		   d[y+1][x-1] +
		   d[y+1][x+1] +
		   d[y][x-1] +
		   d[y][x+1] +
		   d[y][x] +
		  d[y+1][x];
	      }

	    }
	    current/=50;
	    z[y][x] = current;
	  }
	}
      }
    }
  }
}


/*

void Func3(int z[][MSIZE], int d[][MSIZE])
{
  int y, x, blockrow,blockcolumn,current;
  int near = 1;  // The weight of neighbor
  int itself = 42; // The weight of center


    for (blockrow = 0; blockrow < MSIZE; blockrow += BLOCKSIZE) {
      for (blockcolumn = 0; blockcolumn < MSIZE; blockcolumn += BLOCKSIZE) {
	for (y = blockrow; y < blockrow + BLOCKSIZE; y++) {
	  int top = (y==0) ? y : y-1;
	  int bot = (y==MSIZE-1) ? y : y+1;
	  for (x = blockcolumn; x < blockcolumn + BLOCKSIZE; x++) {
	    int left = (x==0) ? x : x-1;
	    int right = (x==MSIZE-1) ? x : x+1;
	    current = itself * d[y][x];
	         current += d[top][left]
		   + d[top][x]
		   + d[top][right]
		   + d[y][left]
		   + d[y][right]
		   + d[bot][left]
		   + d[bot][x]
		   + d[bot][right];
	         current/=50;
		z[y][x] = current;
	  }
	}
      }
    }
}


*/

void Func4(int b[], int a[])
{
  int chuck_size=MSIZE;
  int array_size=VSIZE/chuck_size;
  int chuck[chuck_size];
  int i, j, sum;
  
#pragma omp parallel shared(chuck,b) private(i,j)
  {
    #pragma omp for
    for(j=0; j<chuck_size; j++) {
      int pos = j*array_size;
      b[pos]=a[pos];
      for (i=1; i<array_size; i++) {
	int temp = pos+i;
	b[temp]=b[temp-1]+a[temp];
      }
      chuck[j]=b[pos+array_size-1];
    }
  }

  sum = 0;


  for(j=0; j<chuck_size-1; j++) {
    sum += chuck[j];
    chuck[j] = sum;
  }
 

#pragma omp parallel shared(chuck,b) private(i,j)
  {
    #pragma omp for
    for(j=1; j<chuck_size; j++) {
      int pos = j*array_size;
      int divide = chuck[j-1]/(j+1);
      for (i=0; i<array_size; i++) {
	b[pos+i]+=divide;
      }
    }
  }
}


void Func5(int b[], int a[])
{
  int i=0, j,  stride, stride2, step;
  int temp;
  long log_N=log2(VSIZE);

  
#pragma omp parallel shared(b) private(j)
{
#pragma omp for

  for(j=0; j<VSIZE; j+=2) {
    int temp = a[j];
    b[j]=temp;
    b[j+1] = temp + a[j+1];
  }
}

 for(i=4; i<VSIZE; i<<=1) {
#pragma omp paralell for private(j)
   for(j=0; j<VSIZE; j+=i) {
     int pos = j+i-1;
     int temp1 = b[j+i/2-1];
     int temp2 = b[pos];
     b[pos] = temp1+temp2;
     
     
   }
 }

 
  b[VSIZE-1]=0;

  for(i=(log_N-1);i>=0; i--) {
    int pow_i = (int)(pow(2, i));
    int pow_i1 = (int)(pow(2, i+1));
#pragma omp parallel for shared(b) private(j)
    for(j=0; j<VSIZE; j+=pow_i1) {
      int temp1=b[j+pow_i-1];
      int temp2=b[j+pow_i1-1];
      b[j+pow_i-1] = temp2;
      b[j+pow_i1-1] = temp1+temp2;
    }
  }
}
