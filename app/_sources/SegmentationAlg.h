#pragma once
/* Local */
#include "RGBGraph.h"

/* System */
#include <vector>

enum eSegmAlgOption
{
    EucliSeedGrowClustering,
    EucliClustering,
    GeoClustering,
    ResiClustering
};

class SegmentationAlg
{
public:
    SegmentationAlg(RGBGraph& img, double threshold);
    void StartSegmentation();
    virtual ~SegmentationAlg() {};
    void AddSeed(RGBVertex seed);
    void SetThreshold(double threshold);
    void ClearSeeds();
    int GetClusterCount();
protected:
    // Set image graph vertexes to their unclustered state
    void ClearClusters();
    virtual void FindCluster(RGBVertex& seed) = 0;
protected:
    RGBGraph* m_image = nullptr;
    double m_threshold = 0;
    std::vector<RGBVertex> m_seeds;
    /*
    Vector of vectors where each vector represents a clusters and
    keeps pointers individual pixels
    */
    std::vector<std::vector<RGBVertex*>> m_clusters;
    int m_clusterCount = 0;
};

