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
    // khởi tạo đối tượng AudioWav
    AudioWav aW1("MixSignal_TC1_Stereo.wav");
    //
    aW1 = aW1.multibleSignal(0, 1);
    // xuất file âm thanh
    aW1.exportToWavFile("Signal_TC2.wav");

    // xuất file dữ liệu
    aW1.exportDataToFile("Signal_TC2.dat");

    aW1.printInfo();
    cout << "Done!!!!";
    getchar();
}