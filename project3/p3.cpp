#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

unsigned int seed = 0;

int	NowYear = 2022;		    // 2022 - 2027
int	NowMonth = 0;		    // 0 - 11

// float	NowPrecip;		// inches of rain per month
// float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		    // number of deer in the current population
int NowHunters;

const float GRAIN_GROWS_PER_MONTH =	    15.0;
const float ONE_DEER_EATS_PER_MONTH =   0.75;

const float AVG_PRECIP_PER_MONTH =		8.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			    2.0;	// plus or minus noise

const float AVG_TEMP =				    60.0;	// average
const float AMP_TEMP =				    20.0;	// plus or minus
const float RANDOM_TEMP =			    10.0;	// plus or minus noise

const float MIDTEMP =				    40.0;
const float MIDPRECIP =				    10.0;



float
SQR( float x )
{
        return x*x;
}


float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX
        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;
        return (int)(  Ranf(seedp, low,high) );
}

float x = Ranf( &seed, -1.f, 1.f );

float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
float temp = AVG_TEMP - AMP_TEMP * cos( ang );
float NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
float NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );

float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

void Deer() {
	// cout << "deer" << endl;
	while( NowMonth <= 72 )
	{
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		int nextNumDeer = NowNumDeer;
		int carryingCapacity = (int)( NowHeight );
		// cout << nextNumDeer << " " << carryingCapacity << "\n";
		if( nextNumDeer < carryingCapacity ) {
			nextNumDeer++;
			// cout << "DEER INCREASE " << nextNumDeer << endl;
		}

		else if( nextNumDeer > carryingCapacity ) {
			nextNumDeer--;
			// cout << "DEER DECREASE " << nextNumDeer << endl;
		}

		// Variable is hunters; each hunter will try to kill one deer per month
		int Hunters = NowHunters;
		// Hunters can only hunt as long as there are enough deer
		if (NowNumDeer >= 5)
			nextNumDeer -= Hunters;

		if( nextNumDeer < 0 )
			nextNumDeer = 0;

		// DoneComputing barrier:
		#pragma omp barrier
		NowNumDeer = nextNumDeer;

		// DoneAssigning barrier:
		#pragma omp barrier

		// DonePrinting barrier:
		#pragma omp barrier

	}
}

void Grain() {
	// cout << "grain" << endl;
	while( NowMonth <=72 )
	{
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation
		// float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
		// float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

		// cout << tempFactor << ", " << precipFactor << ", " << NowHeight << endl;

		float nextHeight = NowHeight;
 		nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 		nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
		if( nextHeight < 0. ) nextHeight = 0.; 
		
		// DoneComputing barrier:
		#pragma omp barrier
		NowHeight = nextHeight;
		// cout << NowHeight << endl;

		// DoneAssigning barrier:
		#pragma omp barrier
		// Nothing
		// DonePrinting barrier:
		#pragma omp barrier
	}
}


void Watcher() {
	while( NowMonth <= 72 )
	{

	// DoneComputing barrier:
	#pragma omp barrier

	// DoneAssigning barrier:
	#pragma omp barrier

	NowMonth++;
	// if (NowMonth % 12 == 0) {
	// 	NowMonth = 0;
	// 	NowYear++;
	// }

	tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
	precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

	ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
	temp = AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
	precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
		NowPrecip = 0.;

	// cout << "HEIGHT " << NowHeight;
	// fprintf(stderr, "height: %f\n", NowHeight);
	// cout << NowMonth << " PRECIP: " << NowPrecip << " TEMP: " << NowTemp << " HEIGHT: " << NowHeight << "  DEER: " << NowNumDeer << " HUNTERS: " << NowHunters << endl;
	cout << NowMonth << ", " << NowPrecip << ", " << NowTemp << ", " << NowHeight << ", " << NowNumDeer << ", " << NowHunters << endl;

	// printf("Month: %2d, Precip: %8.2f, NowTemp: %8.2f\n",
    //          NowMonth, NowPrecip, NowTemp);

	// DonePrinting barrier:
	#pragma omp barrier
	}
}


void Hunters() {
	// cout << "agent" << endl;
	while( NowMonth <= 72 ) 
	{
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
	int newNumHunters = NowHunters;
	int availableDeer = NowNumDeer - 5;

	if (newNumHunters < (availableDeer))
		newNumHunters++;
	else 
		newNumHunters = 0;

	// DoneComputing barrier:
	#pragma omp barrier
	NowHunters = newNumHunters;

	// DoneAssigning barrier:
	#pragma omp barrier

	// DonePrinting barrier:
	#pragma omp barrier
	
	}
}


int main(int argc, char *argv[]) {

	cout << "inside main function" << endl;
	
	cout << "PRECIP  " << NowPrecip << endl;
	if ( NowPrecip < 0. )
		NowPrecip = 0.;

	// NowNumDeer = 5;
	// NowHeight = 30;

	cout << "DEER  " << NowNumDeer << endl;
	cout << "GRAIN  " << NowHeight << endl;
	cout << "HUNTERS  " << NowHunters << endl;

	// // DEER SECTION
	// int nextNumDeer = NowNumDeer;
	// int carryingCapacity = (int)( NowHeight );
	// if( nextNumDeer < carryingCapacity )
	// 	nextNumDeer++;
	// else
	// 	if( nextNumDeer > carryingCapacity )
	// 		nextNumDeer--;

	// if( nextNumDeer < 0 )
	// 	nextNumDeer = 0;

	omp_set_num_threads( 4 );	// same as # of sections
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			Deer( );	
		}

		#pragma omp section
		{
			Grain( );
		}

		#pragma omp section
		{
			Watcher( );
		}

		#pragma omp section
		{
			Hunters( );	// your own
		}

	}       // implied barrier -- all functions must return in order
		// to allow any of them to get past here


}