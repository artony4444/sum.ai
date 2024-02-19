

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
    
    model m(size, {size}, size);
    
    for(int a = 0; a < 10; a++)
    {
        int in = random_float() * size;
        int out = m.train(in, in);
        cout << in << " > " << out << endl;
    }
}