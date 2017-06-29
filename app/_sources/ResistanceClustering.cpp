/* Local */
#include "ResistanceClustering.h"
#include <iostream>
#include <iomanip>

ResistanceClustering::ResistanceClustering(RGBGraph& img, double threshold)
    : SegmentationAlg(img, threshold)
{
    m_laplacian = new cv::Mat(cv::Mat::zeros(m_image->GetWidth() * m_image->GetHeight(),
    m_image->GetWidth() * m_image->GetHeight(), CV_32F));

    m_extCurrents = new cv::Mat(cv::Mat::zeros(2, 1, CV_32F));
    m_extCurrents->at<float>(0, 0) = 1;
    m_extCurrents->at<float>(1, 0) = -1;

    m_ijPotentials = new cv::Mat(cv::Mat::zeros(2, 1, CV_32F));
    CalculateLaplacian();
}

void ResistanceClustering::FindCluster(RGBVertex& seed)
{
    for (size_t i = 0; i < m_clusters[seed.GetCluster()].size(); i++)
    {
        for (auto edge : m_clusters[seed.GetCluster()].at(i)->GetEdges())
        {
            if (edge == nullptr)
                continue;

            RGBVertex tempVertex = *(edge->GetNeighbour());
            double tempDistance = MeasureDistance(seed, tempVertex);

            if (tempDistance < m_threshold && tempVertex.GetCluster() == -1)
            {
                edge->GetNeighbour()->SetCluster(seed.GetCluster());
                m_clusters[seed.GetCluster()].push_back(edge->GetNeighbour());
            }
        }
    }
}

// Returns Resistance distance
// Author's shot to optimalize for image clustering purposes
// Method will try to seperate parts from the Laplacian matrix without 
// need for inverting the whole matrix
double ResistanceClustering::MeasureDistance(RGBVertex& seed, RGBVertex& target)
{
    int i = (seed.GetY() * m_image->GetWidth() + seed.GetX());
    int j = (target.GetY() * m_image->GetWidth() + target.GetX());
    m_separatedLaplacian = new cv::Mat(cv::Mat::zeros(2, 2, CV_32F));
    m_separatedLaplacianInv = new cv::Mat(cv::Mat::zeros(2, 2, CV_32F));

    m_separatedLaplacian->at<float>(0, 0) = m_laplacian->at<float>(i, i);
    m_separatedLaplacian->at<float>(0, 1) = m_laplacian->at<float>(i, j);
    m_separatedLaplacian->at<float>(1, 0) = m_laplacian->at<float>(j, i);
    m_separatedLaplacian->at<float>(1, 1) = m_laplacian->at<float>(j, j);
    *m_separatedLaplacianInv = m_separatedLaplacian->inv();
    *m_ijPotentials = *m_separatedLaplacianInv * (*m_extCurrents);


    double r = std::fabs(m_separatedLaplacianInv->at<float>(0, 0) - 
        2 *m_separatedLaplacianInv->at<float>(0, 1) 
            + m_separatedLaplacianInv->at<float>(1, 1));
    
    return r;

}

// This method is made exactly according the describtion in
// Diffusion - Based Image Segmentation Methods bt Jan Gaura Ph.D. (page 17)
double ResistanceClustering::MeasureDistanceEqSolve(RGBVertex& seed, RGBVertex& target)
{
    int i = (seed.GetY() * m_image->GetWidth() + seed.GetX());
    int j = (target.GetY() * m_image->GetWidth() + target.GetX());
    if (i == j)
        return 0;

    // Holds laplacian widthout i and j rows and cols
    m_reducedLaplacian = new cv::Mat(cv::Mat::zeros(m_image->GetWidth() * m_image->GetHeight(),
        m_image->GetWidth() * m_image->GetHeight(), CV_32F));
    *m_reducedLaplacian = m_laplacian->clone();
    // Holds inverted reduced laplacian
    m_reducedLaplacianInv = new cv::Mat(cv::Mat::zeros(m_image->GetWidth() * m_image->GetHeight() - 2,
        m_image->GetWidth() * m_image->GetHeight() - 2, CV_32F));
    // Holds all potentials
    m_potetials = new cv::Mat(cv::Mat::zeros(m_image->GetWidth() * m_image->GetHeight(), 1, CV_32F));
    // Holds all currents
    m_currents = new cv::Mat(cv::Mat::zeros(m_image->GetWidth() * m_image->GetHeight(), 1, CV_32F));

    // Set potentials to 1 and 0
    m_potetials->at<float>(i, 0) = 1.0f;
    m_potetials->at<float>(j, 0) = -1.0f;

    // Calculate currents based of Lf = r
    *m_currents = (*m_laplacian) * (*m_potetials);

    // Ommitting I and J rows and colums from Laplacian and potentials
    // Getting reduced Laplacian and vector of potentials
    if (i < j)
    {
        RemoveRowAndCols(j, j);
        RemoveRowAndCols(i, i);
    }
    else
    {
        RemoveRowAndCols(i, i);
        RemoveRowAndCols(j, j);
    }

    // Calculating inversion of reduced Laplacian
    *m_reducedLaplacianInv = m_reducedLaplacian->inv();

    // Calculating all potentials based on f = L^-1 * r
    *m_potetials = (*m_reducedLaplacianInv) * (*m_currents);

    std::cout << "Currents" << std::endl;
    // inverting potencials to current
    for (int x = 0; x < m_potetials->rows; x++)
    {
        m_currents->at<float>(x, 0) = 1 / m_potetials->at<float>(x, 0);
        std::cout << m_potetials->at<float>(x, 0) << "\t";
    }

    float r = 0;
    std::cout << std::endl << "Distance Target: " << target.GetY() << " " 
            << target.GetX() << " - " << r << std::endl;
    return r;

}

