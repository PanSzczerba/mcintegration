#include <array>
#include <iostream>
#include <future>
#include <random>
#include <chrono>
#include <functional>
#include <omp.h>

template<bool...> struct bool_pack{};
template<class... Ts>
using conjunction = std::is_same<bool_pack<true,Ts::value...>, bool_pack<Ts::value..., true>>;

template<typename... Ts>
using AllDouble = typename std::enable_if<conjunction<std::is_convertible<Ts, double>...>::value>::type;

template<int arg_count, typename GenR, typename F, typename... Args>
struct Call_N {
    static inline typename F::result_type call(std::function<GenR(int)> generator, F fn, Args... args) {
        return Call_N<arg_count - 1, GenR, F, GenR, Args...>::call(generator, fn, generator(arg_count - 1), args...);
    }
};

template<typename GenR, typename F, typename... Args>
struct Call_N<0, GenR, F, Args...> {
    static inline typename F::result_type call(std::function<GenR(int)>, F fn, Args... args) {
        return fn(args...);
    }
};

template<typename... Args, typename = AllDouble<Args...>>
double mc_integration(std::function<double(Args...)> fn, std::array<std::pair<double, double>, sizeof...(Args)> ranges , size_t iters) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() + (unsigned)omp_get_thread_num();
    std::default_random_engine generator(seed);
    std::array<std::uniform_real_distribution<double>, sizeof...(Args)> distributions;

    // initialize distributions and calulate volume of integration
    double volume = 1;
    for(size_t i = 0; i < sizeof...(Args); i++) {
        distributions[i] = std::uniform_real_distribution<double>(ranges[i].first, ranges[i].second);
        volume *= (ranges[i].second - ranges[i].first);
    }

    double average = 0;
    size_t average_count = 0;
    // generate samples and calculate average
    for(size_t i = 0; i < iters; i++) {
        double next_value = Call_N<sizeof...(Args), double, std::function<double(Args...)>>::
            call([&](int i) { return distributions[i](generator); }, fn);

        average_count++;
        average += (next_value - average) / average_count; 
    }

    return average * volume;
}

template<typename... Args, typename = AllDouble<Args...>>
std::pair<double, double> mc_integration_var(std::function<double(Args...)> fn, std::array<std::pair<double, double>, sizeof...(Args)> ranges, size_t iters) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() + (unsigned)omp_get_thread_num();
    std::default_random_engine generator(seed);
    std::array<std::uniform_real_distribution<double>, sizeof...(Args)> distributions;

    // initialize distributions and calulate volume of integration
    double volume = 1;
    for(size_t i = 0; i < sizeof...(Args); i++) {
        distributions[i] = std::uniform_real_distribution<double>(ranges[i].first, ranges[i].second);
        volume *= (ranges[i].second - ranges[i].first);
    }

    double average = 0;
    double average_square = 0;
    size_t average_count = 0;
    // generate samples and calculate average
    for(size_t i = 0; i < iters; i++) {
        double next_value = Call_N<sizeof...(Args), double, std::function<double(Args...)>>::
            call([&](int i) { return distributions[i](generator); }, fn);

        average_count++;
        average += (next_value - average) / average_count; 
        average_square += (next_value * next_value - average_square) / average_count;
    }

    double expected_value = average * volume;
    double variance = average_square * volume * volume - expected_value * expected_value;
    return std::pair<double, double>(expected_value, variance);
}


template<typename... Args, typename = AllDouble<Args...>>
size_t estimate_min_var_split(std::function<double(Args...)> fn, std::array<std::pair<double, double>, sizeof...(Args)> ranges, size_t estimate_iters) {
    
    int thread_num = omp_get_max_threads();
    size_t iters_per_thread = estimate_iters / (size_t)thread_num;
    double min_variance = 0;
    auto ranges_copy = ranges;
    double a = ranges[0].first;
    double b = ranges[0].second;
    double span = b - a;
#pragma omp parallel for firstprivate(ranges_copy) reduction(+:min_variance)
    for(int i = 0; i < thread_num; i++) {
        ranges_copy[0] = std::pair<double, double>(a + (i * span) / thread_num,
                a + ((i + 1) * span) / thread_num);
        auto ev_var = mc_integration_var(fn, ranges_copy, iters_per_thread);
        min_variance = ev_var.second;
    }
    size_t best_split_index = 0;

    for(size_t i = 1; i < sizeof...(Args); i++) {
        ranges_copy = ranges;
        a = ranges[i].first;
        b = ranges[i].second;
        span = b - a;
        double variance;
#pragma omp parallel for firstprivate(ranges_copy) reduction(+:variance)
        for(int j = 0; j < thread_num; j++) {
            ranges_copy[i] = std::pair<double, double>(a + (j * span) / thread_num,
                    a + ((j + 1) * span) / thread_num);
            auto ev_var = mc_integration_var(fn, ranges_copy, iters_per_thread);
            variance = ev_var.first;
        }

        if (variance < min_variance) {
            min_variance = variance;
            best_split_index = i;
        }
    }
    return best_split_index;
}

template<typename... Args, typename = AllDouble<Args...>>
double mc_integration_mth(std::function<double(Args...)> fn, std::array<std::pair<double, double>, sizeof...(Args)> ranges, size_t iters, size_t estimate_iters=1000) {
    
    int thread_num = omp_get_max_threads();
    size_t iters_per_thread = iters / (size_t)thread_num;
    size_t best_split_index = 0;
    if(sizeof...(Args) > 1) {
        best_split_index = estimate_min_var_split(fn, ranges, estimate_iters);
    }

    double result = 0;
    double a = ranges[best_split_index].first;
    double b = ranges[best_split_index].second;
    double span = b - a;
    auto ranges_copy = ranges;
#pragma omp parallel for firstprivate(ranges_copy) reduction(+:result)
    for(int i = 0; i < thread_num; i++) {
        ranges_copy[best_split_index] = std::pair<double, double>(a + (i * span) / thread_num,
                a + ((i + 1) * span) / thread_num);
        result = mc_integration(fn, ranges_copy, iters_per_thread);
    }

    return result;
}


