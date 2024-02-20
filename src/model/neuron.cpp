

class neuron
{
    public:
    
    string name = "unspecified";
    
    float bias = random_float() * 2 - 1;
    float charges = 0;
    float lastCharge = 0;
    
    class path;
    vector<path*> pathIn;
    vector<path*> pathOut;
    
    neuron(){}
    neuron(string n) { name = n + to_string( (int) (random_float(1000)*1000) ); }
    
    void charge(float c)
    {
        charges += c;
    }
    
    float discharge()
    {
        float c = charges;
        charges = 0;
        lastCharge = c;
        return c; // + bias; // (bias sucks!)
    }
    
    float fire()
    {
        float c = activation(discharge());
        for(path* p : pathOut) { p->fire(c); }
        return c;
    }
    
    float activation(float c)
    {
        return c; // 0 > c ? 0 : c;
    }
    
    // back propogation
    
    void feedback(float loss) // shift this function to model for better control?
    {
        bias += loss * vars::plasticity;
        for(path* p : pathIn) { p->feedback(loss); }
    }
    
    // path
    
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
        
        float weight = random_float() * 2 - 1;
        float lastCharge = 0;
        
        neuron* from;
        neuron* to;
        
        path(neuron* in, neuron* out)
        {
            from = in;
            to = out;
        }
        
        void fire(float c)
        {
            lastCharge = c * weight;
            to->charge(c * weight);
        }
        
        void feedback(float loss)
        {
            weight += from->lastCharge * loss * vars::plasticity;
            from->feedback(loss);
        }
    };
};