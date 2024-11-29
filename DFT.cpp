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

        float PowerK = ValueSine * ValueSine + ValueCosine * ValueCosine;
        PowerSpectrum.push_back(PowerK);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;

    std::ofstream outputfile("SineWave_1Hz.txt");
    int vsize = PowerSpectrum.size();
    for (int n = 0; n < vsize; n++)
    {
        outputfile << PowerSpectrum[n] << std::endl;
    }

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

    auto duration_fftw = std::chrono::duration_cast<std::chrono::microseconds>(stop_fftw - start_fftw);
    std::cout << duration_fftw.count() << std::endl;
    fftw_free(out);
}

