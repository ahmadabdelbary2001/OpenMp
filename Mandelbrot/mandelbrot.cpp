#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <random>
#include <chrono>
#include <iostream>
#include <vector>
#include <stdexcept>

#define MAX_ITER 1000 // Max iterations for Mandelbrot

// Configuration for complex plane bounds
constexpr double REAL_MIN = -2.0;
constexpr double REAL_MAX = 1.5;
constexpr double IMAG_MIN = -1.0;
constexpr double IMAG_MAX = 1.0;
constexpr double SAMPLE_AREA = (REAL_MAX - REAL_MIN) * (IMAG_MAX - IMAG_MIN);

// Function to determine if a complex point is in the Mandelbrot set
bool isInMandelbrot(double real, double imag, int maxIter) {
    double zr = 0.0, zi = 0.0, zr2 = 0.0, zi2 = 0.0;

    for (int i = 0; i < maxIter; i++) {
        zi = 2.0 * zr * zi + imag;
        zr = zr2 - zi2 + real;
        zr2 = zr * zr;
        zi2 = zi * zi;

        if (zr2 + zi2 > 4.0) {
            return false;
        }
    }
    return true;
}

// Sequential Monte Carlo approximation of the Mandelbrot set area
double sequentialMonteCarlo(long long numPoints, int maxIter) {
    if (numPoints <= 0 || maxIter <= 0) {
        throw std::invalid_argument("Invalid input parameters");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(REAL_MIN, REAL_MAX);
    std::uniform_real_distribution<> disY(IMAG_MIN, IMAG_MAX);

    long long pointsInSet = 0;

    for (long long i = 0; i < numPoints; i++) {
        double x = disX(gen);
        double y = disY(gen);

        if (isInMandelbrot(x, y, maxIter)) {
            pointsInSet++;
        }
    }
    return SAMPLE_AREA * (static_cast<double>(pointsInSet) / numPoints);
}

// Parallel Monte Carlo approximation using OpenMP
double parallelMonteCarlo(long long numPoints, int maxIter, int numThreads) {
    if (numPoints <= 0 || maxIter <= 0 || numThreads <= 0) {
        throw std::invalid_argument("Invalid input parameters");
    }

    long long pointsInSet = 0;
#pragma omp parallel num_threads(numThreads) reduction(+:pointsInSet)
    {
        unsigned int seed = omp_get_thread_num();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> disX(REAL_MIN, REAL_MAX);
        std::uniform_real_distribution<> disY(IMAG_MIN, IMAG_MAX);

#pragma omp for
        for (long long i = 0; i < numPoints; i++) {
            double x = disX(gen);
            double y = disY(gen);

            if (isInMandelbrot(x, y, maxIter)) {
                pointsInSet++;
            }
        }
    }
    return SAMPLE_AREA * (static_cast<double>(pointsInSet) / numPoints);
}

int main() {
    constexpr long long numPoints = 100'000'000; // Number of points to sample
    constexpr int maxIter = MAX_ITER;
    std::vector<int> threadCounts = {1, 2, 4}; // Different thread counts to test

    std::cout << "Mandelbrot Set Area Approximation using Monte Carlo Method\n";
    std::cout << "Number of points: " << numPoints << "\n";
    std::cout << "Maximum iterations: " << maxIter << "\n\n";

    // Sequential execution
    auto start = std::chrono::high_resolution_clock::now();
    double seqArea = sequentialMonteCarlo(numPoints, maxIter);
    auto end = std::chrono::high_resolution_clock::now();
    double seqTime = std::chrono::duration<double>(end - start).count();

    std::cout << "Sequential Execution:\n";
    std::cout << "  Area: " << seqArea << "\n";
    std::cout << "  Time: " << seqTime << " seconds\n\n";

    // Parallel execution with different thread counts
    std::cout << "Parallel Execution Results:\n";
    for (int threads : threadCounts) {
        start = std::chrono::high_resolution_clock::now();
        double parArea = parallelMonteCarlo(numPoints, maxIter, threads);
        end = std::chrono::high_resolution_clock::now();
        double parTime = std::chrono::duration<double>(end - start).count();

        double speedup = seqTime / parTime;
        double efficiency = speedup / threads;

        std::cout << "Threads: " << threads << "\n";
        std::cout << "  Area: " << parArea << "\n";
        std::cout << "  Time: " << parTime << " seconds\n";
        std::cout << "  Speedup: " << speedup << "x\n";
        std::cout << "  Efficiency: " << efficiency << "\n\n";
    }

    return 0;
}
