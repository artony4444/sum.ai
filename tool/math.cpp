

// ----- Functions ----- //

#include <stdlib.h> // rand()
#include <time.h>   // time(NULL)

void seedRandom()
{
    srand(time(NULL));
}

float random_float(int depth = 1000)
{
    return (double) (rand() % depth) / depth;
}