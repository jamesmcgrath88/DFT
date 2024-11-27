// DFT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <fftw3.h>
#include <complex.h>
#include <chrono>
#include <algorithm>

#define PI 3.14159265f

std::vector<std::string> splitCSVFileLine(std::string& line)
{
    std::vector<std::string> values;
    std::string value;
    size_t pos = 0;
    while ((pos = line.find(",")) != std::string::npos)
    {
        value = line.substr(0, pos);
        values.push_back(value);
        line.erase(0, pos + 1);
    }
    values.push_back(line);

    return values;
}

int main()
{
    std::cout << "DFT!\n";

    std::ifstream inputCSVFile("C:\\SoftwareDevelopment\\Tools\\DFT\\DFT\\Data\\SineWave_1Hz.csv");

    std::vector<float> timeStamps;
    std::vector<float> signalAmplitude;

    std::string line;
    while (std::getline(inputCSVFile, line))
    {
        std::vector<std::string> lineValues = splitCSVFileLine(line);

        timeStamps.push_back(std::stof(lineValues[0]));
        signalAmplitude.push_back(std::stof(lineValues[1]));
    }

    int N = signalAmplitude.size();
    std::vector<float> PowerSpectrum;
    auto start = std::chrono::high_resolution_clock::now();
    for (int k = 0; k < N; k++)
    {
        float ValueSine = 0.0f;
        float ValueCosine = 0.0f;
        for (int n = 0; n < N; n++)
        {
            ValueSine -= signalAmplitude[n] * sin((2.0f * PI * (float)k * (float)n) / (float)N);
            ValueCosine += signalAmplitude[n] * cos((2.0f * PI * (float)k * (float)n) / (float)N);
        }

        float PowerK = ValueSine * ValueSine + ValueCosine *ValueCosine;
        PowerSpectrum.push_back(PowerK);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // To get the value of duration use the count()
    // member function on the duration object
    std::cout << duration.count() << std::endl;

    /*for (int i = 0; i < N; i++)
    {
        std::cout << PowerSpectrum[i] << std::endl;
    }*/

    fftw_complex * out;
    fftw_plan p;
    double* fftwIn = new double[N];

    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

    for (int i = 0; i < N; i++)
    {
        fftwIn[i] = signalAmplitude[i];
    }
    auto start_fftw = std::chrono::high_resolution_clock::now();
    p = fftw_plan_dft_r2c_1d(N, fftwIn, out, FFTW_ESTIMATE);

    fftw_execute(p); /* repeat as needed */
    auto stop_fftw = std::chrono::high_resolution_clock::now();
    fftw_destroy_plan(p);
    
    /*for (int i = 0; i < N; i++)
    {
        std::cout << "[" << i << "]" << " " << *out[i] << std::endl;
    }*/

    auto duration_fftw = std::chrono::duration_cast<std::chrono::microseconds>(stop_fftw - start_fftw);
    std::cout << duration_fftw.count() << std::endl;
    fftw_free(out);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
