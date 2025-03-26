#pragma once

#include "Audio.h"
#include <bits/stdc++.h>

Audio::Audio(std::string str)
{
    if (str.substr(str.size() - 4) != ".wav")
        throw std::invalid_argument("Can only read WAV files!");
    load_wav(str);
}

void Audio::load_wav(std::string str)
{
    FILE *fp;
    errno_t err = fopen_s(&fp, str.c_str(), "rb");
    if (err != 0)
        throw std::runtime_error("Error opening file!");

    // Chunk
    char type[5] = {0}, format[5] = {0}, Subchunk1ID[5] = {0}, Subchunk2ID[5] = {0};
    fread(type, sizeof(char), 4, fp);
    if (strcmp(type, "RIFF") != 0)
        throw std::runtime_error("Not a RIFF file!");

    fread(&ChunkSize, sizeof(int), 1, fp);
    fread(format, sizeof(char), 4, fp);
    if (strcmp(format, "WAVE") != 0)
        throw std::runtime_error("Not a WAVE format!");

    // 1st Subchunk
    fread(Subchunk1ID, sizeof(char), 4, fp);
    if (strcmp(Subchunk1ID, "fmt "))
        throw std::runtime_error("Missing fmt header!");
    fread(&Subchunk1Size, sizeof(int), 1, fp);
    fread(&AudioFormat, sizeof(short), 1, fp);
    fread(&NumChannels, sizeof(short), 1, fp);
    fread(&SampleRate, sizeof(int), 1, fp);
    fread(&ByteRate, sizeof(int), 1, fp);
    fread(&BlockAlign, sizeof(short), 1, fp);
    fread(&BitsPerSample, sizeof(short), 1, fp);

    // 2nd Subchunk
    fread(Subchunk2ID, sizeof(char), 4, fp);
    if (strcmp(Subchunk2ID, "data") != 0)
        throw std::runtime_error("Missing data header!");
    fread(&Subchunk2Size, sizeof(int), 1, fp);
    // data

    // Subchunk2Size = NumSamples * NumChannels * BitsPerSample/8
    int NumSamples = Subchunk2Size / (NumChannels * (BitsPerSample / 8));

    data2Channel = std::vector<std::pair<short, short>>(NumSamples);
    for (int i = 0; i < NumSamples; i++)
    {
        fread(&data2Channel[i].first, sizeof(short), 1, fp);
        fread(&data2Channel[i].second, sizeof(short), 1, fp);
    }
    fclose(fp);
}

void Audio::write_wav(std::string str)
{
    FILE *fp;
    fopen_s(&fp, str.c_str(), "wb");

    fwrite(type, sizeof(char), 4, fp);
    fwrite(&ChunkSize, sizeof(int), 1, fp);
    fwrite(format, sizeof(char), 4, fp);

    // 1st Subchunk
    fwrite(Subchunk1ID, sizeof(char), 4, fp);
    fwrite(&Subchunk1Size, sizeof(int), 1, fp);
    fwrite(&AudioFormat, sizeof(short), 1, fp);
    fwrite(&NumChannels, sizeof(short), 1, fp);
    fwrite(&SampleRate, sizeof(int), 1, fp);
    fwrite(&ByteRate, sizeof(int), 1, fp);
    fwrite(&BlockAlign, sizeof(short), 1, fp);
    fwrite(&BitsPerSample, sizeof(short), 1, fp);

    // 2nd Subchunk
    fwrite(Subchunk2ID, sizeof(char), 4, fp);
    fwrite(&Subchunk2Size, sizeof(int), 1, fp);

    // Ideally instead of the for loop there would be something like
    // fwrite(&data2Channel[0], sizeof(short), data2Channel.size(), fp);
    // But I have pairs and they have to be written interchangebly like right,left right,left right,left...

    for (int i = 0; i < data2Channel.size(); i++)
    {
        fwrite(&data2Channel[i].first, sizeof(short), 1, fp);
        fwrite(&data2Channel[i].second, sizeof(short), 1, fp);
    }
    fclose(fp);
}

void Audio::getData()
{
    std::ofstream oFile("data2Channel.dat");
    for (int i = 0; i < data2Channel.size(); i++)
    {
        oFile << data2Channel[i].first << " " << data2Channel[i].second << std::endl;
    }
    oFile.close();
}