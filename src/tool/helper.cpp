
void printInput(vector<float> input)
{
    int index = 0;
    
    for(float i : input)
    {
        if(index % (int) sqrt(input.size()) == 0) cout << "|" << endl; index++;
        cout << (i > 0 ? "■" : "□");
    }
    cout << endl << endl << endl;
}

vector<float> getWsum(model m)
{
    vector<float> val;
    for(neuron n : m.structure[0])
    {
        float sum = 0;
        for(neuron::path* p : n.pathOut)
        {
            sum += p->weight;
            
            for(neuron::path* p2 : p->to->pathOut)
            {
                sum += p2->weight;
            }
        }
        val.push_back(sum);
    }
    return val;
}

vector<float> getWsum(cnn m)
{
    vector<float> val;
    for(vector<cnn_neuron> ns : m.structure[1])
    {
        for(cnn_neuron n : ns)
        {
            float sum = n.filter;
            for(cnn_neuron::path* p : n.pathOut)
            {
                // sum += p->weight;
                // sum += p->from->filter;
            }
            val.push_back(sum);
        }
    }
    return val;
}