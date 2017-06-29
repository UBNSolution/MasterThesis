/* Local */
#include "GeodesicClustering.h"


void GeodesicClustering::FindCluster(RGBVertex& seed)
{
    seed.SetSeedDistance(0);
    RGBVertex* start = &seed;

    for (;;)
    {
        // Stopping the recursion when shortest paths to all vertexes are found
        if (m_veticesChecked >= m_image->GetImageGraph().size())
            break;

        m_veticesChecked++;

        for (auto& e : start->GetEdges())
        {
            // We are skipping nullptrs and vertexes with computed distances
            if (e == nullptr || e->GetNeighbour()->GetIsSeedDistanceFinal())
                continue;

            // If we got this far we have no-null vertex which distance hasn't been finilezed yet!
            // We will assing its distance as a sum of start vertex and edge between them

            // We are ignoring costly weights
            if (e->GetResistance() > m_threshold)
                continue;

            double edgeWeight = e->GetResistance() + start->GetSeedDistance();

            double seedDistance = e->GetNeighbour()->GetSeedDistance();

            // Is weight of the edge less then current tentitive distance in the node
            if (edgeWeight <= seedDistance)
            {
                // Rewrite current seed distance with the current weight in the edge
                e->GetNeighbour()->SetSeedDistance(edgeWeight);
                m_knownVertices.push_back(e->GetNeighbour());
            }
        }
        start->SetIsDistanceFinal(true);
        start->SetCluster(seed.GetCluster());
        m_visitedVertices.push_back(start);

        double smallestSeedDistance = DOUBLE_MAX;
        // Find neighbour with lowest seed distance
        for (size_t i = 0; i < m_knownVertices.size(); i++)
        {
            if (m_knownVertices[i]->GetSeedDistance() < smallestSeedDistance)
            {
                if (m_knownVertices[i]->GetIsSeedDistanceFinal() == false)
                {
                    smallestSeedDistance = m_knownVertices[i]->GetSeedDistance();
                    start = m_knownVertices[i];
                }
                else
                {
                    // if we find closed node we will erase it from vector since we don't need it
                    // this should keep our vector tight without thousands of values
                    m_knownVertices.erase(m_knownVertices.begin() + i--);
                }
            }
        }
    }
    // Reset the counter for next seed
    m_veticesChecked = 0;

    // Now we will clear all unclustered pixels
    for (auto& v : m_image->GetImageGraph())
    {
        if (v.GetCluster() == -1)
            v.SetDefault();
    }
}
