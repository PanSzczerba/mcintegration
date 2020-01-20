#include <functional>
#include <iostream>
#include <chrono>
#include "mc_integration.h"

using namespace std;

double fn(double x, double y) {
    return 3 * x + y;
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
    constexpr double a = 1;
    constexpr double b = 3;
    constexpr double c = 0;
    constexpr double d = 1;
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    cout<<"MC integration for 3x + y in (" << a << "," << b << ") * (" << c << "," << d << ") = " <<
        mc_integration_mth(function<double(double, double)>(fn),
                {pair<double, double>(a,b), pair<double, double>(c,d)}, iters) << endl;
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    cout << "Elapsed time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;

}
