/*
    reading suggestion
    1) model/neuron.cpp
    2) model/model.cpp
    3) main.cpp
    tools/* with all of them
    
    ignore model/cnn/* (under development)
*/

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
#include <filesystem>

struct dataset {
    vector<vector<float>> inputs;
    vector<int> expected;
};

dataset getData(string type = "test") // "test" or "train"
{
    
    // CHANGE [string directory] as per "/datasets" file location
    string directory = "/storage/emulated/0/my files/backups/files/codes/c++/ai"; // __fs::filesystem::current_path();
    string path = "/datasets/mnist_"+type+".csv";
    
    cout << "loading file at :- \n\n"+directory+path << endl;
    ifstream file(directory+path); if(!(file)) cout << "\n-------------- file not found!" << endl << endl;
    
    string line;
    getline(file, line); // copies file's line to string line (one by one in each call in loop) (this call removes header 1x1 1x2 1x3 1x4.. and do nothing with it)
    
    vector<vector<float>> inputs; // 28x28 = 784 // images
    vector<int> expected; // labels [0123456789]
    
    for(int a = 0; a < vars::maxDatasetSize && getline(file, line); a++) // vars::maxDatasetSize limits importing as 60,000 takes time to import
    {
        vector<string> str = split(line, ","); // csv to string || eg. "1, 2, 3" to {"1", "2", "3"}
        vector<float> train = toFloats(str); // string to float || eg. {"1", "2", "3"} to {1, 2, 3}
        
        expected.push_back(train[0]); // train[0] contains label [0123456789]
        train.erase(train.begin());   // removes train[0] label from dataset
        
        divv(train, 255); // divide by 255 gray-scale to float [0-1]
        inputs.push_back(train); // one by one, pushing lines
    }
    
    file.close();
    
    return { inputs, expected };
}


int main()
{
    seedRandom(); // required to seed random once at start. (I did'nt find other way to get true random)
    
    // import CSV
    
    vars::maxDatasetSize = 60000; // limits dataset import, 60,000 is huge dataset and takes time, so I limit for testing purpose
    dataset dst = getData("train"); // imports CSV
    vector<vector<float>>& inputs = dst.inputs;
    vector<int>& expected = dst.expected;
    
    // training
    
    int iSize = 784;             // inputSize
    vector<int> hSize = {15};    // hiddenSize
    int oSize = 10;              // outputSize
    
    vars::biasPlasticity = 0.001;
    vars::plasticity = 0.01;
    vars::batchSize = 1;
    vars::epoch = 1;
    
    model m(iSize, hSize, oSize);
    
    int fail = 0; int count = 0; int accuracyStep = 1000; vector<float> accuracyV(accuracyStep, 0);
    
    for(int x = 0; x < 1; x++)
    {
        for(int y = 0; y < vars::epoch * vars::batchSize; y++) // using epoch * batch for equal learning rate
        {
            for(int i = 0; i < inputs.size(); i++)
            {
                int e = expected[i];
                int out = m.train(inputs[i], e);
                bool pass = (out == e); count++;// if(!pass) fail++;
                accuracyV.erase(accuracyV.begin()); accuracyV.push_back(pass*100);
                int accuracy = (int)(summ(accuracyV) / accuracyStep); //(int)((1-((float)fail/count))*100);
                cout << x << y << " " << accuracy << "% "; cout << i << " (" << e << " > " << out << ")" << (pass ? " PASS" : " ") << endl;
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
        int accuracy = (int)((1-((float)fail/count))*100);
        cout << accuracy << "% "; cout << i << " (" << e << " > " << out << ")" << (pass ? " PASS" : " ") << endl;
        // printInput(inputs[i]);
        // printInput(getWsum(m));
        // printInput(m.getLastLoss());
        // if(i % 1000 == 0) { fail = 0; count = 0; }
    }
    
    // cout << endl << "fail count :" << fail << endl;
}


/*
int main() // test2 disabled
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
    vars::batchSize = 10;
    
    int totalAccuracy = 0;
    int totalaccuracyCount = 0;
    
    for(int z = 0; z < 10; z++)
    {
        model m(iSize, hSize, oSize);
        
        int fail = 0; int count = 0;
        
        for(int a = 0; a < 10 * vars::batchSize; a++)
        {
            int in = a % iSize; // random_float() * size;
            int e = in % oSize;
            
            int out = m.train(in, e);
            bool pass = (out == e); count++; if(!pass) fail++;
            // cout << (int)((1-((float)fail/count))*100) << "% "; cout << a << " (" << e << " > " << out << ")" << (pass ? " PASS" : " ") << endl;
        }
        int accuracy = (int)((1-((float)fail/count))*100);
        totalAccuracy += accuracy; totalaccuracyCount++;
        cout << endl << "fail count :" << fail << endl;
        cout << "accuracy : " << accuracy << "%" << endl;
    }
    
    cout << endl << "total accuracy : " << (totalAccuracy/totalaccuracyCount) << "%";
    
    
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

}
*/