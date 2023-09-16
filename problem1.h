#include <cmath>
#include <iostream>
#include <tuple>

template <typename F>
double trapezoidal_next(const F& f, double a, double b, double dx, double Sn) {
    double sum = 0;
    for (double x = a + dx; x < b; x += 2*dx) {
        sum += f(x);
    }
    return 0.5 * Sn + dx * sum;
}

template <typename F>
std::tuple<bool, double> adaptive_simpson(const F& f, double a, double b, double tolerance) {
    double slast = 0.5 * (f(a) - f(b)) * (b - a); // Use the average value of f(a) and f(b)
    double sthis = trapezoidal_next(f, a, b, (b - a) / 2.0, slast);
    double spslast = 0;
    double spsthis = (4.0 / 3.0) * sthis - (1.0 / 3.0) * spslast;
    double error = std::abs(spsthis - spslast) / 15.0;
    int ithis = 2;

    while (std::abs(error*10) > tolerance) { // Simplify the termination condition
        ithis++;
        //std::cout<<ithis<<" "<<error<<"\n";
        if (std::isnan(spsthis)) {
            return std::make_tuple(false, spsthis);
        }
        slast = sthis;
        sthis =  trapezoidal_next(f, a, b, (b - a) / std::pow(2, ithis), sthis);
        spslast = spsthis;
        spsthis = (4.0 / 3.0) * sthis - (1.0 / 3.0) * slast;
        
        error = std::abs(spsthis - spslast) / 15.0;
    }

    return std::make_tuple(true, spsthis);
}
