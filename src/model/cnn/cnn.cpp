

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
        for(int layer = structure.size()-1; layer > 0 ; layer--)
        {
            if(true)
            {
                int windowSizeY = structure[layer-1].size() - (structure[layer].size() - 1);
                int windowSizeX = structure[layer-1][0].size() - (structure[layer][0].size() - 1);
                
                int stride = 1;
                
                if(structure[layer][0][0].name[0] == 'm')
                {
                    windowSizeY = structure[layer-1].size() / structure[layer].size();
                    windowSizeX = structure[layer-1][0].size() / structure[layer][0].size();
                    stride = windowSizeY;
                }
                
                cout << endl << "layer" << layer << " stride" << stride << " w" << windowSizeX << "x" << windowSizeY << endl;
                
                for(int nl = 0; nl < structure[layer].size(); nl++)
                {
                    for(int ni = 0; ni < structure[layer][nl].size(); ni++)
                    {
                        cnn_neuron* n2 = &structure[layer][nl][ni];
                        
                        for(int y = 0; y < windowSizeY; y++)
                        {
                            for(int x = 0; x < windowSizeX; x++)
                            {
                                int yi = y + (nl * stride); if(yi >= structure[layer-1].size()) continue;
                                int xi = x + (ni * stride); if(xi >= structure[layer-1][yi].size()) continue;
                                
                                cnn_neuron* n1 = &structure[layer-1][yi][xi];
                                
                                cnn_neuron::path* p = new cnn_neuron::path( n1, n2 );
                                n1->addPathOut(p);
                                n2->addPathIn(p);
                                n1->id = (yi*10) + xi;
                                n2->id = (nl*10) + ni;
                            }
                        }
                    }
                }
            }
        }
        
        /*
        // debug
        
        for(int layer = structure.size()-1; layer > 0 ; layer--)
        {
            cout << endl << layer << "layer \n";
            
            for(int nl = 0; nl < structure[layer].size(); nl++) 
            {
                cout << "l" << nl << endl;
                
                for(int n = 0; n < structure[layer][nl].size(); n++)
                {
                    cout << nl << n << " - ";
                    
                    for(cnn_neuron::path* p : structure[layer][nl][n].pathIn)
                    {
                        cout << p->from->id/10 << p->from->id%10 << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
        }
        */
        
    }
    
    // input -----
    
    vector<float> input(vector<float> input) 
    { 
        return feedforward(input);
    }
    
    // train
    
    vector<float> train(vector<float> input, vector<float> expected) 
    {
        vector<float> output = feedforward(input);
        vector<float> loss = getLoss(output, expected);
        backpropogate(loss);
        return output;
    }
    
    int train(vector<float> i, int e) { return train(i, toVector(e)); } //  (i)input  (e)expected
    
    int train(vector<float> i, vector<int> e) { return postprocess(train(i, preprocess(e))); }
    
    
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
    
    // back propogation
    
    vector<float> getLoss(vector<float>& output, vector<float>& expected)
    {
        vector<float> loss;
        for(int i = 0; i < output.size(); i++)
        {
            loss.push_back(expected[i] - output[i]); // cout << i << "  e:" << expected[i] << "   o:" << roundm(output[i]) << "   l:" << roundm(loss.back()) << endl;
        }
        return loss;
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
    
    
    // processing -----
    
    vector<float> preprocess(vector<int> indexes) // indexes > charges | creates a float vector and charges indexes with charge "1".
    {
        int size = structure[0].size();
        vector<float> output(size, 0);
        for(int index : indexes) { if(index < size && index >= 0) output[index] = 1; }
        return output;
    }
    
    int postprocess(vector<float> output) // charges > index | gets index of max charge in a vector
    {
        int len = output.size();
        int maxIndex = 0;
        for(int i = 0; i < len; i++) { if(output[i] > output[maxIndex]) { maxIndex = i; } }
        return maxIndex;
    }
};