

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
    
    // training 
    
    int size = 10;
    
    model m(size, {size}, size);
    
    int fail = 0;
    for(int a = 0; a < 100; a++)
    {
        int in = random_float() * size;
        int out = m.train(in, in);
        bool pass = (in == out); if(!pass) fail++;
        cout << in << " > " << out << (pass ? " PASS" : " ") << endl;
    }
    cout << endl << "fail count :" << fail << endl;
}