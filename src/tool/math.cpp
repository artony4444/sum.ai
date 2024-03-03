

// ----- random ----- //

#include <stdlib.h> // rand()
#include <time.h>   // time(NULL)
#include <cmath>    // exp()

void seedRandom()
{
    srand(time(NULL));
}

float random_float(int depth = 1000)
{
    return (double) (rand() % depth) / depth;
}

float roundm(float n, float multiple = 0.01)
{
    return (int)(n * 1/multiple) / (float)(1/multiple);
}

int summ(vector<int> s)
{
    int sum = 0;
    for(int num : s) { sum += num; }
    return sum;
}
float summ(vector<float> s)
{
    float sum = 0;
    for(float num : s) { sum += num; }
    return sum;
}

int multt(vector<int> s)
{
    int sum = 1;
    for(int num : s) { sum *= num; }
    return sum;
}

vector<float> divv(vector<float> arr, float by)
{
    vector<float> divide;
    for(float num : arr) { divide.push_back(num / by); }
    return divide;
}

// ----- functions ----- //

class functions
{
    public:
    
    static double step(double x) {
        return x > 0 ? 1 : 0;
    }
    
    static double sigmoid(double x) {
        return 1 / (1 + exp(-x));
    }
    
    static double relu(double x) {
        return x > 0 ? x : 0;
    }
    
    static double leakyRelu(double x, double alpha = 0.01) {
        return x > 0 ? x : alpha * x;
    }
    
    static double tanh(double x) {
        return tanh(x);
    }
    
    static double d_tanh(double x) {
        return 1 - (tanh(x) * tanh(x));
    }
    
    static vector<float> softmax(vector<float> input)
    {
        vector<float> output;
        float sum = 0;
        for(float i : input) { sum += exp(i); }
        for(float i : input) { output.push_back(exp(i) / sum); }
        return output;
    }
    
    static float softmaxF(vector<float> input)
    {
        float sum = 0;
        for(float i : input) { sum += exp(i); }
        return sum / exp(input.size());
    }
};