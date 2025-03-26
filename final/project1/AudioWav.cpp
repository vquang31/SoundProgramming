#include "AudioWav.hpp"
#include <cstring>
#include <bits/stdc++.h>
AudioWav::AudioWav(string s)
{
    loadWavFile(s);
    loadCommonData();
}

AudioWav::AudioWav(DigitalSignal dS)
{
    bitsPerSample = dS.getBitDepth();
    numberChannels = dS.getNumberChannels();
    sampleRate = dS.getSampleRate();
    byteRate = sampleRate * numberChannels * bitsPerSample / 8;
    totalSamples = dS.getTotalSamples();
    subchunk2Size = totalSamples * numberChannels * (bitsPerSample / 8);
    chunkSize = 36 + subchunk2Size;

    loadCommonData();

    if (numberChannels == 1)
    {
        digitalSignal_1Channel = dS;
        data_1Channel.clear();
        for (int i = 0; i < totalSamples; i++)
        {
            data_1Channel.push_back(digitalSignal_1Channel.getData(i));
        }
    }
    else
    {
        digitalSignal_2Channel.first = dS;
        digitalSignal_2Channel.second = dS;
        data_2Channel = vector<std::pair<short, short>>(totalSamples);
        for (int i = 0; i < totalSamples; i++)
        {
            data_2Channel[i].first = digitalSignal_2Channel.first.getData(i);
            data_2Channel[i].second = digitalSignal_2Channel.second.getData(i);
        }
    }
}

AudioWav::AudioWav(DigitalSignal dS1, DigitalSignal dS2)
{

    digitalSignal_2Channel.first = dS1;
    digitalSignal_2Channel.second = dS2;

    bitsPerSample = dS1.getBitDepth();
    numberChannels = 2; //
    sampleRate = dS1.getSampleRate();
    byteRate = sampleRate * numberChannels * bitsPerSample / 8;

    totalSamples = (dS1.getTotalSamples() >= dS2.getTotalSamples()) ? dS1.getTotalSamples() : dS2.getTotalSamples();
    subchunk2Size = totalSamples * numberChannels * (bitsPerSample / 8);
    chunkSize = 36 + subchunk2Size;

    loadDataFromDigitalSignal();
}

void AudioWav::loadCommonData()
{
    strcpy(type, "RIFF");
    strcpy(format, "WAVE");
    strcpy(subchunk1ID, "fmt ");
    strcpy(subchunk2ID, "data");
    subchunk1Size = 16;
    audioFormat = 1;
}

void AudioWav::loadDataFromDigitalSignal()
{
    loadCommonData();

    if (numberChannels == 1)
    {
        data_1Channel.clear();
        for (int i = 0; i < totalSamples; i++)
        {
            data_1Channel.push_back(digitalSignal_1Channel.getData(i));
        }
    }
    else
    {
        for (int i = 0; i < totalSamples; i++)
        {
            data_2Channel[i].first = digitalSignal_2Channel.first.getData(i);
            data_2Channel[i].second = digitalSignal_2Channel.second.getData(i);
        }
    }
}

