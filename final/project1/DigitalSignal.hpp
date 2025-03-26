#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <fstream>

using namespace std;

class DigitalSignal
{
private:
    // note
    std::vector<int> data;

    // tự định nghĩa
    int sampleRate = 0; // frequency
    int bitDepth = 0;   // bitPerSample
    int numberChannels = 0;

    // nội suy
    long long totalSamples = 0; //  = data.size()
    int maxAmplitude = 0;       //  = 2^(bitDep-1) - 1
    double duration = 0;        //  = totalSample / sampleRate / numberChannels
public:
    const static int TYPE_FREQUENCY = 1;
    const static int TYPE_AMPLITUDE = 2;
    const static int TYPE_RATIO = 3;

    DigitalSignal();

    /*

    */
    DigitalSignal(std::string s, int sampleRate, int bitDepth, int numberChannels, int type);

    ///
    /// khi truyền dữ liệu(data) tạo ra tín hiệu âm thanh theo 2 cách
    ///
    /*
        Cách 1: type == TYPE_FREQUENCY
            truyền vào MẢNG TẦN SỐ
            truyền các tham số cần thiết
            --> nhận được một đối tượng tín hiệu âm thanh số hoàn chỉnh
        Cách 2:
            truyền vào MẢNG DỮ LIỆU có kiểu là (int) đại diện cho biên đô của âm thanh
            truyền các tham số cần thiết
            --> nhận được một đối tượng tín hiệu âm thanh số hoàn chỉnh

    */
    DigitalSignal(std::vector<short> data, int sampleRate, int bitDepth, int numberChannels, double duration, int type);
    // setter

    void setSampleRate(int sampleRate);
    void setBitDepth(int bitDepth);
    void setNumberChannels(int numberChannels);
    void setData(int x)
    {
        data.push_back(x);
    }
    /// getter

    int getData(int index)
    {
        if (index >= data.size())
            return 0;
        return data[index];
    }
    int getSampleRate();
    int getBitDepth();
    int getMaxAmplitude();
    int getNumberChannels();
    long long getTotalSamples();
    double getDuration();

    ////

    void printSignalInfo();
    void printSignalData();

    void exportData(std::string s);

    ////

    DigitalSignal mixSignal(DigitalSignal ortherDS, double ratioSefl, double ratioSecond);

    DigitalSignal multipleSignal(DigitalSignal ortherDs);

    DigitalSignal multipleSignal(double ratio);

    DigitalSignal inverseSignal();

    DigitalSignal timeShifting(double time);

    DigitalSignal downSampling(int factor);

    DigitalSignal upSampling(int factor);
};
