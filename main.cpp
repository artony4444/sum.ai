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
    for(float elem : output) { cout << elem << ", "; }
    // n.fire();
}


/*

neuron n1;
    neuron n2;
    
    cout << "neuron1 " << n1.bias << endl;
    cout << "neuron2 " << n2.bias << endl;
    path p = *new path(&n1, &n2);
    
    cout << "path " << p.weight << endl;
    cout << "p n1 " << p.from->bias << endl;
    cout << "p n1 " << p.to->bias << endl;
    
    cout << endl;
    
    n1.charge(10.0);
    cout << n1.charges << endl;
    p.fire(n1.fire());
    cout << n2.charges << endl;



*/
