#pragma once
/* Local */
#include "RGBVertex.h"

/* OpenCV */
// Due to some documentation warnings on MacOS platform we have to add this condition
#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#pragma clang pop
#else
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif

class RGBGraph
{
public:
    // Creates vector representing a graph of the image with weights between pixels
    RGBGraph(const cv::Mat& img);
    // Checks all surrounding pixels
    void GenerateRGBVertexEdges(RGBVertex& vertex, const cv::Mat& img);
    // Returns a vector of verticies represeting individual pixels
    std::vector<RGBVertex>& GetImageGraph();
    // Returns image in a cv::Mat format
    cv::Mat& GetImageMat();
    // Returns width of the image
    cv::Mat& GetImageMat(int clusterId);
    // Returns width of the image
    int GetWidth() const;
    // Returns height of the image
    int GetHeight() const;

private:
    bool m_connectDiagonals = false;
    cv::Mat* m_result;
    std::vector<RGBVertex> m_imageGraph;
    int m_originalWidth = 0;
    int m_originalHeight = 0;
};