void AudioWav::loadWavFile(string s)
{
    if (s.substr(s.size() - 4) != ".wav")
        throw std::invalid_argument("Can only read WAV files!");
    FILE *file;

    // Chunk
    type[4] = '\0';
    format[4] = '\0', subchunk1ID[4] = '\0', subchunk2ID[4] = '\0';
    fread(type, sizeof(char), 4, file);
    if (strcmp(type, "RIFF") != 0)
        throw std::runtime_error("Not a RIFF file!");

    fread(&chunkSize, sizeof(int), 1, file);
    fread(format, sizeof(char), 4, file);
    if (strcmp(format, "WAVE") != 0)
        throw std::runtime_error("Not a WAVE format!");

    // 1st Subchunk
    fread(subchunk1ID, sizeof(char), 4, file);
    if (strcmp(subchunk1ID, "fmt "))
        throw std::runtime_error("Missing fmt header!");
    fread(&subchunk1Size, sizeof(int), 1, file);
    fread(&audioFormat, sizeof(short), 1, file);
    fread(&numberChannels, sizeof(short), 1, file);
    fread(&sampleRate, sizeof(int), 1, file);
    fread(&byteRate, sizeof(int), 1, file);
    fread(&blockAlign, sizeof(short), 1, file);
    fread(&bitsPerSample, sizeof(short), 1, file);

    // 2nd Subchunk
    fread(subchunk2ID, sizeof(char), 4, file);
    if (strcmp(subchunk2ID, "data") != 0)
        throw std::runtime_error("Missing data header!");
    fread(&subchunk2Size, sizeof(int), 1, file);
    // data

    // Subchunk2Size = totalSamples * NumChannels * (BitsPerSample/8)
    totalSamples = subchunk2Size / numberChannels / (bitsPerSample / 8);

    if (numberChannels == 1)
    {
        data_1Channel = vector<short>(totalSamples);
        for (int i = 0; i < totalSamples; i++)
        {

            fread(&data_1Channel[i], sizeof(short), 1, file);
        }
        double duration = totalSamples / sampleRate / numberChannels;
        DigitalSignal tmp1(data_1Channel, sampleRate, bitsPerSample, duration, numberChannels, DigitalSignal::TYPE_AMPLITUDE);
        digitalSignal_1Channel = tmp1;
    }
    else
    {
        data_2Channel = vector<pair<short, short>>(totalSamples);
        vector<short> dataLeft = vector<short>(totalSamples);
        vector<short> dataRight = vector<short>(totalSamples);
        int count = 0;

        for (int i = 0; i < totalSamples; i++)
        {
            fread(&data_2Channel[i].first, sizeof(short), 1, file);
            fread(&data_2Channel[i].second, sizeof(short), 1, file);
            dataLeft[i] = data_2Channel[i].first;
            dataRight[i] = data_2Channel[i].second;
        }
        double duration = totalSamples / sampleRate / numberChannels;
        DigitalSignal tmp1(dataLeft, sampleRate, bitsPerSample, numberChannels, duration, DigitalSignal::TYPE_AMPLITUDE);
        DigitalSignal tmp2(dataRight, sampleRate, bitsPerSample, numberChannels, duration, DigitalSignal::TYPE_AMPLITUDE);
        digitalSignal_2Channel.first = tmp1;
        digitalSignal_2Channel.second = tmp2;
    }
    fclose(file);
}

void AudioWav::exportToWavFile(string s)
{
    ofstream audioFile;
    audioFile.open(s, std::ios::binary);
    audioFile << type;
    writeToFile(audioFile, chunkSize, 4);
    audioFile << format;
    audioFile << "fmt ";
    writeToFile(audioFile, subchunk1Size, 4);                                   // Subchunk1Size Size
    writeToFile(audioFile, 1, 2);                                               // Compression code
    writeToFile(audioFile, numberChannels, 2);                                  // Number of channels
    writeToFile(audioFile, sampleRate, 4);                                      // Sample rate
    writeToFile(audioFile, sampleRate * numberChannels * bitsPerSample / 8, 4); // Byte rate
    writeToFile(audioFile, bitsPerSample * numberChannels / 8, 2);              // Block align
    writeToFile(audioFile, bitsPerSample, 2);                                   // Bit depth
    audioFile << "data";
    writeToFile(audioFile, subchunk2Size, 4);

    if (numberChannels == 1)
    {
        for (int i = 0; i < data_1Channel.size(); i++)
        {
            writeToFile(audioFile, data_1Channel[i], 2);
        }
    }
    else
    {
        for (int i = 0; i < data_2Channel.size(); i++)
        {
            writeToFile(audioFile, data_2Channel[i].first, 2);
            writeToFile(audioFile, data_2Channel[i].second, 2);
        }
    }
    cout << "Export file WAV thanh cong" << endl;
    audioFile.close();
}

void AudioWav::exportDataToFile(string s)
{
    std::ofstream oFile(s);
    if (numberChannels == 1)
    {
        for (int i = 0; i < data_1Channel.size(); i++)
        {
            oFile << data_1Channel[i] << endl;
        }
    }
    else
    {
        for (int i = 0; i < data_2Channel.size(); i++)
        {
            oFile << data_2Channel[i].first << " " << data_2Channel[i].second << endl;
        }
    }
    cout << "Export data thanh cong" << endl;
    oFile.close();
}

AudioWav AudioWav::mixSignal(AudioWav oAW, double ratioSelf, double ratioSecond)
{
    AudioWav result;
    result = *this;
    if (numberChannels == 1)
    {
        result.digitalSignal_1Channel = result.digitalSignal_1Channel.mixSignal(oAW.digitalSignal_1Channel, ratioSelf, ratioSecond);
    }
    else
    {
        result.digitalSignal_2Channel.first = result.digitalSignal_2Channel.first.mixSignal(oAW.digitalSignal_2Channel.first, ratioSelf, ratioSecond);
        result.digitalSignal_2Channel.second = result.digitalSignal_2Channel.second.mixSignal(oAW.digitalSignal_2Channel.second, ratioSelf, ratioSecond);
    }
    result.loadDataFromDigitalSignal();

    return result;
}

