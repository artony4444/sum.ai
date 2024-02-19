

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
    
    // input -----
    
    int input(int i)
    {
        return input(vectorize(i));
    }
    
    int input(vector<int> i)
    {
        return postprocess(input(preprocess(i)));
    }
    
    vector<float> input(vector<float> input)
    {
        return forwardprop(input);
    }
    
    // train -----
    
    int train(int i, int e)
    {
        return train(vectorize(i), vectorize(e));
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
    
    
    // ---------- helper ---------- //
    
    
    // structure -----
    
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
                    
                    path* p = new path( n1, n2 );
                    n1->addPathOut(p);
                    n2->addPathIn(p);
                }
            }
        }
    }
    
    // forwardprop -----
    
    vector<float> forwardprop(vector<float> input)
    {
        charge(input);
        fire();
        return discharge();
    }
    
    void charge(vector<float> input)
    {
        for(int i = 0; structure[0].size() > i; i++) { structure[0][i].charge(input[i]); }
    }
    
    void fire()
    {
        for(int l = 0; structure.size()-1 > l; l++)
        {
            for(int i = 0; structure[l].size() > i; i++)
            {
                float f = structure[l][i].fire();
                for(path* p : structure[l][i].pathOut) { p->fire(f); }
            }
        }
    }
    
    vector<float> discharge()
    {
        vector<float> output;
        for(neuron& n : structure.back()) { output.push_back(n.discharge()); }
        return output;
    }
    
    // processing -----
    
    vector<float> preprocess(vector<int> indexes)
    {
        int size = structure[0].size();
        vector<float> output(size, 0);
        for(int index : indexes) { if(index < size && index >= 0) output[index] = 1; }
        return output;
    }
    
    int postprocess(vector<float> output)
    {
        int len = output.size();
        int maxIndex = 0;
        for(int i = 0; i < len; i++) { if(output[i] > output[maxIndex]) { maxIndex = i; } }
        return maxIndex;
    }
};