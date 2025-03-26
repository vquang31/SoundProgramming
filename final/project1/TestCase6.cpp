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
    vector<short> data = {262};
    // Khởi tạo đối tượng
    DigitalSignal dS1(data, 8000, 16, 1, 1, DigitalSignal::TYPE_FREQUENCY);
    AudioWav aW1(dS1);

    // Xuất data trước khi Up
    aW1.exportDataToFile("SignalBeforeDOWN_TC6.dat");

    // down sampling
    aW1 = aW1.downSampling(2);

    // In thông tin
    aW1.printInfo();

    // Xuất data sau khi up
    aW1.exportDataToFile("SignalAfterDOWN_TC6.dat");

    // Xuất file WavWav
    aW1.exportToWavFile("Signal_TC6.wav");

    getchar();
}