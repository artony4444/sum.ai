

class neuron
{
    public:
    
    string name = "unspecified";
    
    float bias = random_float() * 2 - 1;
    float charges = 0;
    
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
        return c + bias;
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
        
        neuron* from;
        neuron* to;
        
        path(neuron* in, neuron* out)
        
        {
            from = in;
            to = out;
        }
        
        void fire(float c)
        {
            to->charge(c * weight);
        }
    };
};