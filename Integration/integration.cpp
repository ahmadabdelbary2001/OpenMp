#include <iostream>
#include <cmath>
#include <omp.h>
#include <vector>
#include <stdexcept>

// Function to calculate f(x) = ln(x) / x
double f(double x) {
    return log(x) / x;
}

// Function to perform numerical integration using the Riemann sum with OpenMP
double calculate_area(double a, double b, int n, int num_threads) {
    if (a <= 0 || b <= 0 || a >= b || n <= 0) {
        throw std::invalid_argument("Invalid input: Ensure a > 0, b > a, and n > 0");
    }

    double h = (b - a) / n;
    double area_sum = 0.0;

    // Use OpenMP to parallelize the loop with static scheduling for balanced workload
    #pragma omp parallel for reduction(+:area_sum) num_threads(num_threads) schedule(static)
    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        area_sum += f(x) * h;
    }

    return area_sum;
}

int main() {
    // Integration parameters
    const double a = 1.0;
    const double b = 10.0;
    const int n = 1000000;  // Number of intervals
    std::vector<int> threadCounts = {1, 2, 4};  // Different thread counts to test

    std::cout << "Numerical Integration of ln(x)/x from " << a << " to " << b << "\n";
    std::cout << "Number of intervals: " << n << "\n\n";

    // Perform integration for each thread count
    for (int num_threads : threadCounts) {
        double start = omp_get_wtime();
        double result = calculate_area(a, b, n, num_threads);
        double end = omp_get_wtime();

        double elapsed = end - start;

        std::cout << "Threads: " << num_threads << "\n";
        std::cout << "  Result: " << result << "\n";
        std::cout << "  Calculation time: " << elapsed << " seconds\n\n";
    }

    return 0;
}
