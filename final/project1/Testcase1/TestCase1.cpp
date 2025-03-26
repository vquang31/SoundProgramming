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
    // Khởi tạo tần số âm thanh muốn phát
    vector<short> data1 = {262};
    vector<short> data2 = {1011};

    // Khởi tạo đối tượng tín hiệu
    DigitalSignal dS1(data1, 8000, 16, 1, (double)2, DigitalSignal::TYPE_FREQUENCY);
    // dS1.printSignalInfo();
    DigitalSignal dS2(data2, 8000, 16, 1, (double)2, DigitalSignal::TYPE_FREQUENCY);
    // mix đối tượng tín hiệu theo tỉ lệ 1:1
    DigitalSignal dS3 = dS1.mixSignal(dS2, 0.5, 0.5);

    // khởi tạo đối tượng AudioWav
    AudioWav aW1(dS1);
    AudioWav aW2(dS2);
    AudioWav aW3(dS3);

    // xuất file dữ liệu
    aW1.exportDataToFile("Signal1_TC1_Mono.dat");
    aW2.exportDataToFile("Signal2_TC1_Mono.dat");
    aW3.exportDataToFile("MixSignal_TC1_Mono.dat");

    // sử dụng phép nhân tín hiệu với hằng số 0.2 để GIẢM ÂM LƯỢNG của các file âm thanh
    aW1 = aW1.multibleSignal(0.2, 0.2);
    aW2 = aW2.multibleSignal(0.2, 0.2);
    aW3 = aW3.multibleSignal(0.2, 0.2);

    // xuất file âm thanh
    aW1.exportToWavFile("Signal1_TC1_Mono.wav");
    aW2.exportToWavFile("Signal2_TC1_Mono.wav");
    aW3.exportToWavFile("MixSignal_TC1_Mono.wav");

    // in ra info của file Mix
    aW3.printInfo();

    cout << "\nDone!!!!";

    getchar();
}