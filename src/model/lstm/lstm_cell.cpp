#include <iostream>
#include <map>
using namespace std;

class lstm_cell : public neuron
{
    public:
    
    float cell_state = 1;
    float hidden_state = 1;
    float charges = 0;
    
    lstm_cell() {}
    
    void charge(float c) { charges += c; }
    
    void fire()
    {
        cell_state *= gates["forget"].run(this);
        cell_state += gates["input1"].run(this) * gates["input2"].run(this);
        hidden_state = gates["output"].run(this) * tanh(cell_state);
        
        charges = 0;
        for(path* p : pathOut) { p->fire(hidden_state); }
    }
    
    // gate -----
    
    class gate;
    map<string, gate> gates {
        {"forget", *new gate("sigmoid")},
        {"input1", *new gate("sigmoid")},
        {"input2", *new gate("tanh")},
        {"output", *new gate("sigmoid")}
    };
    
    class gate
    {
        public:
        
        float bias = 0;
        float weight = 1;
        float h_weight = 1;
        
        string function = "sigmoid";
        
        gate(){}
        gate(string f) { function = f; }
        
        float activation(float c)
        {
            if(function == "sigmoid") return functions::sigmoid(c);
            else if(function == "tanh") return tanh(c);
            else return c;
        }
        
        float run(lstm_cell* c)
        {
            float i = c->charges * weight;
            float s = c->hidden_state * h_weight;
            float sum = i + s + bias;
            return activation(sum);
        }
    };
    
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
        
        lstm_cell* from;
        lstm_cell* to;
        
        path(lstm_cell* in, lstm_cell* out)
        {
            from = in;
            to = out;
        }
        
        void fire(float c)
        {
            float charge = c * weight;
            to->charge(charge);
        }
    };
};


/*





*/