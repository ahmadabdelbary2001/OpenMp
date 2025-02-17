# OpenMP Parallel Computing Projects

![OpenMP Logo](https://www.openmp.org/wp-content/uploads/OpenMP_300x150.png)  
Two parallel computing implementations demonstrating OpenMP capabilities for numerical integration and fractal analysis.

## Project Overview

Contains two distinct OpenMP-accelerated programs:
1. **Numerical Integration** - Riemann sum calculation of ∫(ln(x)/x)dx from 1 to 10
2. **Mandelbrot Set Analysis** - Monte Carlo estimation of Mandelbrot set area

## Features

### Integration Program (`integration.cpp`)
- OpenMP-parallelized Riemann sum calculation
- Multiple thread count testing (1, 2, 4 threads)
- Runtime performance comparison
- Input validation and error handling

### Mandelbrot Program (`mandelbrot.cpp`)
- Sequential vs parallel Monte Carlo comparison
- Dynamic thread configuration (1, 2, 4 threads)
- Speedup and efficiency calculations
- Random sampling with thread-local PRNGs
- Complex plane boundary configuration

## Requirements

- OpenMP-compatible compiler (GCC >= 9.0)
- C++17 standard library
- GNU Make
- (Optional) gprof for profiling

## Project Structure
