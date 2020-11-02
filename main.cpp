#include <iostream>
#include <opencv2/opencv.hpp>
#include <experimental/filesystem>
#include <fstream>
#include "deps/argparse/argparse.hpp"
#include "libs/cvUtils.h"
#include "libs/fileUtils.h"

namespace fs = std::filesystem;
using namespace std;
using namespace cv;

/// Load images from a vector of ABSOLUTE image paths.
/// \param imagePaths
/// \return
vector<Mat> loadImages (const vector<string> & imagePaths)
{
    vector<Mat> images;
    for (auto const & filename : imagePaths)
    {
        Mat image;
        image = imread(filename, IMREAD_COLOR);

        if (image.empty())
        {
            cout << filename << " was not loaded \n";
            continue;
        }

        images.push_back(image);
    }

    return images;
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

void train (const string & folder, const string & outputFile, const int & suffix)
{
//    const string folder = "/Users/v-mendele/MEGAsync/PBLDatasetCreate/dataset/GRAY/1TRAIN/CMFD";
    const vector<string> imagesNames = findFilesInDirectory(folder);
//    const auto resultFile = "../output.txt";

    ofstream fout;
    fout.open(outputFile, ios::app);
    assert(!fout.fail());

    for (const auto & imageName : imagesNames)
    {
        cout << "Opening image..." << endl;

        Mat image = imread(imageName, IMREAD_GRAYSCALE);
        int histogram[256];

        cout << "Compute histogram" << endl;

        computeHistogram(image, histogram);

        cout << "Insert in file" << endl;

        insertHistogramInFile(fout, histogram, suffix);
    }

    fout.close();
    assert(!fout.fail());
}

const int DISTANCE_SAD = 0;

int getDistanceSAD(int (&a) [256], int (&b) [256])
{
    int sum = 0;

    for (int i = 0; i < 256; i += 1)
    {
        const auto distance = abs(a[i] - b[i]);
        sum += distance;
    }

    return sum;
}

int getDistance (int (&a) [256], int (&b) [256], int method = DISTANCE_SAD)
{
    switch (method) {
        case DISTANCE_SAD:
            return getDistanceSAD(a, b);
        default:
            return -1;
    }
}

void test(const string & testImageFile, const string & trainingData)
{
    Mat image;
    image = imread(testImageFile, IMREAD_GRAYSCALE);

    int histogram[256];
    computeHistogram(image, histogram);

    fstream trainingStream;
    trainingStream.open(trainingData, ios::in);

    bool correctlyMasked = false;
    int minDistance = INT_MAX;

    if (trainingStream.is_open())
    {
        string line;

        while (getline(trainingStream, line)) {
            const auto stringValues = explode(line, ' ');
            int lineHistogram[256];

            for (int i = 0; i < stringValues.size() - 2; i += 1)
            {
                const auto stringValue = stringValues[i];
                const auto intValue = stoi(stringValue);
                lineHistogram[i] = intValue;
            }

            const auto typeString = stringValues[stringValues.size() - 1];
            const auto type = stoi(typeString);

            // Calculate distance
            const auto distance = getDistance(histogram, lineHistogram);

            if (distance < minDistance) {
                minDistance = distance;
                correctlyMasked = type == 1;
            }
        }

        cout << "You are " << (correctlyMasked ? "correctly" : "not correctly") << " masked." << endl;

//        throw new Exception("Can't open file");
    }

    // refaire un histogramme
    // pour chaque image
    //      faire une distance
    // prendre le min
}

int main(int argc, const char ** argv) {

    ArgumentParser argumentParser;

    argumentParser.addArgument("--mode");
    argumentParser.addArgument("-d", "--dataset-path", 1, true);
    argumentParser.addArgument("-o", "--output-file", 1, true);
    argumentParser.addArgument("-t", "--test-image", 1, true);
    argumentParser.addArgument("-a", "--training-data", 1, true);
    argumentParser.addArgument("-s", "--suffix", 1, true);
    argumentParser.addFinalArgument("mode", 1, false);
    argumentParser.parse(argc, argv);

    const auto mode = argumentParser.retrieve<string>("mode");

    if (mode == "train") {
        cout << "Start training..." << endl;
        const auto trainDatasetPath = argumentParser.retrieve<string>("dataset-path");
        const auto resultFilePath = argumentParser.retrieve<string>("output-file");
        const auto suffixString = argumentParser.retrieve<string>("suffix");
        const auto suffix = stoi(suffixString);
        train(trainDatasetPath, resultFilePath, suffix);
    } else if (mode == "test") {
        cout << "Start testing..." << endl;
        const auto testImageFile = argumentParser.retrieve<string>("test-image");
        const auto trainingData = argumentParser.retrieve<string>("training-data");
        cout << testImageFile << endl;
        test(testImageFile, trainingData);
    } else {
        cout << "You must select a mode using --mode (train|test)";
    }


//    const string filename = "../examples/test.jpg

//    cout << "[ ";
//
//    for (int i = 0; i < 256; ++ i)
//    {
//        cout << histogram[i] << ", ";
//
//        if (i % 10 == 0 && i != 0)
//            cout << endl;
//    }
//
//    cout << " ]";

    return 0;
}
