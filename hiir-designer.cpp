#include <array>
#include <vector>
#include <iostream>
#include "hiir/PolyphaseIir2Designer.h"
#include <limits>
#include <string>

void print_usage(char** argv)
{
    std::cerr << "Usage: " << argv[0] << " n_stages transition_bw [stopband_attenuation]" << '\n';
    std::cerr << "n_stages is the number of 2x up/down sampling stages" << '\n';
    std::cerr << "transition_bw is the transition bandwidth of the first stage" << '\n';
    std::cerr << "stopband_attenuation is the expected stopband attenuation" << '\n';
}

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 4) {
        print_usage(argv);
        return -1;   
    }

    int n_stages { 0 };
    double transition_bw { 0.01 };
    double stopband_attenuation { 96 };

    try {
        n_stages = std::stoi(argv[1]);
        transition_bw = std::stod(argv[2]);
        if (argc == 4)
            stopband_attenuation = std::stod(argv[3]);
    } catch (const std::exception& e [[maybe_unused]]) {
        print_usage(argv);
        return -1;
    }

    if (n_stages < 2) {
        std::cerr << "The number of stages should be greater than 2 (" << n_stages << ")" << '\n';
        return -1;
    }

    if (transition_bw <= 0.0 || transition_bw >= 0.5) {
        std::cerr << "The transition bandwidth should be between 0 and 0.5 (" << transition_bw << ")" << '\n';
        return -1;
    }

    if (stopband_attenuation <= 0.0) {
        std::cerr << "The stopband attenuation should be positive (" << stopband_attenuation << ")" << '\n';
        return -1;
    }

    // Set the precision printing to the double limits
    std::cout.precision(std::numeric_limits<double>::max_digits10);

    std::cout << "Number of x2 stages: " << n_stages << '\n';
    std::cout << "Stopband attenuation: " << stopband_attenuation << " dB" << '\n';
    std::array<double, 1024> iir_coeffs;
    for (int i = 0; i < n_stages; ++i) {
        auto numCoeffs = hiir::PolyphaseIir2Designer::compute_coefs(iir_coeffs.data(), stopband_attenuation, transition_bw);
        if (numCoeffs > iir_coeffs.size()) {
            std::cerr << "Oops, the number of indices of the filter overflowed (" << numCoeffs << ")" << '\n';
            return -1;
        }
        std::cout << "Coefficients for phase " << (i+1) << " with transition bandwidth " << transition_bw << " (Total " << numCoeffs << " coefficients)" << '\n';
        std::cout << "{ ";
        for (int coeff_idx = 0; coeff_idx < numCoeffs; coeff_idx++)
            std::cout << iir_coeffs[coeff_idx] << ' ';
        std::cout << "}" << '\n';
        transition_bw += 0.5;
        transition_bw /= 2.0;
    }
}