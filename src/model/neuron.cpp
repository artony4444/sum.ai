

class neuron
{
    public:
    
    string name = "unspecified";
    
    float bias = 0;
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
    
    int batchID = 1; // starts with 1 and ends at batchSize
    
    void backpropogate(float loss) // callFrom line4 model::train()
    {
        update(loss, batchID);
        batchID++; if(batchID > vars::batchSize) batchID = 1;
    }
    
    void feedback(float loss, float weight, int bid) // callFrom lastLine neuron::path::feedback()
    {
        update(loss * weight * (1-(pow(tanh(lastCharge),2))) , bid); // derivative of tanh 1-tanh²
    }
    
    float lossSum = 0; int lossCount = 0;
    
    void update(float loss, int bid)
    {
        if(loss == 0) return; if(name[0] == 'i') return;
        lastLoss = loss;
        
        // current loss
        float cLoss = loss * vars::biasPlasticity;
        lossSum += cLoss; lossCount++;
        
        if(bid >= vars::batchSize) // bias changes after every batch
        {
            // average loss
            bias += (lossSum / lossCount);
            lossSum = 0; lossCount = 0;
        }
        
        for(path* p : pathIn) { p->feedback(loss, bid); }
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
        
        float lossSum = 0; int lossCount = 0;
        
        void feedback(float loss, int batchID)
        {
            // current loss
            float cLoss = from->lastCharge * loss * vars::plasticity;
            lossSum += cLoss; lossCount++;
            
            if(batchID >= vars::batchSize) // weight changes after every batch
            {
                // average loss
                weight += (lossSum / lossCount);
                lossSum = 0; lossCount = 0;
            }
            
            from->feedback(loss, weight, batchID);
        }
    };
};