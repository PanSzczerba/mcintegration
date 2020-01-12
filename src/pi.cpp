#include <functional>
#include <iostream>
#include <cmath>
#include <limits>
#include "mc_integration.h"
using namespace std;

double f1(double x) {
    return sqrt(1 - x*x);
}

double f2(double x) {
    return -sqrt(1 - x*x);
}

void print_usage() {
    cout<< "pi ITERATIONS" << endl;
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
    constexpr double a = -1;
    constexpr double b = 1;
    std::array<std::pair<double,double>,1> range = {std::pair<double,double>(a,b)};

    cout.precision(numeric_limits<double>::max_digits10);
    cout << mc_integration_mth(function<double(double)>(f1), range, iters/2) -
            mc_integration_mth(function<double(double)>(f2), range, iters/2) 
         << endl;
}

