#include <iostream>
#include "mc_integration.h"

using namespace std;

double fn(double x) {
    return 3 * x;
}

int main() {
    constexpr double a = 0;
    constexpr double b = 3;
    constexpr size_t iters = 100000;

    cout<<"MC integration for 2x in (" << a << "," << b << ") = " <<
        mc_integration(std::function<double(double)>(fn), {pair<double, double>(a, b)}, iters) << endl;
}
