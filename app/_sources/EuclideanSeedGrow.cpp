/* Local */
#include "EuclideanSeedGrow.h"

void EuclideanSeedGrow::FindCluster(RGBVertex& seed)
{
    for (size_t i = 0; i < m_clusters[seed.GetCluster()].size(); i++)
    {
        for (auto edge : m_clusters[seed.GetCluster()].at(i)->GetEdges())
        {
            if (edge == nullptr)
                continue;

            RGBVertex tempVertex = *(edge->GetNeighbour());
            double tempDistance = GetEuclideanDistance(seed, tempVertex);

            if (tempDistance < m_threshold && tempVertex.GetCluster() == -1)
            {
                edge->GetNeighbour()->SetCluster(seed.GetCluster());
                m_clusters[seed.GetCluster()].push_back(edge->GetNeighbour());
            }
        }
    }
}

double EuclideanSeedGrow::GetEuclideanDistance(RGBVertex& seed, RGBVertex& target)
{
    double tempSum = 0;
    tempSum += (pow(seed.GetR() - target.GetR(), 2));
    tempSum += (pow(seed.GetG() - target.GetG(), 2));
    tempSum += (pow(seed.GetB() - target.GetB(), 2));
    return sqrt(tempSum);
}
