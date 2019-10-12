// Fighting Fantasy combat analysis
// simulation of combat outcomes with heuristic Luck-based strategies
// outputs statistics of victory probability for each heuristic strategy, and the best performing one, for each Skill difference, Stamina state, and Luck

#include <stdio.h>
#include <stdlib.h>

// define random number and die roll macros
#define RND drand48()
#define DIE (1+(int)(RND*6))

// function returning success or failure of a Luck test within combat
// Luck l is depleted when used
// circ[umstance] describes outcome of this attack round (1 or -1); tsh, tso current Stamina state, strategy our heuristic strategy, and thresh[old] a value below which Luck is not used
// strategies: if Luck is <= thresh don't use it, otherwise:
// 0 never use; 1 always use; 2 use on loss; 3 use on win; 4 use on loss when weak; 5 use on win when opponent strong; 6 use when weak; 7 use when v weak; 8 use when weaker than opponent 
int luck(int circ, int *l, int tsh, int tso, int strategy, int thresh)
{
  if(*l > thresh)
    {
      switch(strategy)
	{
	case 0: return 0;
	case 1: (*l)--; return ((DIE+DIE) <= (*l+1) ? 1 : -1);
	case 2: if(circ == -1) { (*l)--; return ((DIE+DIE) <= (*l+1) ? 1 : -1); } return 0;
	case 3: if(circ == 1) { (*l)--; return ((DIE+DIE) <= (*l+1) ? 1 : -1); } return 0;
	case 4: if(circ == -1 && tsh < 6) { (*l)--; return ((DIE+DIE) <= (*l+1) ? 1 : -1); } return 0;
	case 5: if(circ == 1 && tso > 6) { (*l)--; return ((DIE+DIE) <= (*l+1) ? 1 : -1); } return 0;
	case 6: if(tsh < 6) { (*l)--; return ((DIE+DIE) <= (*l+1) ? 1 : -1); } return 0;
	case 7: if(tsh < 4) { (*l)--; return ((DIE+DIE) <= (*l+1) ? 1 : -1); } return 0;
	case 8: if(tso > tsh) { (*l)--; return ((DIE+DIE) <= (*l+1) ? 1 : -1); } return 0;
	}
    }
  return 0;
}
	  
int main(void)
{
  int khero, kopp;
  int shero, sopp;
  int lhero;
  int tsh, tso, ash, aso, tl;
  int victory = 0;
  int i, t;
  int strategy;
  int l;
  int thresh;
  int expt;
  double best = 0, bestthis = 0;
  int bests, bestt;
  double bare;
  FILE *fp;
  int d;
  double outcome[9];
  int mylim = 1e4;
  
  srand48(190);

  fp = fopen("heuristic.txt", "w"); fclose(fp);

  // loop through Skill difference
  for(d = -10; d <= 10; d++)
    {
      kopp = 10;
      khero = kopp+d;
      // loop through hero Stamina
      for(shero = 1; shero <= 24; shero++)
	{
	  // loop through opponent Stamina
	  for(sopp = 1; sopp <= 24; sopp++)
	    {
	      // loop through initial hero Luck
	      for(lhero = 0; lhero <= 13; lhero++)
		{
		  // loop through strategies
		  for(strategy = 0; strategy <= 8; strategy++)
		    {
		      bestthis = 0;

		      // loop through Luck thresholds
		      for(thresh = 12; thresh > 0; thresh--)
			{
			  victory = 0;
			  // loop through combat iterations
			  for(i = 0; i < mylim; i++)
			    {
			      // initial conditions
			      tsh = shero; tso = sopp; tl = lhero;
			      // simulate attack rounds, applying strategy according to each outcome
			      for(t = 0; t < 1000; t++)
				{
				  ash = DIE+DIE+khero;
				  aso = DIE+DIE+kopp;
				  if(ash > aso)
				    {
				      l = luck(1, &tl, tsh, tso, strategy, thresh);
				      if(l == 1) tso -= 4;
				      if(l == -1) tso -= 1;
				      if(l == 0) tso -= 2;
				    }
				  if(ash < aso)
				    {
				      l = luck(-1, &tl, tsh, tso, strategy, thresh);
				      if(l == 1) tsh -= 1;
				      if(l == -1) tsh -= 3;
				      if(l == 0) tsh -= 2;
				    }
				  if(tsh <= 0 || tso <= 0) break;
				}
			      if(tso <= 0 && tsh > 0) victory++;
			    }
			  // if strategy avoids Luck, record stats as the "null" case
			  if(strategy == 0)
			    {
			      bare = (double)victory/mylim;
			      best = bare;
			      bests = 0;
			      bestt = 0;
			    }
			  // otherwise if this is the best strategy so far for these parameters
			  else if((double)victory/mylim > best)
			    {
			      best = (double)victory/mylim;
			      bests = strategy;
			      bestt = thresh;
			    }
			  if((double)victory/mylim > bestthis)
			    bestthis = (double)victory/mylim;
			}
		      // record outcome for this strategy
		      outcome[strategy] = bestthis;
		      if(strategy == 0) outcome[strategy] = bare;
		    }
		  // output
		  fp = fopen("heuristic.txt", "a"); 
		  fprintf(fp, "%i %i %i %i ", d, shero, sopp, lhero);
		  for(strategy = 0; strategy <= 8; strategy++) 
		    fprintf(fp, "%f ", outcome[strategy]);
		  fprintf(fp,"%f %i %i\n", best, bests, bestt);
		  fclose(fp);
		  printf("%i %i %i %i %f %f %i %i\n", d, shero, sopp, lhero, bare, best, bests, bestt);
		}
	      fp = fopen("heuristic.txt", "a"); 
	      fprintf(fp, "\n");
	      fclose(fp);
	    }
	}
    }

  return 0;
}
