#pragma once
/* Local */
#include "RGBEdge.h"

/* System */
#include <vector>
#include <limits>

/* Defines */
#define DOUBLE_MAX std::numeric_limits<double>::max()


class RGBVertex
{
public:
    RGBVertex(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    
    // We are accepting a null pointer here for nonexisting neighbours for the
    // pixels at the edge of the image
    void AddNeighbour(RGBVertex* neighbour);
    void SetCluster(int clusterId);
    void SetSeedDistance(double seedDistance);
    void SetIsDistanceFinal(bool status);
    void SetDefault();
    // Getters
    int GetX() const;
    int GetY() const;
    int GetCluster() const;
    unsigned char GetR() const;
    unsigned char GetG() const;
    unsigned char GetB() const;
    unsigned char GetLuminosity() const;
    double GetSeedDistance() const;
    bool GetIsSeedDistanceFinal() const;
    std::vector<RGBEdge*> GetEdges() const;

private:
    int m_x = -1;
    int m_y = -1;
    int m_cluster = -1;
    double m_seedDistance = DOUBLE_MAX;
    bool m_seedDistanceIsFinal = false;
    unsigned char m_r = 0;
    unsigned char m_g = 0;
    unsigned char m_b = 0;
    unsigned char m_luminosity = 0;

    // Holds 8 pointers for every pixel to neighbouring pixels
    // Starts at left-top neighbour (index 0)
    // Going clockwise to left neighbour (index 7)
    // Nullptr means there is no pixel at that location
    std::vector<RGBEdge*> m_edges;
};
