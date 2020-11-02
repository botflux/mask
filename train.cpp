#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "deps/argparse/argparse.hpp"
#include "libs/fileUtils.h"
#include "libs/cvUtils.h"
#include "libs/MessageReplacer.h"

using namespace std;
using namespace cv;

const char loadingSteps[4] = { '\\', '|', '/', '-' };

/// Train the model.
/// \param datasetFolder
/// \param outputFile
/// \param suffix
void train (const string & datasetFolder, const string & outputFile, int suffix);

/**
 * Handle console loading animation.
 * @param lastLoadingMessage
 * @param newMessage
 */
void handleLoading (string & lastLoadingMessage, const string & newMessage);

int main (const int argc, const char ** argv) {
    ArgumentParser argumentParser;

    argumentParser.addArgument("-s", "--suffix", 1, false);
    argumentParser.addArgument("-d", "--dataset", 1, false);
    argumentParser.addFinalArgument("resultFile", 1, false);

    argumentParser.parse(argc, argv);

    const auto suffixString = argumentParser.retrieve<string>("suffix");
    const auto datasetDirectoryPath = argumentParser.retrieve<string>("dataset");
    const auto resultFilePath = argumentParser.retrieve<string>("resultFile");
    const auto suffix = stoi(suffixString);

    train(datasetDirectoryPath, resultFilePath, suffix);

    return 0;
}

void train (const string & datasetFolder, const string & outputFile, int suffix)
{
    const auto imagesNames = findFilesInDirectory(datasetFolder);
    const auto imagesCount = imagesNames.size();

    int done = 0;

    ofstream fout;
    fout.open(outputFile, ios::app);
    assert(!fout.fail());

//    string lastMessage;
    MessageReplacer replacer;

    for (const auto & imageName : imagesNames)
    {
        Mat image = imread(imageName, IMREAD_GRAYSCALE);
        int histogram[256];

        // Compute histogram
        computeHistogram(image, histogram);
        insertHistogramInFile(fout, histogram, suffix);

        // Handling console output
        done += 1;
        const auto donePercent = (int)round(((float)done / (float) imagesCount) * 100);
        string loadingCharacter(1, loadingSteps[done % 4]);
        const auto consoleMessage =  loadingCharacter + " " + to_string(donePercent) + "%";
        replacer.replace(consoleMessage);
    }

    replacer.clear();

    cout << "Training finished! You can find the result under " << outputFile << endl;

    fout.close();
    assert(!fout.fail());
}

//void handleLoading (string & lastLoadingMessage, const string & newMessage)
//{
//    const auto lastMessageLength = lastLoadingMessage.length();
//
//    for (auto i = 0; i < lastMessageLength; i += 1)
//    {
//        cout << "\b" << flush;
//    }
//
//    lastLoadingMessage = newMessage;
//    cout << newMessage;
//}