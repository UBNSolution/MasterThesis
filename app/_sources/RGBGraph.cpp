/* Local */
#include "RGBGraph.h"

RGBGraph::RGBGraph(const cv::Mat& img) :
    m_originalWidth(img.cols), m_originalHeight(img.rows)
{
    for (int y = 0; y < m_originalHeight; y++)
    {
        for (int x = 0; x < m_originalWidth; x++)
        {
            RGBVertex vertex(
                x,
                y,
                img.at<cv::Vec3b>(y, x).val[2],
                img.at<cv::Vec3b>(y, x).val[1],
                img.at<cv::Vec3b>(y, x).val[0]
            );
            m_imageGraph.push_back(vertex);
        }
    }

    for (auto& v : m_imageGraph)
    {
        GenerateRGBVertexEdges(v, img);
    }
}

void RGBGraph::GenerateRGBVertexEdges(RGBVertex& vertex, const cv::Mat& img)
{
    auto AddVertexNeighbour = [](int x, int y, std::vector<RGBVertex>& imageGraph,
        RGBVertex& vertex, const cv::Mat& img)
    {
        if (x >= 0 && y >= 0 && x < img.cols && y < img.rows)
            vertex.AddNeighbour(&(imageGraph.at(y * img.cols + x)));
        else
            vertex.AddNeighbour(nullptr);
    };
    int newX = 0;
    int newY = 0;
    // Left-Above
    if (m_connectDiagonals)
    {
        newX = vertex.GetX() - 1;
        newY = vertex.GetY() - 1;
        AddVertexNeighbour(newX, newY, m_imageGraph, vertex, img);
    }
    else
    {
        // This will add nullptr since x, y = -1 is outside of matrix
        AddVertexNeighbour(-1, -1, m_imageGraph, vertex, img);
    }
    // Above
    newX = vertex.GetX() + 0;
    newY = vertex.GetY() - 1;
    AddVertexNeighbour(newX, newY, m_imageGraph, vertex, img);

    if (m_connectDiagonals)
    {
        // Right-Above
        newX = vertex.GetX() + 1;
        newY = vertex.GetY() - 1;
        AddVertexNeighbour(newX, newY, m_imageGraph, vertex, img);
    }
    else
    {
        // This will add nullptr since x, y = -1 is outside of matrix
        AddVertexNeighbour(-1, -1, m_imageGraph, vertex, img);
    }
    // Right
    newX = vertex.GetX() + 1;
    newY = vertex.GetY() + 0;
    AddVertexNeighbour(newX, newY, m_imageGraph, vertex, img);

    if (m_connectDiagonals)
    {
        // Right-Bellow
        newX = vertex.GetX() + 1;
        newY = vertex.GetY() + 1;
        AddVertexNeighbour(newX, newY, m_imageGraph, vertex, img);
    }
    else
    {
        // This will add nullptr since x, y = -1 is outside of matrix
        AddVertexNeighbour(-1, -1, m_imageGraph, vertex, img);
    }
    // Bellow
    newX = vertex.GetX() + 0;
    newY = vertex.GetY() + 1;
    AddVertexNeighbour(newX, newY, m_imageGraph, vertex, img);

    if (m_connectDiagonals)
    {
        // Left-Bellow
        newX = vertex.GetX() - 1;
        newY = vertex.GetY() + 1;
        AddVertexNeighbour(newX, newY, m_imageGraph, vertex, img);
    }
    else
    {
        // This will add nullptr since x, y = -1 is outside of matrix
        AddVertexNeighbour(-1, -1, m_imageGraph, vertex, img);
    }
    // Left
    newX = vertex.GetX() - 1;
    newY = vertex.GetY() + 0;
    AddVertexNeighbour(newX, newY, m_imageGraph, vertex, img);
}

std::vector<RGBVertex>& RGBGraph::GetImageGraph()
{
    return m_imageGraph;
}

cv::Mat& RGBGraph::GetImageMat()
{
    m_result = new cv::Mat(m_originalHeight, m_originalWidth, CV_8UC3);
    for (auto vertex : m_imageGraph)
    {
        m_result->at<cv::Vec3b>(vertex.GetY(), vertex.GetX()) =
                cv::Vec3b(vertex.GetB(), vertex.GetG(), vertex.GetR());
    }
    return *m_result;
}

cv::Mat& RGBGraph::GetImageMat(int clusterId)
{
    m_result = new cv::Mat(m_originalHeight, m_originalWidth, CV_8UC3);
    for (auto vertex : m_imageGraph)
    {
        if (vertex.GetCluster() == clusterId)
        {
            m_result->at<cv::Vec3b>(vertex.GetY(), vertex.GetX()) =
                cv::Vec3b(vertex.GetB(), vertex.GetG(), vertex.GetR());
        }
        else
        {
            // Draw chessboard background
            const int tileSize = 16; // should be even number
            const unsigned char darkTileShade = 80;
            const unsigned char lightTileShade = 140;
            if (vertex.GetY() % tileSize < tileSize / 2)
            {
                if (vertex.GetX() % tileSize > (tileSize / 2) - 1)
                {
                    m_result->at<cv::Vec3b>(vertex.GetY(), vertex.GetX()) =
                        cv::Vec3b(darkTileShade, darkTileShade, darkTileShade);
                }
                else
                {
                    m_result->at<cv::Vec3b>(vertex.GetY(), vertex.GetX()) =
                        cv::Vec3b(lightTileShade, lightTileShade, lightTileShade);
                }
            }
            else
            {
                if (vertex.GetX() % tileSize <= (tileSize / 2) - 1)
                {
                    m_result->at<cv::Vec3b>(vertex.GetY(), vertex.GetX()) =
                        cv::Vec3b(darkTileShade, darkTileShade, darkTileShade);
                }
                else
                {
                    m_result->at<cv::Vec3b>(vertex.GetY(), vertex.GetX()) =
                        cv::Vec3b(lightTileShade, lightTileShade, lightTileShade);
                }
            }
        }
    }
    return *m_result;
}

int RGBGraph::GetWidth() const
{
    return m_originalWidth;
}

int RGBGraph::GetHeight() const
{
    return m_originalHeight;
}
