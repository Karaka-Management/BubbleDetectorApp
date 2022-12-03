/**
 * Karaka
 *
 * @package   Config
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef MODELS_CONFIG_H
#define MODELS_CONFIG_H

typedef struct {
    const int max_min_area = 1000;
    int new_min_area = 10;
    int old_min_area = 10;

    const int max_max_area = 1000;
    int new_max_area = 250;
    int old_max_area = 250;

    const int max_min_threshold = 100;
    int new_min_threshold = 10;
    int old_min_threshold = 10;

    const int max_max_threshold = 1000;
    int new_max_threshold = 250;
    int old_max_threshold = 250;

    const int max_step_threshold = 1000;
    int new_step_threshold = 100;
    int old_step_threshold = 100;

    const int max_min_distance = 500;
    int new_min_distance = 10;
    int old_min_distance = 10;

    const int max_min_circularity = 100;
    int new_min_circularity = 10;
    int old_min_circularity = 10;

    const int max_max_circularity = 100;
    int new_max_circularity = 100;
    int old_max_circularity = 100;

    const int max_min_convexity = 100;
    int new_min_convexity = 87;
    int old_min_convexity = 87;

    const int max_max_convexity = 100;
    int new_max_convexity = 100;
    int old_max_convexity = 100;

    const int max_min_inertia = 100;
    int new_min_inertia = 50;
    int old_min_inertia = 50;

    const int max_max_inertia = 100;
    int new_max_inertia = 100;
    int old_max_inertia = 100;

    bool logImage = false;
    int lastLog = 0;

    int lastUpdate = 0;
    int hasUpdate = false;
    char *outputImage;

    float volumeThreshold = 0.0;
    int countThreshold = 0;
    int maxFrameUpdate = 100;
    int minLogDelay = 500;

    bool verbose = false;
} app_config;

#endif