

class neuron // template class
{
    public:
    
    string connect = "dense";
    
    neuron() {}
    neuron(string connect) { this->connect = connect; }
    
    virtual string getType() { return "neuron"; }
    
    virtual void charge(float c){}
    virtual void fire(){}
    virtual float discharge(){}
    virtual float getLastCharge(){}
    
    virtual void backpropogate(float loss){}
    virtual void feedback(float loss, float weight = 1){}
    virtual void update(float loss){}
    
    class path;
    
    virtual void addPathIn(path* p){}
    virtual void addPathOut(path* p){}
    
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
            weight += (from->getLastCharge() * loss * vars::plasticity) / vars::batchSize;
            from->feedback(loss, weight);
        }
    };
};