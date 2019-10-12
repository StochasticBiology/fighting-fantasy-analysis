// Fighting Fantasy combat analysis
// simulation of combat outcomes with no Luck-based strategy
// outputs a table of victory probabilities for different Skill differences (k) and hero, opponent Staminas (sh, so)

#include <stdio.h>
#include <stdlib.h>

#define RND drand48()

int main(void)
{
  int k;
  int sh, so;
  int i;
  int tsh, tso;
  int ash, aso;
  double vic;
  FILE *fp;
  
  fp = fopen("simscan.txt", "w");
  // loop through Skill difference
  for(k = -10; k <= 10; k++)
    {
      // loop through hero Stamina
      for(sh = 1; sh <= 30; sh++)
	{
	  // loop through opponent Stamina
	  for(so = 1; so <= 30; so++)
	    {
	      vic = 0;
	      // 1000 iterations for each parameterisation
	      for(i = 0; i < 1000; i++)
		{
		  // initial conditions
		  tsh = sh; tso = so;
		  // loop through attack rounds and record if victory occurs
		  for(;;)
		    {
		      if(tsh <= 0 || tso <= 0) break;
		      ash = (int)(RND*6)+(int)(RND*6)+2 + k;
		      aso = (int)(RND*6)+(int)(RND*6)+2;
		      if(ash > aso) tso -= 2;
		      else if(aso > ash) tsh -= 2;
		    }
		  if(tsh > 0) vic++;
		}
	      fprintf(fp, "%i %i %i %f\n", k, sh, so, vic/1000);
	    }
	  fprintf(fp, "\n");
	}
    }
  fclose(fp);
  

  return 0;
}
		  
