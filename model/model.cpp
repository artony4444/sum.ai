

class model
{
    public:
    
    struct neurons
    {
        neuron neuron;
        vector<path> pathIn;
        vector<path> pathOut;
    };
    
    vector<vector<neurons>> structure;
    vector<int> size;
    
    model(int inputS, vector<int> hiddenS, int outputS)
    {
        createStructure(inputS, hiddenS, outputS);
        size.clear(); size.push_back(inputS); for(int s : hiddenS) { size.push_back(s); }; size.push_back(outputS);
    }
    
    // structure -----
    
    void createStructure(int inputS, vector<int> hiddenS, int outputS)
    {
        structure.push_back(createLayer(inputS, "i"));
        for(int n : hiddenS) {structure.push_back(createLayer(n, "h"));}
        structure.push_back(createLayer(outputS, "o"));
        
        connectDense();
    }
    
    // input -----
    
    int input(int i)
    {
        return input({i});
    }
    
    int input(vector<int> i)
    {
        return postprocess(input(preprocess(i)));
    }
    
    vector<float> input(vector<float> input)
    {
        return feedForward(input);
    }
    
    // train
    
    int train(int i, int e)
    {
        return train({i}, {e});
    }
    
    int train(vector<int> i, vector<int> e)
    {
        return postprocess(train(preprocess(i), preprocess(e)));
    }
    
    vector<float> train(vector<float> input, vector<float> expected)
    {
        vector<float> output = this->input(input);
        // backprop lossF gradiantD
        return output;
    }
    
    
    ////////// helper //////////
    
    
    // structure
    
    vector<neurons> createLayer(int size, string name) { return *new vector<neurons>(size, {*new neuron(name)} ); }
    
    void connectDense()
    {
        for(int a = 0; a < structure.size()-1; a++)
        {
            for(int b = 0; b < structure[a].size(); b++) 
            {
                neurons* n1 = &structure[a][b];
                
                for(neurons& nrs : structure[a+1])
                {
                    neurons* n2 = &nrs;
                    
                    path* p = new path( &(n1->neuron), &(n2->neuron) );
                    n1->pathOut.push_back(*p);
                    n2->pathIn.push_back(*p);
                }
            }
        }
    }
    
    // feed forward
    
    vector<float> feedForward(vector<float> input)
    {
        charge(input);
        fire();
        return discharge();
    }
    
    void charge(vector<float> input)
    {
        for(int i = 0; structure[0].size() > i; i++) { structure[0][i].neuron.charge(input[i]); }
    }
    
    void fire()
    {
        for(int l = 0; structure.size()-1 > l; l++)
        {
            for(int i = 0; structure[l].size() > i; i++)
            {
                float f = structure[l][i].neuron.fire();
                for(path& p : structure[l][i].pathOut) { p.fire(f); }
            }
        }
    }
    
    vector<float> discharge()
    {
        vector<float> output;
        for(neurons& nrs : structure.back()) { output.push_back(nrs.neuron.discharge()); }
        return output;
    }
    
    // processing
    
    vector<float> preprocess(vector<int> indexes)
    {
        int size = structure[0].size();
        vector<float> output(size, 0);
        for(int index : indexes) { if(index < size && index >= 0) output[index] = 1; }
        return output;
    }
    
    int postprocess(vector<float> output)
    {
        return *max_element(output.begin(), output.end());
    }
};