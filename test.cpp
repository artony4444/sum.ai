
#include <iostream>
#include <vector>
using namespace std;

struct s
{
    int a = 10;
    int b = 20;
};

int main()
{
    vector<int> a{10, 20, 30};
    vector<int>* b = &a;
    (*b)[0] = 20;
    cout << a[0];
    return 0;
}

/*
int s(int size[])
{
    return size[0];
}

int main()
{
    // int* a = new int[]{10,20};
    auto a = *new vector<int>{10, 20};
    
    for(int b : a)
    {
        cout << a[0] << " ";
    }
    
    vector<int> v = {10, 20};
    v.push_back(10);
    v.insert(v.begin(), 1);
    printVector<int>(v);
    return 0;
    
}


template <typename T> void printVector(vector<T> v)
{
    for(int i = 0; v.size() > i; i++)
    {
        cout << v[i] << " ";
    }
}
*/