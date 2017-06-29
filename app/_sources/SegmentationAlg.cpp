/* Local */
#include "SegmentationAlg.h"

SegmentationAlg::SegmentationAlg(RGBGraph& img, double threshold) : 
        m_image(&img), m_threshold(threshold) { }

void SegmentationAlg::StartSegmentation()
{
    ClearClusters();
    for (auto s : m_seeds)
    {
        FindCluster(s);
    }
}

void SegmentationAlg::ClearClusters()
{
    for (auto& v : m_image->GetImageGraph())
    {
        v.SetDefault();
        v.SetCluster(-1);
    }
    for (auto& s : m_seeds)
    {
        s.SetDefault();
    }
}

void SegmentationAlg::SetThreshold(double threshold)
{
    m_threshold = threshold;
}

void SegmentationAlg::AddSeed(RGBVertex seed)
{
    RGBVertex* tempPoint = &(m_image->GetImageGraph().at((seed.GetY() * m_image->GetWidth() + seed.GetX())));
    tempPoint->SetCluster(m_clusterCount);
    seed.SetCluster(m_clusterCount);
    m_seeds.push_back(seed);
    m_clusters.push_back(*new std::vector<RGBVertex*>);
    m_clusters[m_clusterCount++].push_back(tempPoint);
}

void SegmentationAlg::ClearSeeds()
{
    m_seeds.clear();
    m_clusterCount = 0;
    for (auto c : m_clusters)
    {
        c.clear();
    }
    m_clusters.clear();
}

int SegmentationAlg::GetClusterCount()
{
    return m_clusterCount;
}
