/* Local */
#include "RGBVertex.h"

/* System */
#include <stdexcept>
#include <cmath> // std::pow(), exp()
#include <climits> // std::numeric_limits

#include <iostream> // remove


RGBVertex::RGBVertex(int x, int y, unsigned char r, unsigned char g, unsigned char b) :
        m_x(x), m_y(y), m_r(r), m_g(g), m_b(b), 
        m_luminosity((unsigned char)(m_r * 0.21 + m_g * 0.72 + m_b * 0.07))
{ 

}

void RGBVertex::AddNeighbour(RGBVertex* neighbour)
{
    // Calculate weight
    auto calcWeight = [](RGBVertex& v1, RGBVertex& v2)
    {
        const float sigma = 0.1f;

        float gaussian = exp(-((std::pow(std::fabs(((float)v1.GetLuminosity()) - 
                ((float)v2.GetLuminosity())), 2)) / 2 * (std::pow(sigma, 2))));
        return gaussian;
    };

    // If no neighbour exists we are still creating new entry in a our list of neighbours
    // so indexing remains always the same even for pixels at the edge of the image
    if (neighbour == nullptr)
    {
        m_edges.push_back(nullptr);
    }
    else
    {
        RGBEdge* temp = new RGBEdge(*neighbour, calcWeight(*this, *neighbour));
        m_edges.push_back(temp);
    }
}

void RGBVertex::SetCluster(int clusterId)
{
    m_cluster = clusterId;
}

void RGBVertex::SetSeedDistance(double seedDistance)
{
    if (!m_seedDistanceIsFinal)
    {
        m_seedDistance = seedDistance;
    }
    else
    {
        throw std::runtime_error("Distance for this vertex is set to be final, it can't be changed.");
    }
}

void RGBVertex::SetIsDistanceFinal(bool status)
{
    m_seedDistanceIsFinal = status;
}

void RGBVertex::SetDefault()
{
    m_seedDistanceIsFinal = false;
    m_seedDistance = DOUBLE_MAX;
}

int RGBVertex::GetX() const
{
    return m_x;
}

int RGBVertex::GetY() const
{
    return m_y;
}

int RGBVertex::GetCluster() const
{
    return m_cluster;
}

unsigned char RGBVertex::GetR() const
{
    return m_r;
}

unsigned char RGBVertex::GetG() const
{
   return m_g;
}

unsigned char RGBVertex::GetB() const
{
    return m_b;
}

unsigned char RGBVertex::GetLuminosity() const
{
    return m_luminosity;
}

double RGBVertex::GetSeedDistance() const
{
    return m_seedDistance;
}


bool RGBVertex::GetIsSeedDistanceFinal() const
{
    return m_seedDistanceIsFinal;
}

std::vector<RGBEdge*> RGBVertex::GetEdges() const
{
    return m_edges;
}
