// Fighting Fantasy combat analysis
// stochastic optimal control simulation for optimal Luck strategy at each state

#include <stdio.h>
#include <stdlib.h>

// define some array sizes, corresponding to the subset of state space we're exploring
#define TOPS 25
#define TOPL 13
#define MAXS (TOPS+3)
#define MAXL (TOPL+100)
#define MAXK 30

double pw = 0.5, pl = 0.5, pd = 0;

// the probability of a success luck test for a given luck
double q(double luck)
{
  int coeffs[] = {1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1};
  double probs[100];
  int norm = 36;
  double q;
  int i;

  q = 0;
  for(i = 0; i < 12; i++)
    probs[i] = (double) coeffs[i] / norm;

  for(i = 0; i <= luck-2; i++)
    q += probs[i];

  return q;
}

// referencing function used to map a set of four params to an index for a 1D array
int sref(int c1, int c2, int c3, int c4)
{
  return (c1+3)*MAXS*MAXL*2 + (c2+3)*MAXL*2 + (c3+100)*2 + c4;
}

int main(void)
{
  int shero, sopp, luck;
  int outcome;
  double *vprob;
  int *strategy;
  int change;
  double py, pn;
  int round;
  FILE *fp;
  int coeffs[] = {1, 4, 10, 20, 35, 56, 80, 104, 125, 140, 146, 140, 125, 104, 80, 56, 35, 20, 10, 4, 1};
  double probs[100];
  int norm = 1296;
  int kdiff;
  int i;
  double py1, py2, py3, py4, pn1, pn2;
  int *strats;
   
  // allocate memory for arrays of victory probability and optimal strategies through state space
  vprob = (double*)malloc(sizeof(double)*MAXL*MAXS*MAXS*2);
  strategy = (int*)malloc(sizeof(int)*MAXL*MAXS*MAXS*2);
  strats = (int*)malloc(sizeof(int)*MAXL*MAXS*MAXS*MAXK);
  
  fp = fopen("optimal-strategies.txt", "w");

  // loop through Skill differences -- perform optimal computation for each
  for(kdiff = -10; kdiff <= 10; kdiff++)
    {
      // calculate win/loss attack round probabilities for this Skill difference
      for(i = 0; i < 21; i++)
	probs[i] = (double) coeffs[i] / norm;

      pw = pl = pd = 0;
      for(i = -10; i <= 10; i++)
	{
	  if(i == kdiff) pd = probs[i+10];
	  else if(i > kdiff) pl += probs[i+10]; 
	  else pw += probs[i+10];
	}

      pl = pl/(1.-pd);
      pw = pw/(1.-pd);
  
      // state space is labelled with (sopp, shero, luck, outcome) where outcome is 0 for a lost attack round and 1 for a won attack round
      // sref(sopp, shero, luck, outcome) maps this tuple to a 1D array reference for ease of memory use
      // initialise victory probabilities at defined end points: hero win, opponent win, and undefined (-1) region of "both win" (should never be encountered)
      for(shero = -3; shero < TOPS; shero++)
	{
	  for(sopp = -3; sopp < TOPS; sopp++)
	    {
	      for(luck = -100; luck < TOPL; luck++)
		{
		  vprob[sref(sopp, shero, luck, 0)] = -1;
		  vprob[sref(sopp, shero, luck, 1)] = -1;
		}
	    }
	}
  
      for(shero = -3; shero <=0; shero++)
	{
	  for(sopp = -3; sopp < TOPS; sopp++)
	    {
	      for(luck = -100; luck < TOPL; luck++)
		{
		  vprob[sref(sopp, shero, luck, 0)] = 0;
		  vprob[sref(sopp, shero, luck, 1)] = 0;
		}
	    }
	}
      for(sopp = -3; sopp <= 0; sopp++)
	{
	  for(shero = -3; shero < TOPS; shero++)
	    {
	      for(luck = -100; luck < TOPL; luck++)
		{
		  vprob[sref(sopp, shero, luck, 0)] = 1;
		  vprob[sref(sopp, shero, luck, 1)] = 1;
		}
	    }
	}

      // algorithm proceeds as follows
      // while(first iteration, or change occurred in previous iteration)
      // search space for unprocessed states where victory probability can now be computed
      // compute victory probability for each of these states
      // end while
      change = 1;
      // while loop
      for(round = 1; change == 1; round++)
	{
	  printf("kdiff %i ==== Round %i\n", kdiff, round);
	  change = 0;
	  // search through states
	  for(shero = 1; shero < TOPS; shero++)
	    {
	      for(sopp = 1; sopp < TOPS; sopp++)
		{
		  for(luck = -100; luck < TOPL; luck++)
		    {
		      // first consider the loss outcome
		      // to compute vprob (sopp, shero, luck, 0) we need vprob in all possible subsequent states:
		      // (sopp, shero-3, luck-1, [0 or 1]) : used luck and picked up more damage, then [lost or won] next round
		      // (sopp, shero-2, luck, [0 or 1]) : didn't use luck, then [lost or won] next round
		      // (sopp, shero-1, luck-1, [0 or 1]) : used luck and mitigated damage, then [lost or won] next round
		      if(vprob[sref(sopp, shero, luck, 0)] == -1 && vprob[sref(sopp, shero-3, luck-1, 0)] != -1 && vprob[sref(sopp, shero-2, luck, 0)] != -1 && vprob[sref(sopp, shero-1, luck-1, 0)] != -1 && vprob[sref(sopp, shero-3, luck-1, 1)] != -1 && vprob[sref(sopp, shero-2, luck, 1)] != -1 && vprob[sref(sopp, shero-1, luck-1, 1)] != -1)
			{
			  change = 1;

			  // compute probabilities associated with all outcomes where luck was used
			  py1 = q(luck)*pl*vprob[sref(sopp, shero-1, luck-1, 0)];
			  py2 = (1.-q(luck))*pl*vprob[sref(sopp, shero-3, luck-1, 0)];
			  py3 = q(luck)*pw*vprob[sref(sopp, shero-1, luck-1, 1)];
			  py4 = (1.-q(luck))*pw*vprob[sref(sopp, shero-3, luck-1, 1)];
		      
			  // compute probabilities associated with all outcomes where luck wasn't used
			  pn1 = pl*vprob[sref(sopp, shero-2, luck, 0)];
			  pn2 = pw*vprob[sref(sopp, shero-2, luck, 1)];

			  py = py1+py2+py3+py4; pn = pn1+pn2;

			  // decide which is best i.e. whether to use luck or not
			  // and store that decision and corresponding (maximised) victory probability
			  if(py - pn > 1e-10 * pn)
			    {
			      strategy[sref(sopp, shero, luck, 0)] = 1;
			      vprob[sref(sopp, shero, luck, 0)] = py;
			    }
			  else
			    {
			      strategy[sref(sopp, shero, luck, 0)] = 0;
			      vprob[sref(sopp, shero, luck, 0)] = pn;
			    }

			  // output this calculation
			  if(luck > 0)
			    printf("At (%i,%i,%i,0) . Y00 -> (%i,%i,%i,0) %.3e ; Y01 -> (%i,%i,%i,1) %.3e ; Y10 -> (%i,%i,%i,0) %.3e ; Y11 -> (%i,%i,%i,1) %.3e . N0 -> (%i,%i,%i,0) %.3e ; N1 -> (%i,%i,%i,1) %.3e . %.3e vs %.3e (%.3e) : %c wins\n", sopp, shero, luck, sopp, shero-3, luck-1, py2, sopp, shero-3, luck-1, py4, sopp, shero-1, luck-1, py1, sopp, shero-1, luck-1, py3, sopp, shero-2, luck, pn1, sopp, shero-2, luck, pn2, py, pn, (py-pn), (py > pn ? 'Y' : 'N'));
			}

		      // now consider the win outcome
		      // to compute vprob (sopp, shero, luck, 1) we need vprob all possible subsequent states:
		      // (sopp-4, shero, luck-1, [0 or 1]) : used luck and dealt more damage, then [lost or won] next round
		      // (sopp-2, shero, luck, [0 or 1]) : didn't use luck, then [lost or won] next round
		      // (sopp-1, shero, luck-1, [0 or 1]) : used luck and dealt less damage, then [lost or won] next round
		      if(vprob[sref(sopp, shero, luck, 1)] == -1 && vprob[sref(sopp-4, shero, luck-1, 0)] != -1 && vprob[sref(sopp-2, shero, luck, 0)] != -1 && vprob[sref(sopp-1, shero, luck-1, 0)] != -1 && vprob[sref(sopp-4, shero, luck-1, 1)] != -1 && vprob[sref(sopp-2, shero, luck, 1)] != -1 && vprob[sref(sopp-1, shero, luck-1, 1)] != -1)
			{
			  change = 1;
			  // compute probabilities associated with all outcomes where luck was used
			  py1 = q(luck)*pl*vprob[sref(sopp-4, shero, luck-1, 0)];
			  py2 = (1.-q(luck))*pl*vprob[sref(sopp-1, shero, luck-1, 0)];
			  py3 = q(luck)*pw*vprob[sref(sopp-4, shero, luck-1, 1)];
			  py4 = (1.-q(luck))*pw*vprob[sref(sopp-1, shero, luck-1, 1)];

			  // compute probabilities associated with all outcomes where luck wasn't used
			  pn1 = pl*vprob[sref(sopp-2, shero, luck, 0)];
			  pn2 = pw*vprob[sref(sopp-2, shero, luck, 1)];

			  // decide which is best i.e. whether to use luck or not
			  // and store that decision and corresponding (maximised) victory probability
			  py = py1+py2+py3+py4; pn = pn1+pn2;
			  if(py - pn > 1e-10 * pn)
			    {
			      strategy[sref(sopp, shero, luck, 1)] = 1;
			      vprob[sref(sopp, shero, luck, 1)] = py;
			    }
			  else
			    {
			      strategy[sref(sopp, shero, luck, 1)] = 0;
			      vprob[sref(sopp, shero, luck, 1)] = pn;
			    }

			  // output this calculation
			  if(luck > 0)
			    printf("At (%i,%i,%i,1) . Y00 -> (%i,%i,%i,0) %.3e ; Y01 -> (%i,%i,%i,1) %.3e ; Y10 -> (%i,%i,%i,0) %.3e ; Y11 -> (%i,%i,%i,1) %.3e . N0 -> (%i,%i,%i,0) %.3e ; N1 -> (%i,%i,%i,1) %.3e . %.3e vs %.3e (%.3e) : %c wins\n", sopp, shero, luck, sopp-1, shero, luck-1, py2, sopp-1, shero, luck-1, py4, sopp-4, shero, luck-1, py1, sopp-4, shero, luck-1, py3, sopp-2, shero, luck, pn1, sopp-2, shero, luck, pn2, py, pn, (py-pn), (py > pn ? 'Y' : 'N'));
			}
		    }
		}
	    }
	}
  
      // output probability and best strategy for each state
      for(luck = 0; luck < TOPL; luck++)
	{
	  printf("\nLUCK = %i\n\n", luck);
	  printf("---> sopp\n|\nv shero\n\n\n");
	  for(shero = 1; shero < TOPS; shero++)
	    {
	      for(sopp = 1; sopp < TOPS; sopp++)
		{
		  // this records a 2-bit description of the best win+loss strategy at this (so, sh, l) point
		  strats[(kdiff+10)*MAXL*MAXS*MAXS+luck*MAXS*MAXS+sopp*MAXS+shero] = strategy[sref(sopp, shero, luck, 0)]*2 + strategy[sref(sopp, shero, luck, 1)];
	      	      
		  printf("(%.2f %c) ", pl*vprob[sref(sopp, shero, luck, 0)]+pw*vprob[sref(sopp, shero, luck, 1)], ('A' + strategy[sref(sopp, shero, luck, 0)]*2 + strategy[sref(sopp, shero, luck, 1)]));
		  fprintf(fp, "%i %i %i %i %e %i\n", kdiff, luck, shero, sopp, pl*vprob[sref(sopp, shero, luck, 0)]+pw*vprob[sref(sopp, shero, luck, 1)], strategy[sref(sopp, shero, luck, 0)]*2 + strategy[sref(sopp, shero, luck, 1)]);
		}
	      printf("\n");
	    }
	}
    }
     
  fclose(fp);

  // this section records similarities in optimal strategy structure at different parts of state space

  int ref1, ref2, luck1, luck2, kdiff1, kdiff2;
  int diffs;
 
  fp = fopen("similarities.txt", "w");

  for(ref1 = 0; ref1 <= 20*TOPL; ref1++)
    {
      for(ref2 = ref1+1; ref2 <= 20*TOPL; ref2++)
	{
	  luck1 = ref1 % TOPL;
	  luck2 = ref2 % TOPL;
	  kdiff1 = (ref1-luck1) / TOPL - 10;
	  kdiff2 = (ref2-luck2) / TOPL - 10;

	  diffs = 0;
	  for(sopp = 0; sopp < TOPS; sopp++)
	    {
	      for(shero = 0; shero < TOPS; shero++)
		{
		  if(strats[(kdiff1+10)*MAXL*MAXS*MAXS+luck1*MAXS*MAXS+sopp*MAXS+shero] != strats[(kdiff2+10)*MAXL*MAXS*MAXS+luck2*MAXS*MAXS+sopp*MAXS+shero])
		    diffs++;
		}
	    }
	  fprintf(fp, "%i %i %i %i %i\n", kdiff1, luck1, kdiff2, luck2, diffs);
	}
    }
  fclose(fp);
 
  return 0;
}
