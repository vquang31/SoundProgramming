#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include "DigitalSignal.hpp"
#include "AudioWav.hpp"
#include "DigitalSignal.cpp"
#include "AudioWav.cpp"
using namespace std;

int main()
{
    vector<short> data = {262, 392};
    // Khởi tạo đối tượng
    DigitalSignal dS1(data, 44100, 16, 1, 1, DigitalSignal::TYPE_FREQUENCY);
    AudioWav aW1(dS1);

    // Xuất data trước khi Up
    aW1.exportDataToFile("SignalBeforeTimeShifting_TC4.dat");

    //
    // dịch trái
    AudioWav aW2 = aW1.timeShifting(-0.5f);
    // dịch phải
    AudioWav aW3 = aW1.timeShifting(0.5f);

    // Xuất data sau khi up
    aW2.exportDataToFile("Signal_1_TimeShifting_TC4.dat");
    aW3.exportDataToFile("Signal_2_TimeShifting_TC4.dat");
    // Xuất file WavWav
    aW2.exportToWavFile("Signal_1_TC4.wav");
    aW3.exportToWavFile("Signal_2_TC4.wav");

    getchar();
}