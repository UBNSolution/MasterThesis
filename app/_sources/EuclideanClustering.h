#pragma once
/* Local */
#include "SegmentationAlg.h"


/*
    This segmentation algoritm is going through every pixel in the image,
    with no regards to edges or weights between the pixels.
    Pixel are compared with seed using the Euclidean distance
*/
class EuclideanClustering : public SegmentationAlg
{
public:
    EuclideanClustering(RGBGraph& img, double threshold)
        : SegmentationAlg(img, threshold) { };
    ~EuclideanClustering() {};

private:
    // Goes through every pixel in the image
    void FindCluster(RGBVertex& seed) override;
    // Returns Euclidean distance between N-dimensional data
    double GetEuclideanDistance(RGBVertex& seed, RGBVertex& target);
};

