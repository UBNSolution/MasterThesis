#pragma once
/* Local */
#include "SegmentationAlg.h"

/* System */
#include <string>


// Singleton class handling all application logic
class AppCore
{
public:
    void AppMain();
    static AppCore* GetInstance(char* imgName);

private:
    AppCore(char* imgName);
    bool LoadImage(std::string imgPath, cv::Mat& outputMat);
    static void MouseCallback(int event, int x, int y, int flags, void* userdata);
    static void EuclideanTrackbarCallback(int, void*);
    static void GeodesicTrackbarCallback(int, void*);
    static void ResistanceTrackbarCallback(int, void*);

    void AddRegion();
    void StartClustering();
    void RefreshCvWindows();
    // Calculates average value from area and selects center of
    // Rectangle a coordinates
    RGBVertex* FindRegionMedian(const cv::Rect& region);
    void MainLoop();

    // Returns Euclidean distance between N-dimensional data
    double GetEuclideanDistance(RGBVertex& seed, RGBVertex& target);
private:
    // List of regions selected by user
    std::vector<cv::Rect> m_regions;
    cv::Point m_tempP1;
    cv::Point m_tempP2;
    bool m_clicked = false;
    std::string m_imageName = "";

    // Threshold for clustering
    int m_euclideanThreshold = 80;
    int m_geodesicThreshold = 15;
    int m_resistanceThreshold = 50;
    const int m_euclideanThresholdLimit = 255;
    const int m_geodesicThresholdLimit = 100;
    const int m_resistanceThresholdLimit = 200;

    RGBGraph* m_imageGraph = nullptr;
    SegmentationAlg* m_clustering = nullptr;

    cv::Mat* m_originalImage = nullptr;
    cv::Mat* m_image = nullptr;

    static AppCore* m_instance;
};