// Method is using the oficial aproach using pseudoinverse of laplacian metrics
// Method works only for very small images upto 30x30 afterwards Laplacian matrix
// becomes too large to invert using the Singular Value Decomposition
double ResistanceClustering::MeasureDistancePseudoInverse(RGBVertex& seed, RGBVertex& target)
{
    // TODO: This is to be moved to constructor so it is not computed again for next pair
    invert(*m_laplacian, *m_laplacianInv, cv::DECOMP_SVD);

    int i = (seed.GetY() * m_image->GetWidth() + seed.GetX());
    int j = (target.GetY() * m_image->GetWidth() + target.GetX());
    if (i == j)
        return 0;
    double r = m_laplacianInv->at<float>(i, i) - (2 * m_laplacianInv->at<float>(i, j)) + m_laplacianInv->at<float>(j, j);
    std::cout << "Distance Target: " << target.GetY() << " " << target.GetX() << " "
        << r << std::endl;
    return r;
}


void ResistanceClustering::CalculateLaplacian()
{
    for (int v = 0; v < (int)m_image->GetImageGraph().size(); v++)
    {
        // Place sum of edges on diagonal;
        m_laplacian->at<float>(v, v) = SumEdges(m_image->GetImageGraph()[v]);

        // Place weights in laplacian
        for (auto& e : m_image->GetImageGraph()[v].GetEdges())
        {
            if (e == nullptr)
                continue;

            int laplacianIndex = (e->GetNeighbour()->GetY() *
                m_image->GetWidth() + e->GetNeighbour()->GetX());

            // Matrix is symetric therefor we swap the coordinates
            m_laplacian->at<float>(laplacianIndex, v) = -e->GetConductance();
            m_laplacian->at<float>(v, laplacianIndex) = -e->GetConductance();
        }
    }
}

void ResistanceClustering::RemoveRowAndCols(int rowIndex, int colIndex)
{
    cv::Mat topPart;
    cv::Mat bottomPart;
    cv::Mat leftPart;
    cv::Mat rightPart;

    // We cut top and bottom around the row to be removed
    topPart = m_reducedLaplacian->rowRange(0, rowIndex);
    bottomPart = m_reducedLaplacian->rowRange(rowIndex + 1, m_reducedLaplacian->rows);
    if (topPart.rows == 0)
        cv::vconcat(bottomPart, *m_reducedLaplacian);
    else if (bottomPart.rows == 0)
        cv::vconcat(topPart, *m_reducedLaplacian);
    else
        cv::vconcat(topPart, bottomPart, *m_reducedLaplacian);

    // Removing values corresponding values from vector of potentials
    topPart = m_potetials->rowRange(0, rowIndex);
    bottomPart = m_potetials->rowRange(rowIndex + 1, m_potetials->rows);
    if (topPart.rows == 0)
        cv::vconcat(bottomPart, *m_potetials);
    else if (bottomPart.rows == 0)
        cv::vconcat(topPart, *m_potetials);
    else
        cv::vconcat(topPart, bottomPart, *m_potetials);

    // Removing values corresponding values from vector of currents
    topPart = m_currents->rowRange(0, rowIndex);
    bottomPart = m_currents->rowRange(rowIndex + 1, m_currents->rows);
    if (topPart.rows == 0)
        cv::vconcat(bottomPart, *m_currents);
    else if (bottomPart.rows == 0)
        cv::vconcat(topPart, *m_currents);
    else
        cv::vconcat(topPart, bottomPart, *m_currents);

    // We cut left and right around the col to be removed
    leftPart = m_reducedLaplacian->colRange(0, colIndex);
    rightPart = m_reducedLaplacian->colRange(colIndex + 1, m_reducedLaplacian->cols);
    if (leftPart.cols == 0)
        cv::vconcat(rightPart, *m_reducedLaplacian);
    else if (rightPart.cols == 0)
        cv::vconcat(leftPart, *m_reducedLaplacian);
    else
        cv::hconcat(leftPart, rightPart, *m_reducedLaplacian);

}

float ResistanceClustering::SumEdges(const RGBVertex& vertex)
{
    float sum = 0;
    for (auto& e : vertex.GetEdges())
    {
        if (e == nullptr)
            continue;
        sum += e->GetConductance();
    }
    return (float)sum;
}

ResistanceClustering::~ResistanceClustering()
{
    // Relase of matricies on the heap
    delete m_laplacian;
    delete m_reducedLaplacian;
    delete m_reducedLaplacianInv;
    delete m_laplacianInv;
    delete m_potetials;
    delete m_currents;
    delete m_extCurrents;
    delete m_separatedLaplacian;
    delete m_separatedLaplacianInv;
    delete m_ijPotentials;

    m_laplacian = nullptr;
    m_reducedLaplacian = nullptr;
    m_reducedLaplacianInv = nullptr;
    m_laplacianInv = nullptr;
    m_potetials = nullptr;
    m_currents = nullptr;
    m_extCurrents = nullptr;
    m_separatedLaplacian = nullptr;
    m_separatedLaplacianInv = nullptr;
    m_ijPotentials = nullptr;
}
