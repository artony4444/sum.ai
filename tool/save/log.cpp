/*
DEBUG

Log log{true};
bool debug = true;

class Log
{
    public:
    
    bool isActive = false;
    Log(bool d) { isActive = d;}
    
    void parameter(float p, string s)
    {
        if(isActive) cout << s+" :" << p << endl; 
    }
    
    void charge(string name, float c)
    {
        if(isActive) cout << name << " (+) " << c << endl; 
    }
    
    void discharge(string name, float c)
    {
        if(isActive) cout << name << " (-) " << c << endl; 
    }
    
    void fire(string from, string to, float c)
    {
        if(isActive) cout << "("+from << "->" << to+") " << c << endl; 
    }
    
    log.parameter(bias, "b");
    log.charge(name, c);
    log.discharge(name, c);
    from->log.parameter(weight, "w");
    from->log.fire(from->name, to->name, f*weight);
};

if(debug) cout << "b :" << bias << endl;
if(debug) cout << name << " (+) " << c << endl;
if(debug) cout << name << " (-) " << c << endl;

if(from->debug) cout << "w :" << weight << endl;
if(from->debug) cout << "("+from->name << "->" << to->name+") " << f*weight << endl;


*/


