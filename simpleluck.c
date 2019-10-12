// Fighting Fantasy combat analysis
// simulation of combat outcomes with simple Luck-based strategy (Luck never depletes, and is used on every successful attack round)
// outputs a table of victory probabilities for different Skill differences (kdiff) and hero, opponent Staminas (shero, sopp)
// these victory probabilities are, for theoretical interest, further classified by the final successful round -- the sum of these classes gives overall victory probability

#include <stdio.h>
#include <stdlib.h>

// define random number and die roll macros
#define RND drand48()
#define DIE (1+(int)(RND*6))

int main(void)
{
  int shero, sopp;
  int kdiff;
  double outcome[7];
  int i;
  int sh, so;
  int ah, ao;
  int lastso;
  int l = 12 ;
  int r;
  int nit = 10000;
  FILE *fp;

  fp = fopen("simpleluck.txt", "w");
  // loop through Skill differences
  for(kdiff = -5; kdiff <= 5; kdiff++)
    {
      // loop through hero Stamina
      for(shero = 1; shero <= 24; shero++)
	{
	  // loop through opponent Stamina
	  for(sopp = 1; sopp <= 24; sopp++)
	    {
	      printf("%i %i %i\n", kdiff, shero, sopp);
	      // we label the following outcomes based on the last change to opponent stamina:
	      // 0: 4->0 ; 1: 3->-1; 2: 2->-2; 3: 1->-3; 4: 1->0
	      // the sum of these gives overall victory probability
	      for(i = 0; i <= 6; i++)
		outcome[i] = 0;

	      // loop through simulated iterations
	      for(i = 0; i < nit; i++)
		{
		  // initial conditions
		  sh = shero; so = sopp;
		  // simulate attack rounds
		  for(;;)
		    {
		      ah = DIE+DIE+kdiff;
		      ao = DIE+DIE;
		      lastso = so;
		  
		      if(ao > ah) sh -= 2;
		      else if(ah > ao)
			{
			  // employ luck
			  r = DIE+DIE;
			  if(r <= l) so -= 4;
			  else so -= 1;
			}
		      if(so <= 0 || sh <= 0) break;
		    }
		  // categorise successful outcomes
		  if(sh > so)
		    {
		      if(lastso == 4) outcome[0]++;
		      if(lastso == 3) outcome[1]++;
		      if(lastso == 2 && so == -2) outcome[2]++;
		      if(lastso == 1 && so == -3) outcome[3]++;
		      if(lastso == 1 && so == 0) outcome[4]++;
		    }
		}
	      fprintf(fp, "%i %i %i ", kdiff, shero, sopp);
	      for(i = 0; i <= 4; i++)
		{
		  fprintf(fp, "%f ", outcome[i]/nit);
		}
	      fprintf(fp, "\n");
	    }
	}
    }
  fclose(fp);
  
  return 0;
}
		  
