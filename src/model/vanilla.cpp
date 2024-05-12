


class vanilla : public neuron
{
    public:
    
    float bias = 0;
    float charges = 0;
    
    string layer = "none";
    
    vanilla(){}
    vanilla(string connect, string layerType) : neuron(connect) { this->layer = layerType; }
    
    string getType() override { return "vanilla"; }
    
    // feedforward
    
    void charge(float c) override
    {
        charges += c;
    }
    
    float discharge() override
    {
        float c = charges + bias;
        charges = 0;
        return c;
    }
    
    void fire() override
    {
        // if(layer == "o") return; 
        /* dropout */ // if(layer == "h" && vars::dropout > 0 && random_float() < vars::dropout) { charges = 0; return; }
        float c = activation(discharge()); lastCharge = c;
        for(neuron::path* p : pathOut) { p->fire(c); }
    }
    
    float activation(float c)
    {
        return tanh(c);
    }
    
    float lastCharge = 0;
    
    float getLastCharge() override
    {
        return lastCharge;
    }
    
    // backpropogation
    
    void backpropogate(float loss) override // call from model::train() || once every train samples
    {
        update(loss);
    }
    
    void feedback(float loss, float weight = 1) override // call from neuron::path::feedback() || multiple times from every pathOut
    {
        if(layer == "i") return;
        update(loss * weight * (1 - pow(tanh(lastCharge),2)) ); // derivative of tanh 1-tanh² || derivatives sucks!
    }
    
    void update(float loss) override
    {
        bias += (loss * vars::biasPlasticity ) / vars::batchSize;
        
        for(neuron::path* p : pathIn) { p->feedback(loss); }
    }
    
    // ----- path -----
    
    vector<neuron::path*> pathIn;
    vector<neuron::path*> pathOut;
    
    void addPathIn(neuron::path* p) override
    {
        pathIn.push_back(p);
    }
    
    void addPathOut(neuron::path* p) override
    {
        pathOut.push_back(p);
    }
};





