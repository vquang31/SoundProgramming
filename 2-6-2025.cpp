#include <bits/stdc++.h>

using namespace std;
int numberChannel = 1;
const int bitDepth = 16; // max == 16 /// quyết định Biên độ  //BitsPerSample
double PI = atan(1) * 4;
double a = 1;
int s = 8;

void writeToFile(ofstream &file, int value, int size)
{
    file.write(reinterpret_cast<const char *>(&value), size);
}

int main()
{
    int sampleRate = s * 1000; //
    std::ofstream audioFile;
    audioFile.open("Audio" + to_string(s) + "K" + to_string(numberChannel) + "C" + ".wav", std::ios::binary);
    double f[8] = {262, 294, 330, 349, 392, 440, 494, 523};
    // double f[8] = {10, 11, 12, 12, 13, 13, 14, 14};
    double duration = 8;

    ofstream oFile("Data" + to_string(s) + "K" + to_string(numberChannel) + "C" + ".dat");

    int y = 0;

    double Ts = 1 / (double)sampleRate;

    //////
    // Header chunk
    char type[5] = "RIFF";
    string s = type;
    audioFile << type;
    // audioFile << "RIFF";
    audioFile << "----";
    audioFile << "WAVE";

    // Format chunk
    audioFile << "fmt ";
    writeToFile(audioFile, 16, 4);                                        // Subchunk1Size Size
    writeToFile(audioFile, 1, 2);                                         // Compression code
    writeToFile(audioFile, numberChannel, 2);                             // Number of channels
    writeToFile(audioFile, sampleRate, 4);                                // Sample rate
    writeToFile(audioFile, sampleRate * numberChannel * bitDepth / 8, 4); // Byte rate
    writeToFile(audioFile, bitDepth * numberChannel / 8, 2);              // Block align
    writeToFile(audioFile, bitDepth, 2);                                  // Bit depth

    // Data chunk
    audioFile << "data";
    // writeToFile(audioFile, duration * sampleRate * numberChannel * bitDepth / 8, 4);
    audioFile << "----";

    ///////

    int preAudioPosition = audioFile.tellp();

    auto maxAmplitude = pow(2, bitDepth - 1) - 1;

    for (long long i = 0; i < duration * sampleRate * numberChannel; i++)
    {
        double tmp = a * sin(2 * PI * f[y] * i * Ts);
        // double tmp = a * sin(2 * PI * 5000 * i * Ts);

        if (i % 2 == 0)
        {
            // tmp = 0;
        }
        int amplitude = tmp * maxAmplitude;
        oFile << amplitude << endl;
        //

        if ((y + 1) * sampleRate * numberChannel < i)
            y++;
        int intSample = static_cast<int>(amplitude);
        writeToFile(audioFile, intSample, 2);
    }
    int postAudioPosition = audioFile.tellp();

    audioFile.seekp(preAudioPosition - 4);
    writeToFile(audioFile, (postAudioPosition - preAudioPosition), 4);

    audioFile.seekp(4, ios::beg);
    writeToFile(audioFile, postAudioPosition - 8, 4);

    audioFile.close();
    oFile.close();
    return 0;
}
// Source
// https://github.com/Thrifleganger/audio-programming-youtube/blob/master/wav-file-format/main.cpp
// https://www.youtube.com/watch?v=qqjvB_VxMRM
// http://soundfile.sapp.org/doc/WaveFormat/