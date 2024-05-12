/* (!) INCORRECTLY CODED */

class lstm_cell : public neuron
{
    public:
    
    float cell_state = random_float();
    float hidden_state = random_float();
    float charges = 0;
    
    float lastCellState = random_float();
    
    string layer = "none";
    
    lstm_cell() {}
    lstm_cell(string connect, string layerType) : neuron(connect) { this->layer = layerType; }
    
    string getType() override { return "lstm"; }
    
    void charge(float c) override 
    {
        charges += c;
    }
    
    float discharge() override
    {
        return hidden_state;
    }
    
    void fire() override
    {
        lastCellState = cell_state;
        
        cell_state  *= gates["forget"].run(this);
        cell_state  += gates["input1"].run(this) * gates["input2"].run(this);
        hidden_state = gates["output"].run(this) * tanh(cell_state);
        
        charges = 0;
        
        for(neuron::path* p : pathOut) { p->fire(hidden_state); }
    }
    
    float getLastCharge() override
    {
        return hidden_state;
    }
    
    // backpropogation
    
    void backpropogate(float loss) // call from model::train() || once every train samples
    {
        update(loss);
    }
    
    void feedback(float loss, float weight = 1) // call from neuron::path::feedback() || multiple times from every pathOut
    {
        update(loss * weight); // derivative of tanh (1-tanh²) sigmoid (σ*(1-σ) || derivatives sucks!
    }
    
    void update(float loss)
    {
        float lossBack = updateGates(loss);
        for(neuron::path* p : pathIn) { p->feedback(lossBack); }
    }
    
    float updateGates(float loss)
    {
        gate& output = gates["output"];
        gate& input1 = gates["input1"];
        gate& input2 = gates["input2"];
        gate& forget = gates["forget"];
        
        // BP != BPTT
        
        float csloss = loss * (1-(pow(tanh(cell_state),2))) * output.lastCharge;
        float oloss = output.feedback(loss * tanh(cell_state));
        
        float i1loss = input1.feedback(csloss * input2.lastCharge);
        float i2loss = input2.feedback(csloss * input1.lastCharge);
        
        // csloss = (i1loss/input1.weight * input2.lastCharge) + (input1.lastCharge * i2loss/input2.weight); // + 1
        float floss = forget.feedback(csloss * lastCellState);
        
        return oloss + i2loss + i1loss + floss;
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
        float weight = random_float();   // ((random_float() * 2.9) - 1.45) * vars::plasticity;
        float h_weight = random_float(); // ((random_float() * 2.9) - 1.45) * vars::plasticity;
        
        string function = "sigmoid";
        
        float lastInput = 0;
        float lastState = 0;
        float lastCharge = 0;
        
        gate(){}
        gate(string f) { function = f; }
        
        float run(lstm_cell* c)
        {
            float input = c->charges;
            float state = c->hidden_state;
            
            float i = input * weight;
            float h = state * h_weight;
            float sum = i + h + bias;
            
            lastInput = input;
            lastState = state;
            lastCharge = sum;
            
            return activation(sum);
        }
        
        float activation(float c)
        {
            if(function == "sigmoid") return functions::sigmoid(c);
            else if(function == "tanh") return tanh(c);
            else return c;
        }
        
        float derivative(float x)
        {
            if(function == "sigmoid") return (functions::sigmoid(x) * (1 - functions::sigmoid(x)));
            else if(function == "tanh") return (1-(tanh(x)*tanh(x)));
            return x;
        }
        
        float feedback(float iloss)
        {
            float loss = iloss * derivative(lastCharge);
            
            bias += (loss * vars::biasPlasticity) / vars::batchSize;
            weight += (lastInput * loss * vars::plasticity) / vars::batchSize;
            h_weight += (lastState * loss * vars::plasticity) / vars::batchSize;
            
            return loss * weight;
        }
    };
    
    // path ----
    
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


/*


DETAILS

video :- https://youtu.be/YCzL96nL7j0

L = long term memory
S = short term memory
x¹ = potential
x² = to remember
σ = sigmoid()
t = tanh()


w = weight
sw = stm weight
b = bias
c = input * W
s = S * sW

all functions have their own w, sw, b

forget gate
L = L * σ(c + s + b)

input gate
L¹ = σ(c + s + b)
L² = t(c + s + b)
L += L¹ * L²

output gate
S¹ = σ(c + s + b)
S² = t(L)
S = S¹ * S²



*/