#include <gtest/gtest.h>
#include "../libs/cvUtils.h"

TEST(computeHistogramTest, Works) {
    Mat image = Mat::zeros(3, 3, CV_8UC1);

    image.at<uchar>(0, 0) = 200;
    image.at<uchar>(0, 1) = 0;
    image.at<uchar>(0, 2) = 125;
    image.at<uchar>(1, 1) = 127;

    int histogram[256];

    computeHistogram(image, histogram);


}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

