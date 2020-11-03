#include <iostream>
#include <opencv2/opencv.hpp>
#include "argparse.hpp"
#include "libs/fileUtils.h"
#include "libs/cvUtils.h"
#include "libs/MessageReplacer.h"
#include <iterator>

using namespace std;
using namespace cv;

const char loadingSteps[4] = { '\\', '|', '/', '-' };
void loadTrainedDataset (const string & trainedDataPath, vector<tuple<int, deque<int>>> & trainedData);
void test(const string & datasetDirectoryPath, const string & trainedDataPath, int suffix);
vector<string> explode(const string& s, const char& c);

const int DISTANCE_SAD = 0;
int getDistanceSAD(int (&a) [256], deque<int> b)
{
    int sum = 0;

    for (int i = 0; i < 256; i += 1)
    {
        const auto distance = abs(a[i] - b[i]);
        sum += distance;
    }

    return sum;
}
int getDistance (int (&a) [256], deque<int> b, int method = DISTANCE_SAD)
{
    switch (method) {
        case DISTANCE_SAD:
            return getDistanceSAD(a, b);
        default:
            return -1;
    }
}


int main (int argc, const char ** argv) {
    ArgumentParser argumentParser;

    argumentParser.addArgument("-d", "--dataset", 1, false);
    argumentParser.addArgument("-t", "--trained-data", 1, false);
    argumentParser.addArgument("-s", "--suffix", 1, false);

    argumentParser.parse(argc, argv);

    const auto datasetDirectoryPath = argumentParser.retrieve<string>("dataset");
    const auto trainedDataPath = argumentParser.retrieve<string>("trained-data");
    const auto suffixString = argumentParser.retrieve<string>("suffix");
    const auto suffix = stoi(suffixString);

    test(datasetDirectoryPath, trainedDataPath, suffix);

    return 0;
}

void test(const string &datasetDirectoryPath, const string &trainedDataPath, int suffix) {
    const auto images = findFilesInDirectory(datasetDirectoryPath);
    const auto imagesCount = images.size();
    auto doneCount = 0;
    auto foundCount = 0;

    MessageReplacer replacer;

    vector<tuple<int, deque<int>>> trainedData;
    loadTrainedDataset(trainedDataPath, trainedData);

    for (const auto & imageFolder : images)
    {
        Mat image;
        image = imread(imageFolder, IMREAD_GRAYSCALE);

        int histogram[256];
        computeHistogram(image, histogram);

        int minDistance = INT_MAX;
        int foundSuffix = -1;

        for (auto trainedDataLine : trainedData) {
            auto type = get<0>(trainedDataLine);
            auto trainedHistogram = get<1>(trainedDataLine);
            const auto distance = getDistance(histogram, trainedHistogram);

            if (distance < minDistance) {
                minDistance = distance;
                foundSuffix = type;
            }
        }

        if (foundSuffix == suffix) {
            foundCount ++;
        }

        doneCount ++;
        const auto donePercent = (int) round((float) doneCount / (float) imagesCount * 100);
        string loadingCharacter(1, loadingSteps[doneCount % 4]);
        const auto percentMessage = loadingCharacter + " " + to_string(donePercent) + "%";
//        cout << percentMessage << endl;
        replacer.replace(percentMessage);
    }

    replacer.clear();

    const auto foundPercent = (int) round((float) foundCount / (float) imagesCount * 100);
    const auto foundPercentMessage = to_string(foundPercent) + "% found!";

    cout << foundPercentMessage << endl;
}
void loadTrainedDataset (const string & trainedDataPath, vector<tuple<int, deque<int>>> & trainedData) {
    fstream trainingStream;
    trainingStream.open(trainedDataPath, ios::in);

    if (trainingStream.is_open()) {
        string line;
        while (getline(trainingStream, line)) {
            const auto stringValues = explode(line, ' ');
            auto queue = deque<int>();
//            int lineHistogram[256];

            for (int i = 0; i < stringValues.size() - 2; i += 1) {
                const auto stringValue = stringValues[i];
                const auto intValue = stoi(stringValue);
                queue.push_front(intValue);
//                lineHistogram[i] = intValue;
            }

            const auto typeString = stringValues[stringValues.size() - 1];
            int type = stoi(typeString);

            tuple<int, deque<int>> t = make_tuple(type, queue);
            trainedData.push_back(t);
        }
    }
}

vector<string> explode(const string& s, const char& c)
{
    string buff{""};
    vector<string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    return v;
}