#include <iostream>
#include <random>
#include <chrono>

using namespace std;

double fn(double x) {
    return 2*x;
}

using f_ptr = double (*) (double);

double mc_integration(f_ptr fn, pair<double, double> range , size_t iters) {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(range.first, range.second);

    double prob_density = 1 / (range.second - range.first);

    double average = 0;
    size_t average_count = 0;
    for(size_t i = 0; i < iters; i++) {
        double next_value = fn(distribution(generator)) / prob_density;

        average_count++;
        average += (next_value- average) / average_count; 
    }

    return average;
}

int main() {
    constexpr double a = -1;
    constexpr double b = 3;
    constexpr size_t iters = 100000;

    cout<<"MC integration for 2x in (" << a << "," << b << ") = " <<
        mc_integration(fn, pair<double, double>(a, b), iters) << endl;
}
