// The code to evaluate the height at a given <i>iu</i> and <i>iv</i> is:

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <omp.h>

using namespace std;

// Change later
#ifndef NUMNODES
#define NUMNODES    1000
#endif

#ifndef NUMT
#define NUMT        4
#endif

#define NUMTRIES    10

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

// const float N = 2.5f;
// const float R = 1.2f;
const float N = 2.0f;
const float R = 1.0f;

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float rn = pow( fabs(R), (double)N );
	float r = rn - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(double)N );
	return height;
}

// float Height( int, int );	// function prototype

int main( int argc, char *argv[ ] )
{
    // cout << "main function " << endl;
    
    omp_set_num_threads(NUMT);
    float volume;
    double maxPerformance = 0;

    for (int times = 0; times < NUMTRIES; times++) 
    {
        volume = 0;    
        double time0 = omp_get_wtime();

        #pragma omp parallel for collapse(2) default(none) shared(cout), reduction(+:volume)
        for( int iv = 0; iv < NUMNODES; iv++ )
            {
            for( int iu = 0; iu < NUMNODES; iu++ )
            {
                float z = Height( iu, iv );
                float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
                ( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );
                if (iu == 0 || iv == 0 || iu == NUMNODES - 1 || iv == NUMNODES-1) 
                {
                    // z = Height( iu, iv );
                    if (iu == 0 && iv == 0) {
                        volume += z * fullTileArea * 0.25;
                    }
                    if (iu == 0 && iv == NUMNODES - 1)
                    {
                        volume += z * fullTileArea * 0.25;
                    }
                    if (iu == NUMNODES - 1 && iv == 0)
                    {
                        volume += z * fullTileArea * 0.25;
                    }
                    if (iu == NUMNODES - 1 && iv == NUMNODES - 1)
                    {
                        volume += z * fullTileArea * 0.25;
                    }
                    else {
                        volume += z * fullTileArea * 0.5;
                    }
                }

                else 
                {
                    volume += z * fullTileArea;
                }
                // cout << volume << "\n";
            }
        }

        double time1 = omp_get_wtime( );
        double megaTrialsPerSecond = (double)NUMNODES*NUMNODES / ( time1 - time0 ) / 1000000.;
        // cout << (time1 - time0) << endl;
        // cout << megaTrialsPerSecond << endl;
        if( megaTrialsPerSecond > maxPerformance )
                maxPerformance = megaTrialsPerSecond;
    }

    float double_volume = 2 * volume;
    // cout << "volume: " << double_volume << " " << volume << endl;
    //fprintf(stderr, "%2d, %8d, %6.2lf\n",
      //          NUMT, NUMNODES, maxPerformance);
    fprintf(stderr, "%2d threads : %8d trials ; volume = %6.4f ; megatrials/sec = %6.2lf\n",
             NUMT, NUMNODES, double_volume, maxPerformance);

    // cout << double_volume;

}
