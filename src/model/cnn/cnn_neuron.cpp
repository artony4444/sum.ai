class cnn_neuron
{
    public:
    
    string name = "unspecified";
    int id = 8888;
    
    float bias = 0;
    
    float charges = 0;
    float lastCharge = 0;
    
    bool debug = true;
    bool init = false;
    
    cnn_neuron(){}
    cnn_neuron(string n)
    {
        getName(n);
    }
    
    void getName(string n)
    {
        name = n + to_string( (int) (random_float(1000)*1000) );
    }
    
    void charge(float c)
    {
        if(name[0] == 'm') charges = c > charges ? c : charges;
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
        float c = activation(discharge()); // if(c <= 0) return;
        lastCharge = c;
        for(path* p : pathOut) { p->fire(c); }
    }
    
    float activation(float c)
    {
        return name[0] == 'm' ? c : functions::relu(c);
    }
    
    // back propogation
    
    void backpropogate(float loss)
    {
        update(loss); // softmax derivative (loss * (1 - loss))
    }
    
    void feedback(float loss, float weight)
    {
        update(loss * 1 * 1);
    }
    
    void update(float loss)
    {
        if(loss == 0) return; if(name[0] == 'i') return;
        bias += loss * vars::biasPlasticity;
        for(path* p : pathIn) { p->feedback(loss); }
    }
    
    // path
    
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
        
        float weight = random_float() * 2 - 1;
        
        cnn_neuron* from;
        cnn_neuron* to;
        
        path(cnn_neuron* in, cnn_neuron* out)
        {
            // cout << in->name << " > " << out->name << endl;
            from = in;
            to = out;
        }
        
        void fire(float c)
        {
            float charge = c * weight;
            lastCharge = charge;
            to->charge(charge);
        }
        
        float lastCharge = 0;
        float lastLoss = 0;
        float lastGradient = 0;
        
        void feedback(float loss)
        {
            weight += from->lastCharge * loss * vars::plasticity;
            from->feedback(loss, weight);
        }
    };
};