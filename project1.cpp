/****
 * CS 475 Project 1
 * Author: Jesse Thoren
 * Date: April 10, 2017
 * Description: A project to compute the volume between 2 bezier surfaces.
 *    This leverages an algortihm similar to the trapezoid rule, but in 2D.
 * References: web.engr.oregonstate.edu/~mjb/cs575/Projects/proj01.html
 ****/

#include <omp.h>
#include <stdio.h>
#include <math.h>

//Run by default with 4 threads unless script overrides NUMT
#ifndef NUMT
#define NUMT    4
#endif

//Run by default with 8 subdivisions unless scrip overrides NUMNODES
#ifndef NUMNODES
#define NUMNODES    8
#endif

//Loop 10 times by default to compute an average run unless script overrides
#ifndef NUMTRIES
#define NUMTRIES    10
#endif

#include "heights.cpp"

float Height( int, int );

int main()
{
    //Confirm OpenMP is supported by current machine
    #ifndef _OPENMP
        fprintf(stderr, "OpenMP is not supported on this machine. \n");
        return 1;
    #endif

    //Inform user if NUMT exceeds omp_get_max_threads();
    int maxNUMT = omp_get_max_threads();
    if(NUMT > maxNUMT)
    {
        fprintf(stderr, "Caution: NUMT specified exceeds omp_get_max_threads()\n");
        fprintf(stderr, "   NUMT: %d\nmaxNUMT: %d\n", NUMT, maxNUMT);
        return 1;
    }

    float totalVolume = 0;

    //Set max threads to run with OpenMP
    omp_set_num_threads(NUMT);
    fprintf(stderr, "\nRunning with NUMNODES = %d\n", NUMNODES);
    fprintf(stderr, "Using %d threads in parallel\n", NUMT);
    fprintf(stderr, "Looping %d times to compute average performance\n", NUMTRIES);

    double maxMegaHeights = 0.;
    double sumMegaHeights = 0.;

    //Loop the number of times to compute an average run
    for(int i = 0; i < NUMTRIES; i++)
    {
        totalVolume = 0;

        double time0 = omp_get_wtime();

        //Compute volume
        #pragma omp parallel for reduction(+:totalVolume) default(none)
        for(int i = 0; i < NUMNODES*NUMNODES; i++)
        {
            int iu = i % NUMNODES;
            int iv = i / NUMNODES;

            float fullTileArea = (((XMAX-XMIN)/(float)(NUMNODES-1)) * ((YMAX - YMIN)/(float)(NUMNODES-1)));
        
            //Half total area for iu edge cases
            if(iu == 0 || iu == NUMNODES - 1)
                fullTileArea /= 2;
            if(iv == 0 || iv == NUMNODES - 1)
                fullTileArea /= 2;

            float tileHeight = Height(iu, iv);
            float tileVolume = tileHeight * fullTileArea;

            totalVolume += tileVolume;
        }

        double time1 = omp_get_wtime();
        double megaHeights = (double)NUMNODES*NUMNODES/(time1-time0)/1000000.;
        sumMegaHeights += megaHeights;
        if(megaHeights > maxMegaHeights)
            maxMegaHeights = megaHeights;
        //Enable following line to verify that Volume is consistent.
        //fprintf(stderr, "Total Volume: %g\n", totalVolume);
    }
    double avgMegaHeights = sumMegaHeights/(double)NUMTRIES;
    fprintf(stderr, "       Total Volume = %g units\n", totalVolume);
    fprintf(stderr, "   Peak Performance = %8.21f MegaHeights/Sec\n", maxMegaHeights);
    fprintf(stderr, "Average Performance = %8.21f MegaHeights/Sec\n", avgMegaHeights);
    return 0;
}
