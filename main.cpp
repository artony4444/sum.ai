#include <iostream>
#include <vector>
using namespace std;

#include "tool/math.cpp"
#include "tool/get.cpp"
#include "model/neuron.cpp"
#include "model/model.cpp"

int main()
{
    seedRandom();
    
    int size = 10;
    
    model m(size, *new vector<int>{size}, size);
    vector<float> output = m.input(*new vector<float>(size, 1));
    for(float elem : output) { cout << elem << endl; }
    
}