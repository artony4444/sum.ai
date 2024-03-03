

// ----- helper ----- //

vector<int> toVector(int e) { return *new vector<int>{e}; }

struct vars
{
    inline static float plasticity = 1.0;
    inline static float biasPlasticity = 1.0;
};


vector<string> split(string str, string target)
{
    int targetSize = target.size();
    int strSize = str.size();
    
    vector<string> s;
    string line;
    
    for(int i = 0; i < strSize; i++)
    {
        if(str.substr(i, targetSize) == target)
        {
            s.push_back(line);
            line = "";
            i += targetSize-1;
        }
        else
        {
            line += str[i];
        }
    }
    
    s.push_back(line);
    return s;
}

vector<float> toFloats(vector<string> s)
{
    vector<float> o;
    
    for(string& str : s)
    {
        o.push_back(stof(str));
    }
    return o;
}

void print(vector<float> f)
{
    for(float fl : f)
    {
        cout << fl << "    ";
    }
    cout << endl;
}