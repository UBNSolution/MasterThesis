#pragma once

// Forward declaration to dodge the cross inclusion problem
class RGBVertex;


class RGBEdge
{
public:
    RGBEdge(RGBVertex& neighbour, float weight) : m_neighbour(&neighbour), m_weight(weight) { };
    
    float GetConductance() const;
    float GetResistance() const;
    RGBVertex* GetNeighbour() const;
private:
    RGBVertex* m_neighbour = nullptr;
    // Stores conductance
    float m_weight = 0;
};
