

class neuron
{
    public:
    
    string name = "unspecified";
    
    float bias = 0;
    float charges = 0;
    float lastCharge = 0;
    
    
    neuron(){}
    neuron(string n) { name = n + to_string( (int) (random_float(1000)*1000) ); }
    
    // feed forward
    
    void charge(float c)
    {
        charges += c;
    }
    
    float discharge()
    {
        float c = charges + bias;
        charges = 0;
        return c;
    }
    
    void fire()
    {
        float c = activation(discharge());
        lastCharge = c;
        for(path* p : pathOut) { p->fire(c); }
    }
    
    float activation(float c)
    {
        return tanh(c);
    }
    
    // back propogation
    
    void backpropogate(float loss) // call from model::train() || once every train samples
    {
        update(loss);
    }
    
    void feedback(float loss, float weight) // call from neuron::path::feedback() || multiple times from every pathOut
    {
        update(loss * weight); // derivative of tanh 1-tanh² || derivatives sucks!
    }
    
    void update(float loss)
    {
        if(loss == 0) return; if(name[0] == 'i') return;
        
        bias += (loss * vars::biasPlasticity * (1/pathOut.size()) ) / vars::batchSize;
        
        for(path* p : pathIn) { p->feedback(loss); }
    }
    
    // path -----
    
    class path;
    vector<path*> pathIn;
    vector<path*> pathOut;
    
    void addPathIn(path* p)
    {
        pathIn.push_back(p);
    }
    
    void addPathOut(path* p)
    {
        pathOut.push_back(p);
    }
    
    class path
    {
        public:
        
        float weight = ((random_float() * 2.9) - 1.45) * vars::plasticity;// * 2.9 - 1.45;
        
        neuron* from;
        neuron* to;
        
        path(neuron* in, neuron* out)
        {
            from = in;
            to = out;
        }
        
        void fire(float c)
        {
            float charge = c * weight;
            to->charge(charge);
        }
        
        void feedback(float loss)
        {
            weight += (from->lastCharge * loss * vars::plasticity) / vars::batchSize;
            
            from->feedback(loss, weight);
        }
    };
};
