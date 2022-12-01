/**
 * Karaka
 *
 * @package   App
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui.hpp>

#include "../cOMS/Utils/MathUtils.h"
#include "../cOMS/Utils/ArrayUtils.h"
#include "../cOMS/Utils/FileUtils.h"
#include <vector>

#ifndef OMS_DEMO
    #define OMS_DEMO false
#endif

void printHelp()
{
    printf("    The OCRImageOptimizer developed by jingga is a image optimizer for OCR readers in\n");
    printf("    order to improve the text recognition performance on non-optimal image sources.\n\n");
    printf("    Run: ./App -i <input_path_to_the_image> -o <output_path_for_new_image>\n\n");
    printf("    -h: Prints the help output\n");
    printf("    -v: Prints the version\n");
    printf("    -i: The input image which should get optimized\n");
    printf("    -o: The output path for the optimized image\n\n");
    printf("\n");
    printf("    Website: https://jingga.app\n");
    printf("    Copyright: jingga (c) Dennis Eichhorn\n");
}

void printVersion()
{
    printf("Version: 1.0.0\n");
}

int main(int argc, char **argv)
{
    bool hasHelpCmd  = Utils::ArrayUtils::has_arg("-h", argv, argc);
    if (hasHelpCmd) {
        printHelp();

        return 0;
    }

    bool hasVersionCmd  = Utils::ArrayUtils::has_arg("-v", argv, argc);
    if (hasVersionCmd) {
        printVersion();

        return 0;
    }

    char *inputImage  = Utils::ArrayUtils::get_arg("-i", argv, argc);
    char *outputImage = Utils::ArrayUtils::get_arg("-o", argv, argc);

    if (inputImage == NULL || outputImage == NULL) {
        printf("Invalid application usage:\n");

        printHelp();

        return -1;
    }

    if (!Utils::FileUtils::file_exists(inputImage)) {
        printf("Image file doesn't exist.\n");

        return -1;
    }

    cv::Mat in;
    in = cv::imread(inputImage, cv::COLOR_BGRA2BGR);
    if (!in.data) {
        printf("Couldn't read image.\n");

        return -1;
    }

    cv::SimpleBlobDetector::Params params;

    // Color
    params.filterByColor = false;

    // Change thresholds
	params.minThreshold = 50;
	params.maxThreshold = 500;
    params.thresholdStep = 1.0;

    params.minDistBetweenBlobs = 20;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 50;
    params.maxArea = 5000;

	// Filter by Circularity
	params.filterByCircularity = false;
	params.minCircularity = 0.01;
    params.maxCircularity = 1.0;

	// Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.87;
    params.maxConvexity = 1.0;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.5;
    params.maxInertiaRatio = 50.0;


	// Storage for blobs
	std::vector<cv::KeyPoint> keypoints;

    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

	// Detect blobs
	detector->detect( in, keypoints);

    cv::Mat out;
	cv::drawKeypoints( in, keypoints, out, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	// Show blobs
	cv::imshow("keypoints", out );
	cv::waitKey(0);

    /*
    int maxDim = oms_max(in.size().width, in.size().height);
    if (maxDim > 1500) {
        cv::resize(in, in, cv::Size(
            (int) (in.size().width * 1500 / maxDim),
            (int) (in.size().height * 1500 / maxDim)
        ), 0.0, 0.0, cv::INTER_AREA);
    }
    */

    #if OMS_DEBUG
        cv::imshow("original", in);
    #endif

    // cv::cvtColor(in, out, cv::COLOR_BGRA2BGR); // alternatively use CV_BGR2GRAY

    #if OMS_DEMO
        cv::Size dim = out.size();
        cv::putText(out, "Demo", cv::Point(out.size().width / 2, out.size().height / 2), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    #endif

    cv::imwrite(outputImage, out);

    #if OMS_DEBUG
        cv::waitKey(0);
    #endif

    return 0;
}
