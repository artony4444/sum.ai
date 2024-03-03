

#include <iostream>
#include <vector>
using namespace std;

#include "tool/math.cpp"
#include "tool/get.cpp"
#include "model/neuron.cpp"
#include "model/model.cpp"

#include "model/cnn/cnn_neuron.cpp"
#include "model/cnn/cnn.cpp"
#include "tool/helper.cpp"

#include <fstream>

struct dataset {
    vector<vector<float>> inputs; // 28x28 = 784
    vector<int> expected;
};

dataset getData(string type = "test") // test - train
{
    // GET CSV ---
    
    string path = "/storage/emulated/0/my files/backups/files/codes/c++/ai/";
    ifstream file(path+"dataset/"+type+".csv");
    
    string line;
    getline(file, line);
    
    vector<vector<float>> inputs; // 28x28 = 784
    vector<int> expected;
    
    for(int a = 0; a < 1000 && getline(file, line); a++)
    {
        vector<string> str = split(line, ",");
        vector<float> train = toFloats(str);
        
        expected.push_back(train[0]);
        train.erase(train.begin());
        
        train = divv(train, 255);
        inputs.push_back(train);
    }
    
    file.close();
    
    return { inputs, expected };
}


int main()
{
    seedRandom();
    
    dataset dst = getData("train");
    
    vector<vector<float>> inputs = dst.inputs;
    vector<int> expected = dst.expected;
    
    // training
    
    vector<int> imgSize = {28, 28};
    vector<int> cnnSize = {24, 24};
    vector<int> mpSize = {20, 20};
    
    int iSize = 784;
    vector<int> hSize = {15};
    int oSize = 10;
    
    vars::biasPlasticity = 0.1;
    vars::plasticity = 0.03;
    vars::batchSize = 1;
    
    cnn cnn(imgSize, cnnSize, mpSize);
    model m(iSize, hSize, oSize);
    
    // cnn cnnTest({2,2}, {2,2}, {2,2});
    // cnnTest.input({1,2,3,4});
    
    int fail = 0; int count = 0;
    
    for(int x = 0; x < 1; x++)
    {
        for(int y = 0; y < 1; y++)
        {
            for(int i = 0; i < inputs.size(); i++)
            {
                int e = expected[i];
                int out = m.train(inputs[i], e); // 
                // int out = m.train(cnn.input(inputs[i]), e); cnn.backpropogate(m.getLastLoss());
                bool pass = (out == e); count++; if(!pass) fail++;
                cout << x << y << " " << (int)((1-((float)fail/count))*100) << "% "; cout << i << " (" << e << " > " << out << ")" << (pass ? " PASS" : " ") << endl;
                // printInput(inputs[i]);
                // printInput(getWsum(m));
                // printInput(m.getLastLoss());
                // printInput(getWsum(cnn));
                // if(i % 1000 == 0) { fail = 0; count = 0; }
            }
        }
    }
    
    // test
    
    dst = getData("test");
    inputs = dst.inputs;
    expected = dst.expected;
    
    fail = 0; count = 0;
    
    for(int i = 0; i < inputs.size(); i++)
    {
        int e = expected[i];
        int out = m.postprocess(m.input(inputs[i])); // m.train(cnn.input(inputs[i]), e); cnn.backpropogate(m.getLastLoss());
        bool pass = (out == e); count++; if(!pass) fail++;
        cout << (int)((1-((float)fail/count))*100) << "% "; cout << i << " (" << e << " > " << out << ")" << (pass ? " PASS" : " ") << endl;
        // printInput(inputs[i]);
        // printInput(getWsum(m));
        // printInput(m.getLastLoss());
        // if(i % 1000 == 0) { fail = 0; count = 0; }
    }
    
    // cout << endl << "fail count :" << fail << endl;
}


int main00()
{
    seedRandom();
    
    // scale back and forward prop
    
    // training 
    
    int size = 10;
    
    int iSize = size;
    vector<int> hSize = {size};
    int oSize = size;
    
    vars::biasPlasticity = 0.1;
    vars::plasticity = 1;
    
    model m(iSize, hSize, oSize);
    
    int fail = 0; int count = 0;
    
    for(int a = 0; a < 999; a++)
    {
        int in = a % iSize; // random_float() * size;
        int e = in % oSize;
        
        int out = m.train(in, e);
        bool pass = (out == e); count++; if(!pass) fail++;
        cout << (int)((1-((float)fail/count))*100) << "% "; cout << a << " (" << e << " > " << out << ")" << (pass ? " PASS" : " ") << endl;
    }
    cout << endl << "fail count :" << fail << endl;
    
    /*
    cout << endl << "TESTING----------------" << endl << endl;
    
    fail = 0; count = 0;
    
    for(int a = 0; a < 999; a++)
    {
        int in = a % iSize; // random_float() * size;
        int e = in % oSize;
        
        int out = m.input(in);
        bool pass = (out == e); count++; if(!pass) fail++;
        cout << (int)((1-((float)fail/count))*100) << "% "; cout << a << " (" << e << " > " << out << ")" << (pass ? " PASS" : " ") << endl;
    }
    
    cout << endl << "fail count :" << fail << endl;
    */
}