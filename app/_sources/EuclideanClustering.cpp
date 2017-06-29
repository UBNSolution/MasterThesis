/* Local */
#include "EuclideanClustering.h"

void EuclideanClustering::FindCluster(RGBVertex& seed)
{
    for (auto& v : m_image->GetImageGraph())
    {
        double tempDistance = GetEuclideanDistance(seed, v);

        if (tempDistance < m_threshold && v.GetCluster() == -1)
        {
            v.SetCluster(seed.GetCluster());
            m_clusters[seed.GetCluster()].push_back(&v);
        }
    }
}

double EuclideanClustering::GetEuclideanDistance(RGBVertex& seed, RGBVertex& target)
{
    double tempSum = 0;
    tempSum += (pow(seed.GetR() - target.GetR(), 2));
    tempSum += (pow(seed.GetG() - target.GetG(), 2));
    tempSum += (pow(seed.GetB() - target.GetB(), 2));
    return sqrt(tempSum);
}
