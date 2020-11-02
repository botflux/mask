#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//
// Created by Victor MENDELE on 12/10/2020.
//

#ifndef MASK_CVUTILS_H
#define MASK_CVUTILS_H

/// Append values of the histogram into a text file.
/// \param stream
/// \param histogram
/// \param suffix
void insertHistogramInFile (ofstream & stream, int (&histogram) [256], int suffix)
{
    string line;

    for (const auto & value : histogram)
    {
        line += to_string(value) + " ";
    }

    line += to_string(suffix);

    stream << line << endl;
}

Point points [] = {
        Point (-1, -1),
        Point (-1, 0),
        Point (-1, 1),
        Point (0, 1),
        Point (1, 1),
        Point (1, 0),
        Point (1, -1),
        Point (0, -1)
};

/// Compute an histogram from a given matrix.
/// \param image
/// \param histogram
void computeHistogram(const Mat & image, int (&histogram) [256])
{
    for (int & i : histogram)
        i = 0;

    for (int y = 1; y < image.rows - 1; ++ y)
    {
        for (int x = 1; x < image.cols - 1; ++ x)
        {
            const auto middlePixelColor = (int) image.at<uchar>(y, x);
            int lpb = 0;
            int i = 0;

            for (const auto & point : points)
            {
                const auto & pointInImage = Point (point.x + x, point.y + y);
                const auto pixelColor = (int) image.at<uchar>(pointInImage.y, pointInImage.x);

                if (middlePixelColor >= pixelColor)
                    lpb += pow(2, i);

                ++i;
            }

            histogram[lpb] += 1;
        }
    }
}
#endif //MASK_CVUTILS_H
