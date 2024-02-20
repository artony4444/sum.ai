

// ----- random ----- //

#include <stdlib.h> // rand()
#include <time.h>   // time(NULL)
//#include <cmath>    // exp()

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
    
    
    static vector<float> softmax(vector<float> input)
    {
        vector<float> output;
        float sum = 0;
        for(float i : input) { sum += exp(i); }
        for(float i : input) { output.push_back(exp(i) / sum); }
        return output;
    }
};