AudioWav AudioWav::multibleSignal(AudioWav oAW)
{
    AudioWav result;
    result = *this;
    if (numberChannels == 1)
    {
        result.digitalSignal_1Channel = result.digitalSignal_1Channel.multipleSignal(oAW.digitalSignal_1Channel);
    }
    else
    {
        result.digitalSignal_2Channel.first = result.digitalSignal_2Channel.first.multipleSignal(oAW.digitalSignal_2Channel.first);
        result.digitalSignal_2Channel.second = result.digitalSignal_2Channel.second.multipleSignal(oAW.digitalSignal_2Channel.second);
    }
    result.loadDataFromDigitalSignal();
    return result;
}

AudioWav AudioWav::multibleSignal(double ratio1, double ratio2)
{
    AudioWav result;
    result = *this;
    if (numberChannels == 1)
    {
        result.digitalSignal_1Channel = result.digitalSignal_1Channel.multipleSignal(ratio1);
    }
    else
    {
        result.digitalSignal_2Channel.first = result.digitalSignal_2Channel.first.multipleSignal(ratio1);
        result.digitalSignal_2Channel.second = result.digitalSignal_2Channel.second.multipleSignal(ratio2);
    }
    result.loadDataFromDigitalSignal();
    return result;
}

AudioWav AudioWav::inverseSignal()
{
    AudioWav result;
    result = *this;
    if (numberChannels == 1)
    {
        result.digitalSignal_1Channel = result.digitalSignal_1Channel.inverseSignal();
    }
    else
    {
        result.digitalSignal_2Channel.first = result.digitalSignal_2Channel.first.inverseSignal();
        result.digitalSignal_2Channel.second = result.digitalSignal_2Channel.second.inverseSignal();
    }
    result.loadDataFromDigitalSignal();
    return result;
}

AudioWav AudioWav::timeShifting(double time1)
{
    AudioWav result;
    result = *this;
    result.totalSamples = result.totalSamples + time1 * sampleRate;
    if (numberChannels == 1)
    {
        result.digitalSignal_1Channel = result.digitalSignal_1Channel.timeShifting(time1);
    }
    else
    {
        result.digitalSignal_2Channel.first = result.digitalSignal_2Channel.first.timeShifting(time1);
        result.digitalSignal_2Channel.second = result.digitalSignal_2Channel.second.timeShifting(time1);
    }
    result.loadDataFromDigitalSignal();
    result.subchunk2Size = result.totalSamples * result.numberChannels * bitsPerSample / 2;

    return result;
}

AudioWav AudioWav::downSampling(int factor)
{
    AudioWav result;
    result = *this;
    if (numberChannels == 1)
    {
        result.digitalSignal_1Channel = result.digitalSignal_1Channel.downSampling(factor);
    }
    else
    {
        result.digitalSignal_2Channel.first = result.digitalSignal_2Channel.first.downSampling(factor);
        result.digitalSignal_2Channel.second = result.digitalSignal_2Channel.second.downSampling(factor);
    }
    result.setSampleRate(result.sampleRate / factor);
    result.totalSamples = result.totalSamples / factor;
    result.subchunk2Size = result.totalSamples * numberChannels * (bitsPerSample / 8);
    result.loadDataFromDigitalSignal();

    return result;
}

AudioWav AudioWav::upSampling(int factor)
{
    AudioWav result;
    result = *this;
    if (numberChannels == 1)
    {
        result.digitalSignal_1Channel = result.digitalSignal_1Channel.upSampling(factor);
    }
    else
    {
        result.digitalSignal_2Channel.first = result.digitalSignal_2Channel.first.upSampling(factor);
        result.digitalSignal_2Channel.second = result.digitalSignal_2Channel.second.upSampling(factor);
    }
    result.setSampleRate(result.sampleRate * factor);
    result.totalSamples = result.totalSamples * factor;
    result.subchunk2Size = result.totalSamples * numberChannels * (bitsPerSample / 8);
    result.loadDataFromDigitalSignal();
    return result;
}

void AudioWav::printInfo()
{
    if (numberChannels == 1)
    {
        digitalSignal_1Channel.printSignalInfo();
    }
    else
    {
        cout << "Left channel" << endl;
        digitalSignal_2Channel.first.printSignalInfo();
        cout << endl
             << "Right channel" << endl;
        digitalSignal_2Channel.second.printSignalInfo();
    }
}

//