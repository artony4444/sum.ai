
#include <iostream>
#include <vector>
using namespace std;

#include "tool/math.cpp"
#include "tool/get.cpp"

#include "model/neuron.cpp"
#include "model/model.cpp"

#include "model/cnn/cnn_neuron.cpp"
#include "model/cnn/cnn.cpp"
#include "model/lstm/lstm_cell.cpp"

#include "tool/helper.cpp"

#include <fstream>
#include <filesystem>

int main()
{
    lstm_cell c = *new lstm_cell();
    c.charge(10);
    c.fire();
    return 0;
}