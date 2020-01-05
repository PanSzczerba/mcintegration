#include <iostream>
#include <random>
#include <chrono>
#include <functional>

template<bool...> struct bool_pack{};
template<class... Ts>
using conjunction = std::is_same<bool_pack<true,Ts::value...>, bool_pack<Ts::value..., true>>;

template<typename... Ts>
using AllDouble = typename std::enable_if<conjunction<std::is_convertible<Ts, double>...>::value>::type;

template<int arg_count, typename GenR, typename F, typename... Args>
struct Call_N {
    static typename F::result_type call(std::function<GenR(int)> generator, F fn, Args... args) {
        return Call_N<arg_count - 1, GenR, F, GenR, Args...>::call(generator, fn, args..., generator(arg_count - 1));
    }
};

template<typename GenR, typename F, typename... Args>
struct Call_N<0, GenR, F, Args...> {
    static typename F::result_type call(std::function<GenR(int)> generator, F fn, Args... args) {
        return fn(args...);
    }
};

template<typename... Args, typename = AllDouble<Args...>>
double mc_integration(std::function<double(Args...)> fn, std::array<std::pair<double, double>, sizeof...(Args)> ranges , size_t iters) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::array<std::uniform_real_distribution<double>, sizeof...(Args)> distributions;

    double prob_density = 0;
    bool first_iter = true;
    for(int i = 0; i < sizeof...(Args); i++) {
        distributions[i] = std::uniform_real_distribution<double>(ranges[i].first, ranges[i].second);
        if(first_iter) {
            prob_density = 1.0 / (ranges[i].second - ranges[i].first);
            first_iter = false;
        } else {
            prob_density *= 1.0 / (ranges[i].second - ranges[i].first);
        }
    }

    double average = 0;
    size_t average_count = 0;
    for(size_t i = 0; i < iters; i++) {
        double next_value = Call_N<sizeof...(Args), double, std::function<double(Args...)>>::
            call([&](int i) { return distributions[i](generator); }, fn)  / prob_density;

        average_count++;
        average += (next_value- average) / average_count; 
    }

    return average;
}
