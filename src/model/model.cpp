

class model
{
    public:
    
    vector<vector<neuron>> structure;
    
    model(int inputS, vector<int> hiddenS, int outputS)
    {
        createStructure(inputS, hiddenS, outputS);
    }
    
    // structure -----
    
    void createStructure(int inputS, vector<int> hiddenS, int outputS)
    {
        structure.push_back(createLayer(inputS, "i"));
        for(int n : hiddenS) {structure.push_back(createLayer(n, "h"));}
        structure.push_back(createLayer(outputS, "o"));
        
        connectDense();
    }
    
    vector<neuron> createLayer(int size, string name) { return *new vector<neuron>(size, *new neuron(name) ); }
    
    void connectDense()
    {
        for(int a = 0; a < structure.size()-1; a++)
        {
            for(int b = 0; b < structure[a].size(); b++) 
            {
                neuron* n1 = &structure[a][b];
                
                for(neuron& n : structure[a+1])
                {
                    neuron* n2 = &n;
                    
                    neuron::path* p = new neuron::path( n1, n2 );
                    n1->addPathOut(p);
                    n2->addPathIn(p);
                }
            }
        }
    }
    
    // input -----
    
    int input(int index) { return input(toVector(index)); }
    
    int input(vector<int> i) { return postprocess(input(preprocess(i))); }
    
    vector<float> input(vector<float> input) 
    { 
        return feedforward(input);
    }
    
    // train -----
    
    int train(int i, int e) { return train(toVector(i), toVector(e)); } //  (i)input  (e)expected
    
    int train(vector<int> i, vector<int> e) { return postprocess(train(preprocess(i), preprocess(e))); }
    
    vector<float> train(vector<float> input, vector<float> expected)
    {
        vector<float> rawOutput = this->input(input);
        vector<float> output = functions::softmax(rawOutput); // if(postprocess(output) == postprocess(expected)) return output; // increases flexiblity
        vector<float> loss = getLoss(output, expected);
        int i = 0; for(neuron& n : structure.back()) { n.backpropogate(loss[i]); i++; }
        return output;
    }
    
    int train(vector<float> i, int e) { return train(i, toVector(e)); } //  (i)input  (e)expected
    
    int train(vector<float> i, vector<int> e) { return postprocess(train(i, preprocess(e))); }
    
    
    
    // forwardprop -----
    
    vector<float> feedforward(vector<float>& input)
    {
        charge(input);
        fire();
        return discharge();
    }
    
    // cycle -----
    
    void charge(vector<float>& input)
    {
        for(int i = 0; structure[0].size() > i; i++) { structure[0][i].charge(input[i]); }
    }
    
    void fire()
    {
        for(int l = 0; structure.size()-1 > l; l++)
        {
            for(int i = 0; structure[l].size() > i; i++)
            {
                structure[l][i].fire();
            }
        }
    }
    
    vector<float> discharge()
    {
        vector<float> output;
        for(neuron& n : structure.back()) { output.push_back(n.discharge()); } // print(output);
        return output;
    }
    
    // backprop -----
    
    // read with model::train()
    
    vector<float> getLoss(vector<float>& output, vector<float>& expected)
    {
        vector<float> loss;
        for(int i = 0; i < output.size(); i++)
        {
            loss.push_back(expected[i] - output[i]); // cout << i << "  e:" << expected[i] << "   o:" << roundm(output[i]) << "   l:" << roundm(loss.back()) << endl;
        }
        return loss;
    }
    
    vector<float> getLastLoss()
    {
        vector<float> loss;
        for(neuron& n : structure.back()) { loss.push_back(n.lastLoss); } // print(output);
        return loss;
    }
    
    // processing -----
    
    vector<float> preprocess(vector<int> indexes) // indexes > charges | creates a float vector and charges indexes with charge "1".
    {
        int size = structure[0].size();
        vector<float> output(size, 0);
        for(int index : indexes) { if(index < size && index >= 0) output[index] = 1; }
        return output;
    }
    
    int postprocess(vector<float> output) // charges > index | gets index of max charge in a vector
    {
        int len = output.size();
        int maxIndex = 0;
        for(int i = 0; i < len; i++) { if(output[i] > output[maxIndex]) { maxIndex = i; } }
        return maxIndex;
    }
};