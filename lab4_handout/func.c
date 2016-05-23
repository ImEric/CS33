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
  /*  
  int i,j;
#pragma omp parallel for private(j)
	for(i=0; i<MSIZE; i++)
	  for(j=0; j<MSIZE; j++)
	    d[j][i] = c[i][j];

  */
  
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
  int y, x, blockrow, blockcol;
	int near = 2;  		// The weight of neighbor
        int itself = 84; 	// The weight of center
#pragma omp parallel shared(z,d) private(blockrow,blockcol,x,y)
	{
	  #pragma omp parallel for
	for (blockrow=0; blockrow<MSIZE; blockrow+=BLOCKSIZE) {
		for (blockcol=0; blockcol<MSIZE; blockcol+=BLOCKSIZE) {
		  for (y = blockrow; y < blockrow+BLOCKSIZE; y++){
		    for (x = blockcol; x < blockcol+BLOCKSIZE; x++){
		      	  if (y==0) {
				if (x==0) {
					z[y][x] = 	near * d[y][x] +
						near * d[y][x+1] +
						near * d[y+1][x] +
						near * d[y+1][x+1] +
						near * d[y][x] +
						near * d[y][x+1] +
						near * d[y][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				} else if (x==MSIZE-1) {
					z[y][x] = 	near * d[y][x-1] +
						near * d[y][x] +
						near * d[y+1][x-1] +
						near * d[y+1][x] +
						near * d[y][x-1] +
						near * d[y][x] +
						near * d[y][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				} else {
					z[y][x] = 	near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y+1][x-1] +
						near * d[y+1][x+1] +
						near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				}
			} else if (y==MSIZE-1) {
				if (x==0) {
					z[y][x] = 	near * d[y-1][x] +
						near * d[y-1][x+1] +
						near * d[y][x] +
						near * d[y][x+1] +
						near * d[y][x] +
						near * d[y][x+1] +
						near * d[y-1][x] +
						near * d[y][x] +
						itself * d[y][x];
				} else if (x==MSIZE-1) {
					z[y][x] = 	near * d[y-1][x-1] +
						near * d[y-1][x] +
						near * d[y][x-1] +
						near * d[y][x] +
						near * d[y][x-1] +
						near * d[y][x] +
						near * d[y-1][x] +
						near * d[y][x] +
						itself * d[y][x];
				} else {
					z[y][x] = 	near * d[y-1][x-1] +
						near * d[y-1][x+1] +
						near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y-1][x] +
						near * d[y][x] +
						itself * d[y][x];
				}
			} else {
				if (x==0) {
					z[y][x] = 	near * d[y-1][x] +
						near * d[y-1][x+1] +
						near * d[y+1][x] +
						near * d[y+1][x+1] +
						near * d[y][x] +
						near * d[y][x+1] +
						near * d[y-1][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				} else if (x==MSIZE-1) {
					z[y][x] = 	near * d[y-1][x-1] +
						near * d[y-1][x] +
						near * d[y+1][x-1] +
						near * d[y+1][x] +
						near * d[y][x-1] +
						near * d[y][x] +
						near * d[y-1][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				} else {
					z[y][x] = 	near * d[y-1][x-1] +
						near * d[y-1][x+1] +
						near * d[y+1][x-1] +
						near * d[y+1][x+1] +
						near * d[y][x-1] +
						near * d[y][x+1] +
						near * d[y-1][x] +
						near * d[y+1][x] +
						itself * d[y][x];
				}
			}
			z[y][x]/=100;


		    }
		      
		 }
	     }
	}
    }
}
						
void Func4(int b[], int a[])
{
	int chuck_size=MSIZE;	 
	int array_size=VSIZE/chuck_size;
	int chuck[chuck_size];
    int i, j;
	
	for(j=0; j<chuck_size; j++) {
		b[j*array_size]=a[j*array_size];
		for (i=1; i<VSIZE/chuck_size; i++) {
			b[j*array_size+i]=b[j*array_size+i-1]+a[j*array_size+i];
		}
		chuck[j]=b[(j+1)*array_size-1];
	}
	
	for(j=1; j<chuck_size; j++) {
		chuck[j]=chuck[j-1]+chuck[j];
	}

	for(j=1; j<chuck_size; j++) {
		for (i=0; i<VSIZE/chuck_size; i++) {
			b[j*array_size+i]+=chuck[j-1]/(j+1);
		}
	}
}

void Func5(int b[], int a[])
{
	int i=0, j,  stride, stride2, step;
    int temp;
	long log_N=log2(VSIZE);

	for(j=0; j<VSIZE; j+=2) {
		b[j]=a[j];
		b[j+1] = a[j] + a[j+1];
	}
	
	for(i=4; i<VSIZE; i*=2) {
		for(j=0; j<VSIZE; j+=i) {
				b[j+i-1] = b[j+i/2-1] + b[j+i-1];
		}
	}
	
	b[VSIZE-1]=0;
	for(i=(log_N-1); i>=0; i--) {
		stride2=(2<<i)-1;
		stride=(1<<i)-1;
		step=stride2+1;
		for(j=0; j<VSIZE; j+=(int)pow(2, i+1)) {
                temp=b[j+(int)(pow(2, i))-1];
			b[j+(int)(pow(2, i))-1] = b[j+(int)(pow(2, i+1))-1];
			b[j+(int)(pow(2, i+1))-1] = temp+b[j+(int)(pow(2, i+1))-1];
		}
	}
}
