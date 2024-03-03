

class neuron
{
    public:
    
    string name = "unspecified";
    
    float bias = 0; //random_float() * 2 - 1;
    
    float charges = 0;
    float lastCharge = 0;
    float lastLoss = 0;
    
    neuron(){}
    neuron(string n) { name = n + to_string( (int) (random_float(1000)*1000) ); }
    
    void charge(float c)
    {
        charges += c;
    }
    
    float discharge()
    {
        float c = charges;// + bias;
        charges = 0;
        return c; // + bias; // (bias sucks!)
    }
    
    void fire()
    {
        float c = activation(discharge());// if(c < 0) return;
        lastCharge = c;
        for(path* p : pathOut) { p->fire(c); }
    }
    
    float activation(float c)
    {
        return tanh(c);
    }
    
    // back propogation
    
    int batchID = 1;
    
    void backpropogate(float loss)
    {
        update(loss, batchID); // softmax derivative (loss * (1 - loss))
        batchID++; if(batchID > vars::batchSize) batchID = 1;
    }
    
    void feedback(float loss, float weight, int bid)
    {
        update(loss * weight * 1, bid);
    }
    
    void update(float loss, int bid)
    {
        if(loss == 0) return; if(name[0] == 'i') return;
        lastLoss = loss;
        bias += loss * vars::biasPlasticity;
        for(path* p : pathIn) { p->feedback(loss, bid); }
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
            lastCharge = charge;
            to->charge(charge);
        }
        
        float lastCharge = 0;
        float lastLoss = 0;
        float lastGradient = 0;
        
        float lossSum = 0;
        int lossCount = 0;
        
        void feedback(float loss, int batchID)
        {
            float cLoss = from->lastCharge * loss * vars::plasticity;
            lossSum += cLoss; lossCount++;
            
            if(batchID >= vars::batchSize)
            {
                weight += (lossSum / lossCount);
                lossSum = 0; lossCount = 0;
            }
            from->feedback(loss, weight, batchID);
        }
    };
};