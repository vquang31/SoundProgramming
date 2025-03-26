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
    vector<short> data = {262, 1011};
    // Khởi tạo đối tượng
    DigitalSignal dS1(data, 8000, 16, 1, 1, DigitalSignal::TYPE_FREQUENCY);

    DigitalSignal dS2 = dS1.inverseSignal();

    dS1.exportData("Signal_BeforeInverse_TC3.dat");
    dS2.exportData("Signal_AfterInverse_TC3.dat");

    AudioWav aW1(dS1);
    AudioWav aW2(dS2);

    aW1.exportToWavFile("Signal_BeforeInverse_TC3.wav");
    aW2.exportToWavFile("Signal_AfterInverse_TC3.wav");
    getchar();
}