#pragma once
/* Local */
#include "SegmentationAlg.h"

/*
    This segmentation algorithm is going through the image using
    Dijkstra algorithm, where seed is a start pixel and random pixel is
    the target pixel. Algoritm accepts pixel into the segment when weight between
    two compared pixels (not seed, but neighbouring pixel) 
    is smaller then the threshold
*/
class GeodesicClustering : public SegmentationAlg
{
public:
    GeodesicClustering(RGBGraph& img, double threshold)
        : SegmentationAlg(img, threshold) { };
    ~GeodesicClustering() {};

private:
    // Goes through every pixel in the image
    void FindCluster(RGBVertex& seed) override;
private:
    double m_veticesChecked = 0;
    std::vector<RGBVertex*> m_visitedVertices;
    std::vector<RGBVertex*> m_knownVertices;
};
