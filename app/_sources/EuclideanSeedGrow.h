#pragma once
/* Local */
#include "SegmentationAlg.h"


/*
    This segmentation algoritm using seed growing 
    approach with Euclidean distance
*/
class EuclideanSeedGrow : public SegmentationAlg
{
public:
    EuclideanSeedGrow(RGBGraph& img, double threshold)
        : SegmentationAlg(img, threshold) { };
    ~EuclideanSeedGrow() {};

private:
    // Does region growing
    void FindCluster(RGBVertex& seed) override;;
    // Returns Euclidean distance between N-dimensional data
    double GetEuclideanDistance(RGBVertex& seed, RGBVertex& target);
};
