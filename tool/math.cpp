// ----- random ----- //

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

// ----- functions ----- //

class functions
{
    public:
    
    static void sigmoid()
    {
        cout << "hello i am sigmoid." << endl;
    }
};

// ----- helper ----- //

vector<int> vectorize(int e) { return *new vector<int>{e}; }