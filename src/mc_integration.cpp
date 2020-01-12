#include <functional>
#include <iostream>
#include "mc_integration.h"

using namespace std;

double fn(double x, double y) {
    return 3 * x + y;
}

void print_usage() {
    cout<< "mc_integration ITERATIONS" << endl;
}

int main(int argc, char** argv) {
    if(argc < 2)
    {
        cerr<<"Error: too few arguments"<<endl;
        print_usage();
        return 1;
    }
    
    size_t iters;
    try
    {
        iters = stoull(argv[1]);
    } catch (invalid_argument& e)
    {
        cerr<<"Error: invalid input"<<endl;
        print_usage();
        return 1;
    }
    constexpr double a = 1;
    constexpr double b = 3;

    cout<<"MC integration for 2x in (" << a << "," << b << ") = " <<
        mc_integration_mth(function<double(double, double)>(fn),
                {pair<double, double>(a,b), pair<double, double>(0.0,1.0)}, iters) << endl;

}
