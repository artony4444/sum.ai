struct layer // template
{
    public:
    
    int size;
    vector<int> shape;
    
    string type;
    string connect;
    
    layer(int size, string type="vanilla", string connect="dense")
    {
        this->size = size;
        this->shape.push_back(size);
        
        this->type = type;
        this->connect = connect;
    }
    
    layer(vector<int> shape, string type="vanilla", string connect="dense")
    {
        this->size = summ(shape);
        this->shape = shape;
        
        this->type = type;
        this->connect = connect;
    }
};

class model
{
    public:
    
    vector<vector<neuron*>> structure;
    vector<vector<int>> shape;
    
    // constructor
    
    model(){}
    
    model(int i, vector<int> h, int o)
    {
        structure.push_back(createLayer(i, "i")); shape.push_back({i});
        for(int n : h) {structure.push_back(createLayer(n, "h")); shape.push_back({n}); }
        structure.push_back(createLayer(o, "o")); shape.push_back({o});
        
        connectLayers();
    }
    
    model(layer* i, vector<layer*> h, layer* o)
    {
        structure.push_back(createLayer(i->size, "i", i->type, i->connect)); shape.push_back(i->shape);
        for(layer* l : h) { structure.push_back(createLayer(l->size, "h", l->type, l->connect)); shape.push_back(l->shape); }
        structure.push_back(createLayer(o->size, "o", o->type, o->connect)); shape.push_back(o->shape);
        
        connectLayers();
    }
    
    // layer
    
    vector<neuron*> createLayer(int size, string layer, string type="vanilla", string connect="dense")
    {
        vector<neuron*> r;
        for(int a = 0; a < size; a++) { r.push_back(newNeuron(layer, type, connect)); }
        return r;
    }
    
    neuron* newNeuron(string layer, string type, string connect)
    {
        if(type == "vanilla") { return new vanilla(connect, layer); }
        if(type == "lstm") { return new lstm_cell(connect, layer); }
        return new vanilla(connect, layer);
    }
    
    // connections
    
    void connectLayers()
    {
        for(int layer = structure.size()-1; layer > 0; layer--) // backward loop <--
        {
            for(int index = 0; index < structure[layer].size(); index++)
            {
                string connect = structure[layer][index]->connect;
                
                if(connect == "dense") connectDense(layer, index);
                else connectDense(layer, index);
            }
        }
    }
    
    void connectDense(int layer, int index)
    {
        neuron* n1 = structure[layer][index];
        
        for(neuron* n2 : structure[layer-1])
        {
            neuron::path* p = new neuron::path(n2, n1);
            n2->addPathOut(p); n1->addPathIn(p);
        }
    }
    
    
    // input
    
    int input(int index) { return input(toVector(index)); }
    
    int input(vector<int> i) { return postprocess(input(preprocess(i))); }
    
    vector<float> input(vector<float> input) 
    { 
        return feedforward(input);
    }
    
    // train
    
    int train(int i, int e) { return train(toVector(i), toVector(e)); } //  (i)input  (e)expected
    
    int train(vector<int> i, vector<int> e) { return postprocess(train(preprocess(i), preprocess(e))); }
    
    vector<float> train(vector<float> input, vector<float> expected)
    {
        vector<float> output = this->input(input);
        vector<float> loss = getLoss(output, expected);
        int i = 0; for(neuron* n : structure.back()) { n->backpropogate(loss[i]); i++; }
        return output;
    }
    
    int train(vector<float> i, int e) { return train(i, toVector(e)); } //  (i)input  (e)expected
    
    int train(vector<float> i, vector<int> e) { return postprocess(train(i, preprocess(e))); }
    
    
    // forwardprop
    
    vector<float> feedforward(vector<float>& input)
    {
        charge(input);
        fire();
        return discharge();
    }
    
    void charge(vector<float>& input)
    {
        for(int i = 0; i < structure[0].size(); i++) { structure[0][i]->charge(input[i]); }
    }
    
    void fire()
    {
        for(int l = 0; l < structure.size(); l++)
        {
            for(int i = 0; i < structure[l].size(); i++)
            {
                structure[l][i]->fire();
            }
        }
    }
    
    vector<float> discharge()
    {
        vector<float> output;
        for(neuron* n : structure.back()) { output.push_back(n->discharge()); } // cout << "\noutput :"; print(output); cout << endl;
        return functions::softmax(output);
    }
    
    // backprop
    
    vector<float> getLoss(vector<float>& output, vector<float>& expected)
    {
        vector<float> loss;
        for(int i = 0; i < output.size(); i++)
        {
            loss.push_back(expected[i] - output[i]); // cout << i << "  e:" << expected[i] << "   o:" << roundm(output[i]) << "   l:" << roundm(loss.back()) << endl;
        }
        return loss;
    }
    
    // processing -----
    
    vector<float> preprocess(int index) { preprocess({index}); }
    
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