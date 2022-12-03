/**
 * Karaka
 *
 * @package   Config
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef MODELS_HELPER_H
#define MODELS_HELPER_H

#include <stdio.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>

#include "../cOMS/Utils/ArrayUtils.h"

cv::SimpleBlobDetector::Params createDetectionParams(int argc, char **argv, app_config *config)
{
    if (config->new_step_threshold == 0) {
        config->new_step_threshold = 1;
    }

    cv::SimpleBlobDetector::Params params;

    // Color
    params.filterByColor = Utils::ArrayUtils::has_arg("--filterColor", argv, argc);

    // Change thresholds
	params.minThreshold = Utils::ArrayUtils::has_arg("-min_threshold", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-min_threshold", argv, argc)) : config->new_min_threshold;
	params.maxThreshold = Utils::ArrayUtils::has_arg("-max_threshold", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-max_threshold", argv, argc)) : config->new_max_threshold;
    params.thresholdStep = Utils::ArrayUtils::has_arg("-step_threshold", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-step_threshold", argv, argc)) / 100.0f : (config->new_step_threshold / 100.0f);

    params.minDistBetweenBlobs = Utils::ArrayUtils::has_arg("-min_distance", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-min_distance", argv, argc)) : config->new_min_distance;

	// Filter by Area.
	params.filterByArea = Utils::ArrayUtils::has_arg("--filterArea", argv, argc) || config->new_min_area > 0 || config->new_max_area > 0;
	params.minArea = Utils::ArrayUtils::has_arg("-min_area", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-min_area", argv, argc)) : config->new_min_area;
    params.maxArea = Utils::ArrayUtils::has_arg("-max_area", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-max_area", argv, argc)) : config->new_max_area;

	// Filter by Circularity
	params.filterByCircularity = Utils::ArrayUtils::has_arg("--filterCircularity", argv, argc) || config->new_min_circularity > 0.0 || config->new_max_circularity > 0.0;
	params.minCircularity = Utils::ArrayUtils::has_arg("-min_circularity", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-min_circularity", argv, argc)) / 100.0f : (config->new_min_circularity / 100.0f);
    params.maxCircularity = Utils::ArrayUtils::has_arg("-max_circularity", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-max_circularity", argv, argc)) / 100.0f : (config->new_max_circularity / 100.0f);

	// Filter by Convexity
	params.filterByConvexity = Utils::ArrayUtils::has_arg("--filterConvexity", argv, argc) || config->new_min_convexity > 0.0 || config->new_max_convexity > 0.0;
	params.minConvexity = Utils::ArrayUtils::has_arg("-min_convexity", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-min_convexity", argv, argc)) / 100.0f : (config->new_min_convexity / 100.0f);
    params.maxConvexity = Utils::ArrayUtils::has_arg("-max_convexity", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-max_convexity", argv, argc)) / 100.0f : (config->new_max_convexity / 100.0f);

	// Filter by Inertia
	params.filterByInertia = Utils::ArrayUtils::has_arg("--filterInertia", argv, argc) || config->new_min_inertia > 0.0 || config->new_max_inertia > 0.0;
	params.minInertiaRatio = Utils::ArrayUtils::has_arg("-min_inertia", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-min_inertia", argv, argc)) / 100.0f : (config->new_min_inertia / 100.0f);
    params.maxInertiaRatio = Utils::ArrayUtils::has_arg("-max_inertia", argv, argc) ? atoi(Utils::ArrayUtils::get_arg("-max_inertia", argv, argc)) / 100.0f : (config->new_max_inertia / 100.0f);

    config->old_min_area        = config->new_min_area;
    config->old_max_area        = config->new_max_area;
    config->old_min_threshold   = config->new_min_threshold;
    config->old_max_threshold   = config->new_max_threshold;
    config->old_step_threshold  = config->new_step_threshold;
    config->old_min_distance    = config->new_min_distance;
    config->old_min_circularity = config->new_min_circularity;
    config->old_max_circularity = config->new_max_circularity;
    config->old_min_convexity   = config->new_min_convexity;
    config->old_max_convexity   = config->new_max_convexity;
    config->old_min_inertia     = config->new_min_inertia;
    config->old_max_inertia     = config->new_max_inertia;

    return params;
}

inline
void createImageLog(char *path, cv::Mat image)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char buffer[1024];
    sprintf(buffer, "%s/%d-%d-%d_%d-%d-%d.png", path, (1900 + timeinfo->tm_year), timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    cv::imwrite(buffer, image);
}

#endif