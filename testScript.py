#Testing Script for CS475 Project1
#Jesse Thoren
#Description: Compiles/runs project1.cpp with a variety of thread quantities
#   (NUMT) and number of subdivisions (NUMNODES).

import os

for NUMT in [1, 2, 4, 6, 8]:
    for NUMNODES in [16, 24, 32, 64, 128, 256, 512, 1024, 2048]:
        cmd = "g++ -DNUMT=%d -DNUMNODES=%d -DNUMTRIES=100 project1.cpp -o runProg -O3 -lm -fopenmp" % (NUMT, NUMNODES)
        os.system( cmd )
        cmd = "./runProg"
        os.system( cmd )
cmd = "rm -f runProg"
os.system( cmd )
