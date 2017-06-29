#pragma once
/* Local */
#include "SegmentationAlg.h"

/*
    This segmentation algoritm using seed growing 
    approach with Euclidean distance
*/
class ResistanceClustering : public SegmentationAlg
{
public:
    ResistanceClustering(RGBGraph& img, double threshold);
    ~ResistanceClustering();

private:
    // Goes through every pixel in the image
    void FindCluster(RGBVertex& seed) override;
    double MeasureDistance(RGBVertex& seed, RGBVertex& target);
    double MeasureDistanceEqSolve(RGBVertex& seed,
        RGBVertex& target);
    double MeasureDistancePseudoInverse(RGBVertex& seed,
        RGBVertex& target);
    void CalculateLaplacian();
    void RemoveRowAndCols(int rowIndex, int colIndex);
    float SumEdges(const RGBVertex& vertex);
private:
    cv::Mat* m_laplacian = nullptr;
    // Used by linear eq aproach
    cv::Mat* m_reducedLaplacian = nullptr;
    cv::Mat* m_reducedLaplacianInv = nullptr;
    cv::Mat* m_laplacianInv = nullptr;
    cv::Mat* m_potetials = nullptr;
    cv::Mat* m_currents = nullptr;
    // ---------------------------
    // Used by authors optimalized method
    cv::Mat* m_extCurrents = nullptr;
    cv::Mat* m_separatedLaplacian = nullptr;
    cv::Mat* m_separatedLaplacianInv = nullptr;
    cv::Mat* m_ijPotentials = nullptr;
};
