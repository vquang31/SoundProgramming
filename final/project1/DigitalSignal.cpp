#include "DigitalSignal.hpp"

DigitalSignal::DigitalSignal()
{
}

DigitalSignal::DigitalSignal(string s, int sampleRate, int bitDepth, int numberChannels, int type)
{
    totalSamples = data.size();
    setSampleRate(sampleRate);
    setBitDepth(bitDepth);
    setNumberChannels(numberChannels);
    this->duration = totalSamples / sampleRate / numberChannels;
    std::ifstream file(s);
    string line;
    if (type == TYPE_AMPLITUDE)
    {
        while (std::getline(file, line))
        {
            try
            {
                int tmp = stoi(line);
                data.push_back(tmp);
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Not a valid number " << e.what() << std::endl;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Out of range" << e.what() << std::endl;
            }
        }
    }
    else
    {
        // type == TYPE_RATIO;
        while (std::getline(file, line))
        {
            try
            {
                double tmp = stod(line);
                int x = tmp * maxAmplitude;
                data.push_back(tmp);
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Not a valid number " << e.what() << std::endl;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Out of range" << e.what() << std::endl;
            }
        }
    }

    file.close();
}

DigitalSignal::DigitalSignal(vector<short> data, int sampleRate, int bitDepth, int numberChannels, double duration, int type)
{
    this->sampleRate = sampleRate;
    setBitDepth(bitDepth);
    this->numberChannels = numberChannels;
    if (type == DigitalSignal::TYPE_FREQUENCY)
    {
        // data is frequency
        int len = data.size();
        double pi = atan(1) * 4;
        int x = 0;
        this->duration = duration;
        totalSamples = duration * sampleRate;
        for (int i = 0; i < totalSamples; i++)
        {
            double tmp = sin(2 * pi * data[x] * i * 1 / sampleRate);
            int amplitude = maxAmplitude * tmp;
            this->data.push_back(amplitude);
            if ((x + 1) * totalSamples / len < i)
                x++;
        }
    }
    else
    {
        //  data is array Amplitupe
        int len = data.size();
        this->totalSamples = len;
        for (int i = 0; i < len; i++)
        {
            this->data.push_back(data[i]);
        }
        this->duration = totalSamples / sampleRate / numberChannels;
    }
}

void DigitalSignal::setSampleRate(int sampleRate)
{
    this->sampleRate = sampleRate;
    if (totalSamples != 0 && bitDepth != 0 && numberChannels)
        this->duration = totalSamples / sampleRate / numberChannels;
}

void DigitalSignal::setBitDepth(int bitDepth)
{
    this->bitDepth = bitDepth;
    this->maxAmplitude = pow(2, bitDepth - 1) - 1;
}

void DigitalSignal::setNumberChannels(int numberChannels)
{
    if (numberChannels != 1 && numberChannels != 2)
    {
        cout << "Number channel must be 1 or 2" << endl;
        return;
    }
    this->numberChannels = numberChannels;
    if (totalSamples != 0 && sampleRate != 0 && numberChannels)
        this->duration = totalSamples / sampleRate / numberChannels;
}

int DigitalSignal::getSampleRate()
{
    return sampleRate;
}

int DigitalSignal::getBitDepth()
{
    return bitDepth;
}

int DigitalSignal::getMaxAmplitude()
{
    return maxAmplitude;
}

int DigitalSignal::getNumberChannels()
{
    return numberChannels;
}

long long DigitalSignal::getTotalSamples()
{
    return totalSamples;
}

double DigitalSignal::getDuration()
{
    return duration;
}

DigitalSignal DigitalSignal::mixSignal(DigitalSignal ortherDS, double ratioSefl, double ratioSecond)
{
    DigitalSignal result;
    result = ortherDS;
    int maxLen = data.size() > ortherDS.data.size() ? data.size() : ortherDS.data.size();
    for (int i = 0; i < maxLen; i++)
    {
        if (i < data.size() && i < ortherDS.data.size())
        {
            result.data[i] = data[i] * ratioSefl + ortherDS.data[i] * ratioSecond;
        }
        else if (i < data.size())
        {
            result.data[i] = data[i] * ratioSefl;
        }
        else
        {
            result.data[i] = ortherDS.data[i] * ratioSecond;
        }
    }
    return result;
}

DigitalSignal DigitalSignal::multipleSignal(DigitalSignal ortherDs)
{
    DigitalSignal result;
    result = *this;
    int maxLen = data.size() > ortherDs.data.size() ? data.size() : ortherDs.data.size();

    for (int i = 0; i < maxLen; i++)
    {
        if (i < data.size() && i < ortherDs.data.size())
        {
            result.data[i] = data[i] * ortherDs.data[i];
        }
        else
        {
            result.data[i] = 0;
        }
    }
    return result;
}

DigitalSignal DigitalSignal::multipleSignal(double ratio)
{
    DigitalSignal result;
    result = *this;
    for (int i = 0; i < data.size(); i++)
    {
        result.data[i] = data[i] * ratio;
    }
    return result;
}

DigitalSignal DigitalSignal::inverseSignal()
{
    DigitalSignal result;
    result = *this;
    int length = data.size() - 1;
    for (int i = 0; i < data.size(); i++)
    {
        result.data[i] = data[length - i];
    }
    return result;
}

DigitalSignal DigitalSignal::timeShifting(double time)
{
    DigitalSignal result;
    result = *this;
    int shift = time * sampleRate;
    if (shift > 0)
    {
        for (int i = 0; i < shift; i++)
        {
            // note
            result.data.insert(result.data.begin(), 0);
        }
    }
    else
    {
        shift = -shift;
        for (int i = 0; i < shift; i++)
        {
            // result.data.push_back(0);
            result.data.erase(result.data.begin());
        }
    }
    result.totalSamples = result.totalSamples + sampleRate * time;
    result.duration = result.duration + time;
    return result;
}

DigitalSignal DigitalSignal::downSampling(int factor)
{
    DigitalSignal result;
    result = *this;

    int count = factor;

    result.data.clear();
    for (int i = 0; i < data.size(); ++i)
    {
        if (i % factor == 0)
        {
            result.data.push_back(data[i]);
        }
    }

    result.sampleRate = result.sampleRate / factor;
    result.totalSamples = result.totalSamples / factor;
    return result;
}

DigitalSignal DigitalSignal::upSampling(int factor)
{
    DigitalSignal result;
    result = *this;

    for (int i = 0; i < totalSamples; i++)
    {
        for (int j = 1; j <= factor - 1; j++)
        {
            // note
            result.data.insert(result.data.begin() + i * factor + 1, 0);
        }
    }
    result.sampleRate = result.sampleRate * factor;
    result.totalSamples = result.totalSamples * factor;
    return result;
}

void DigitalSignal::printSignalInfo()
{
    cout << "Sample rate: " << sampleRate << endl;
    cout << "Bit depth: " << bitDepth << endl;
    cout << "Max amplitude: " << maxAmplitude << endl;
    cout << "Number channel: " << numberChannels << endl;
    cout << "Total sample: " << totalSamples << endl;
    cout << "Duration: " << duration << endl;
}

void DigitalSignal::printSignalData()
{
    for (int i = 0; i < data.size(); i++)
    {
        cout << data[i] << endl;
    }
}

void DigitalSignal::exportData(string s)
{
    ofstream file(s);
    for (int i = 0; i < data.size(); i++)
    {
        file << data[i] << endl;
    }
    file.close();
    cout << "Xuat file thanh cong !!" << endl;
}
