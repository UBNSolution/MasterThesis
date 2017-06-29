/* Local */
#include "RGBEdge.h"

/* System */
#include <cmath> // std::fabs

float RGBEdge::GetConductance() const
{
    return m_weight;
}

float RGBEdge::GetResistance() const
{
    return std::fabs(m_weight - 1);
}

RGBVertex* RGBEdge::GetNeighbour() const
{
    return m_neighbour;
}
