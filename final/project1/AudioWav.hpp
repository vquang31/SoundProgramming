#pragma once
#include "DigitalSignal.hpp"

class AudioWav
{
private:
    // number Channel = 1
    vector<short> data_1Channel;
    DigitalSignal digitalSignal_1Channel;

    // numberChannel = 2
    vector<std::pair<short, short>> data_2Channel;
    pair<DigitalSignal, DigitalSignal> digitalSignal_2Channel;

    char type[5];
    char format[5];
    char subchunk1ID[5];
    char subchunk2ID[5];

    int chunkSize;
    int subchunk1Size = 16;
    int sampleRate; //(*) = digitalSinal.sampleRate = frequency
    int byteRate;
    int subchunk2Size; //  = totalSamples * NumChannels * (BitsPerSample/8)

    int audioFormat = 1;
    int numberChannels; // (*)
    int blockAlign;
    int bitsPerSample; //(*) bitDepth

    long long totalSamples; //(*) = Subchunk2Size / numberChannels / (bitsPerSample / 8);
                            //    = duration * sampleRate

protected:
    // common method
    void writeToFile(ofstream &file, int value, int size)
    {
        file.write(reinterpret_cast<const char *>(&value), size);
    }

    void loadCommonData();

    void loadDataFromDigitalSignal();

public:
    // Overloaded Operators
    std::pair<short, short> &operator[](unsigned i) { return data_2Channel[i]; }

    // constructor
    AudioWav() {};
    AudioWav(string s);         // NOTE(4) Đọc dữ liệu âm thanh từ file âm thanh thực tế
    AudioWav(DigitalSignal dS); // Load theo đối tượng tín hiệu số
    AudioWav(DigitalSignal dS1, DigitalSignal dS2);

    void loadWavFile(string s); // load Data file Wav;

    void exportToWavFile(string s); // ghi ra FileFile

    void exportDataToFile(string s); // xuất dữ liệu ra File
    
    void printInfo();
    //
    void setSampleRate(int newSampleRate)
    {
        this->sampleRate = newSampleRate;
        byteRate = sampleRate * numberChannels * bitsPerSample / 8;
    }

    // phép toán

    // viết thêm các lưu ý, các ngoại lệ (exception cho từng phép toán)
    // khi thực hiện các phép toán phải đảm bảo giống nhau về các thuộc tính
    // ví dụ:
    // mixSignal: phép cộng tín hiệu
    // NOTE: khi cộng 2 tín hiệu có thể lớn hơn maxAmplitupe
    //
    AudioWav mixSignal(AudioWav oAW, double ratioSelf, double ratioSecond);

    AudioWav multibleSignal(AudioWav oAW);

    AudioWav multibleSignal(double ratio1, double ratio2);

    AudioWav inverseSignal();

    AudioWav timeShifting(double time1);

    AudioWav downSampling(int factor);

    AudioWav upSampling(int factor);

    //


};