#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;
using namespace std;

//
// Created by Victor MENDELE on 12/10/2020.
//

#ifndef MASK_FILEUTILS_H
#define MASK_FILEUTILS_H
/// Returns every files inside the given directory path.
/// \param directoryPath
/// \return Files list
vector<string> findFilesInDirectory (const string & directoryPath)
{
    vector<string> foundFiles;

    for (const auto & entry : fs::directory_iterator(directoryPath)) {
        string extension = entry.path().extension().string();

        if (extension != ".jpg") {
            continue;
        }

        const string absoluteFilePath = fs::absolute(entry.path());
        foundFiles.push_back(absoluteFilePath);
    }

    return foundFiles;
}
#endif //MASK_FILEUTILS_H
