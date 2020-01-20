#include <functional>
#include <iostream>
#include <chrono>
#include "mc_integration.h"

using namespace std;

double fn(double x, double y, double v, double z) {
    return x + y + v + z;
}

void print_usage() {
    cout<< "time_measure ITERATIONS" << endl;
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
    double a = 0;
    double b = 1;
    auto base_range = pair<double, double>(a,b);
    std::array<pair<double, double>, 4> ranges;
    ranges.fill(base_range);
    
    cout<<"MC integration for x + y + v + z in (" << a << "," << b << ")^4 = " <<
        mc_integration_mth(function<double(double, double, double, double)>(fn),
                ranges , iters) << endl;
}
