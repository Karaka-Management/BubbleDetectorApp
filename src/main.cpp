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
#include <time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>

#include "../cOMS/Utils/MathUtils.h"
#include "../cOMS/Utils/ArrayUtils.h"
#include "../cOMS/Utils/FileUtils.h"
#include <vector>

#include "Models/Config.h"
#include "Models/Helper.h"

#ifndef OMS_DEMO
    #define OMS_DEMO false
#endif

void printHelp()
{
    printf("    The OCRImageOptimizer developed by jingga is a image optimizer for OCR readers in\n");
    printf("    order to improve the text recognition performance on non-optimal image sources.\n\n");
    printf("    Run: ./App -i <input_path_to_the_image> -o <output_path_for_config.new_image>\n\n");
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

app_config config;

cv::Mat in;
cv::Mat out;
cv::Ptr<cv::SimpleBlobDetector> detector;

const cv::Scalar textColor = cv::Scalar(255, 172, 37);
const cv::Scalar bubbleColor = cv::Scalar(0, 0, 255);

static void filter(int, void*)
{
    if (config.new_min_area != config.old_min_area
        || config.new_max_area != config.old_max_area
        || config.new_min_threshold != config.old_min_threshold
        || config.new_max_threshold != config.old_max_threshold
        || config.new_step_threshold != config.old_step_threshold
        || config.new_min_distance != config.old_min_distance
        || config.new_min_circularity != config.old_min_circularity
        || config.new_max_circularity != config.old_max_circularity
        || config.new_min_convexity != config.old_min_convexity
        || config.new_max_convexity != config.old_max_convexity
        || config.new_min_inertia != config.old_min_inertia
        || config.new_max_inertia != config.old_max_inertia
    ) {
        config.hasUpdate = true;
    }

    if (config.lastUpdate >= config.maxFrameUpdate && config.hasUpdate) {
        detector   = cv::SimpleBlobDetector::create(createDetectionParams(0, NULL, &config));
        config.lastUpdate = 0;
        config.hasUpdate  = false;
    }

    if (config.lastUpdate < 10000) {
        ++config.lastUpdate;
    }

    if (config.lastLog < 10000) {
        ++config.lastLog;
    }

    std::vector<cv::KeyPoint> keypoints;

    // Detect blobs
    detector->detect(in, keypoints);

    float totalVolume = 0.0;
    char buffer[25];

    totalVolume = 0.0;
    for (int i = 0; i < keypoints.size(); ++i) {
        totalVolume += OMS_PI * keypoints[i].size * keypoints[i].size;
    }

    if (config.lastLog > config.minLogDelay
        && ((config.volumeThreshold > 0.0 || config.countThreshold > 0)
            && (totalVolume > config.volumeThreshold || keypoints.size() > config.countThreshold)
        )
    ) {
        config.logImage = true;
    }

    cv::drawKeypoints(in, keypoints, out, bubbleColor, cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    rectangle(out, cv::Point(5, 5), cv::Point(180, 43), cv::Scalar(255, 255, 255), cv::FILLED, cv::LINE_8);

    sprintf(buffer, "Bubbles: %ld", keypoints.size());
    cv::putText(out, buffer, cv::Point(7, 20), cv::FONT_HERSHEY_PLAIN, 1.3, textColor, 1);

    memset(buffer, '\0', sizeof(buffer));

    sprintf(buffer, "Area: %.2f", totalVolume);
    cv::putText(out, buffer, cv::Point(7, 40), cv::FONT_HERSHEY_PLAIN, 1.3, textColor, 1);

    if (config.logImage) {
        createImageLog(config.outputImage, out);
        config.logImage = false;
    }

    if (config.verbose) {
        cv::imshow("Bubble Detection", out);
    }
}

inline
void createTrackbars()
{
    cv::createTrackbar("Min. Size", "Settings", &config.new_min_area, config.max_min_area, filter);
    cv::createTrackbar("Max. Size", "Settings", &config.new_max_area, config.max_max_area, filter);
    cv::createTrackbar("Min. Threshold", "Settings", &config.new_min_threshold, config.max_min_threshold, filter);
    cv::createTrackbar("Max. Threshold", "Settings", &config.new_max_threshold, config.max_max_threshold, filter);
    cv::createTrackbar("Threshold Steps", "Settings", &config.new_step_threshold, config.max_step_threshold, filter);
    cv::createTrackbar("Min. Distance", "Settings", &config.new_min_distance, config.max_min_distance, filter);
    cv::createTrackbar("Min. Circularity", "Settings", &config.new_min_circularity, config.max_min_circularity, filter);
    cv::createTrackbar("Max. Circularity", "Settings", &config.new_max_circularity, config.max_max_circularity, filter);
    cv::createTrackbar("Min. Convexity", "Settings", &config.new_min_convexity, config.max_min_convexity, filter);
    cv::createTrackbar("Max. Convexity", "Settings", &config.new_max_convexity, config.max_max_convexity, filter);
    cv::createTrackbar("Min. Inertia", "Settings", &config.new_min_inertia, config.max_min_inertia, filter);
    cv::createTrackbar("Max. Inertia", "Settings", &config.new_max_inertia, config.max_max_inertia, filter);
}

void handleVideo(int argc, char **argv)
{
    config.countThreshold  = atoi(Utils::ArrayUtils::get_arg("-count_threshold", argv, argc));
    config.volumeThreshold = atof(Utils::ArrayUtils::get_arg("-volume_threshold", argv, argc));
    config.minLogDelay     = Utils::ArrayUtils::has_arg("-min_logdelay", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-min_logdelay", argv, argc)) : 500;

    cv::VideoCapture cap;

    int deviceID = atoi(Utils::ArrayUtils::get_arg("-c", argv, argc));
    int apiID    = cv::CAP_ANY;

    cap.open(deviceID, apiID);
    cap.read(in);

    detector = cv::SimpleBlobDetector::create(createDetectionParams(argc, argv, &config));

    if (config.verbose) {
        createTrackbars();
    }

    while (true) {
        cap.read(in);

        if (in.empty()) {
            break;
        }

        filter(0, 0);

        if (cv::waitKey(5) >= 0) {
            break;
        }
    }
}

void handleImage(int argc, char **argv)
{
    char *inputImage  = Utils::ArrayUtils::get_arg("-i", argv, argc);

    if (inputImage == NULL || config.outputImage == NULL) {
        printf("Invalid application usage:\n");

        printHelp();

        return;
    }

    if (!Utils::FileUtils::file_exists(inputImage)) {
        printf("Image file doesn't exist.\n");

        return;
    }

    in = cv::imread(inputImage, cv::COLOR_BGRA2BGR);
    if (!in.data) {
        printf("Couldn't read image.\n");

        return;
    }

    detector = cv::SimpleBlobDetector::create(createDetectionParams(argc, argv, &config));
    config.maxFrameUpdate = 0;

    if (config.verbose) {
        createTrackbars();
    }

    filter(0, 0);

    createImageLog(config.outputImage, out);

    if (config.verbose) {
        cv::waitKey(0);
    }
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

    bool isImage  = Utils::ArrayUtils::has_arg("-i", argv, argc);
    bool isCamera = Utils::ArrayUtils::has_arg("-c", argv, argc);

    config.verbose     = Utils::ArrayUtils::has_arg("--verbose", argv, argc);
    config.outputImage = Utils::ArrayUtils::get_arg("-o", argv, argc);

    if (!isImage && !isCamera) {
        printf("Invalid application usage:\n");

        printHelp();

        return -1;
    }

    if (config.verbose) {
        cv::namedWindow("Bubble Detection", cv::WINDOW_AUTOSIZE);
        cv::namedWindow("Settings", cv::WINDOW_AUTOSIZE);
    }

    if (isImage) {
        handleImage(argc, argv);
    } else {
        handleVideo(argc, argv);
    }

    return 0;
}
