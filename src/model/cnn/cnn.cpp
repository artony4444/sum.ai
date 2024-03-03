

class cnn
{
    public:
    
    vector< vector<vector<cnn_neuron>> > structure;
    
    cnn(vector<int> inputS, vector<int> cnnS, vector<int> outputS)
    {
        createStructure(inputS, cnnS, outputS);
    }
    
    // structure -----
    
    void createStructure(vector<int> inputS, vector<int> cnnS, vector<int> outputS)
    {
        structure.push_back(createLayer(inputS, "i"));
        structure.push_back(createLayer(cnnS, "c"));
        structure.push_back(createLayer(outputS, "m"));
        
        connectCnn();
    }
    
    vector<vector<cnn_neuron>> createLayer(vector<int> size, string name) 
    {
        vector<vector<cnn_neuron>> layer;
        
        for(int a = 0; a < size[0]; a++)
        {
            vector<cnn_neuron> yLayer;
            
            for(int b = 0; b < size[1]; b++) { yLayer.push_back(*new cnn_neuron(name)); }
            
            layer.push_back(yLayer);
        }
        // return vector<vector<cnn_neuron>>(size[0], *new vector<cnn_neuron>(size[1], *new cnn_neuron(name)) ); // name init is not random
        return layer;
    }
    
    void connectCnn()
    {
        for(int layer = 0; layer < structure.size()-1; layer++)
        {
            for(int nl = 0; nl < structure[layer].size(); nl++) 
            {
                for(int n = 0; n < structure[layer][nl].size(); n++)
                {
                    float y = (float) nl / structure[layer].size();
                    float x = (float) n / structure[layer][nl].size();
                    
                    int yi = roundm( (structure[layer+1].size() * y), 1);
                    int xi = roundm( (structure[layer+1][yi].size() * x), 1);
                    
                    cnn_neuron* n1 = &structure[layer][nl][n];
                    cnn_neuron* n2 = &structure[layer+1][yi][xi];
                    
                    cnn_neuron::path* p = new cnn_neuron::path( n1, n2 );
                    n1->addPathOut(p);
                    n2->addPathIn(p);
                    
                    // cout << "x :"<< x << " y :"<< y << " xi :"<< xi << " yi :"<< yi << endl;
                }
            }
        }
    }
    
    // input -----
    
    vector<float> input(vector<float> input) 
    { 
        return feedforward(input);
    }
    
    
    // forwardprop -----
    
    vector<float> feedforward(vector<float> input)
    {
        charge(input);
        fire();
        return discharge();
    }
    
    // cycle -----
    
    void charge(vector<float> input)
    {
        int ySize = structure[0].size();
        
        for(int y = 0; ySize > y; y++)
        {
            for(int x = 0; structure[0][y].size() > x; x++)
            {
                structure[0][y][x].charge(input[y*ySize+x]);
            }
        }
    }
    
    void fire()
    {
        for(int l = 0; structure.size()-1 > l; l++)
        {
            for(int y = 0; structure[l].size() > y; y++)
            {
                for(int x = 0; structure[l][y].size() > x; x++)
                {
                    structure[l][y][x].fire();
                }
            }
        }
    }
    
    vector<float> discharge()
    {
        vector<float> output;
        
        int index = structure.size()-1;
        
        for(int l = index; l == index; l++)
        {
            for(int y = 0; structure[l].size() > y; y++)
            {
                for(int x = 0; structure[l][y].size() > x; x++)
                {
                    output.push_back(structure[l][y][x].discharge());
                }
            }
        }
        // cout << summ(output) << endl; // print(output);
        return output;
    }
    
    void backpropogate(vector<float> loss)
    {
        int index = structure.size()-1;
        
        for(int l = index; l == index; l++)
        {
            int layerSize = structure[l].size();
            
            for(int y = 0; layerSize > y; y++)
            {
                for(int x = 0; structure[l][y].size() > x; x++)
                {
                    structure[l][y][x].backpropogate(loss[y*layerSize+x]);
                }
            }
        }
    }
};