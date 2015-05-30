#include <stdio.h>
#include <stdlib.h>


int main(void)
{
  int i,j;
  int freq[2][255];
  coder_rle();
	decoder_rle();
  init_tableau(freq);

  for(i=0;i<2;i++)
    {
      for(j=0;j<10;j++)
	{
	  printf("%c ",freq[i][j]);
	}
      printf("\n");
    }
  return 0;
}
