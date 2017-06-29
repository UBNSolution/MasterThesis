/* Local */
#include "AppCore.h"
#include "EuclideanClustering.h"
#include "EuclideanSeedGrow.h"
#include "GeodesicClustering.h"
#include "ResistanceClustering.h"

/* System */
#include <iostream>

/* Images defines */
#define IMAGES_FOLDER "images/"


AppCore* AppCore::m_instance = nullptr;

AppCore::AppCore(char* imgName) : m_imageName(std::string(imgName))
{
    m_originalImage = new cv::Mat(0, 0, CV_8UC3);
    m_image = new cv::Mat(0, 0, CV_8UC3);
}

void AppCore::EuclideanTrackbarCallback(int pos, void*)
{
    m_instance->m_euclideanThreshold = pos;
}

void AppCore::GeodesicTrackbarCallback(int pos, void*)
{
    m_instance->m_geodesicThreshold = pos;
}

void AppCore::ResistanceTrackbarCallback(int pos, void*)
{
    m_instance->m_resistanceThreshold = pos;
}

void AppCore::AppMain()
{
    // Loading of the image
    if (!LoadImage(IMAGES_FOLDER + m_imageName, *m_originalImage))
        throw std::runtime_error(std::string("Image could not be loaded. Make sure folder named \"images\" " 
                "is next to the application executable, and image to be loaded is inside the folder!"));

    // Creating copy of the original for editation
    *m_image = m_originalImage->clone();

    RefreshCvWindows();

    m_imageGraph = new RGBGraph(*m_originalImage);
    MainLoop();
    // Heap cleanup
    delete m_originalImage;
    m_originalImage = nullptr;
    delete m_image;
    m_image = nullptr;
    delete m_imageGraph;
    m_imageGraph = nullptr;
    delete m_clustering;
    m_clustering = nullptr;
}

bool AppCore::LoadImage(std::string imgPath, cv::Mat& outputMat)
{
    cv::Mat temp = cv::imread(imgPath, CV_LOAD_IMAGE_COLOR);

    if (temp.rows <= 0 || temp.cols <= 0)
    {
        // Image is not valid we are bailing right here
        return false;
    }

    outputMat.rows = temp.rows;
    outputMat.cols = temp.cols;
    outputMat = temp;
    m_image->cols = outputMat.cols;
    m_image->rows = outputMat.rows;
    return true;
}

void AppCore::MouseCallback(int event, int x, int y, int flags, void* userdata)
{
    switch (event) 
    {
        case  CV_EVENT_LBUTTONDOWN:
            m_instance->m_clicked = true;
            m_instance->m_tempP1.x = x;
            m_instance->m_tempP1.y = y;
            m_instance->m_tempP2.x = x;
            m_instance->m_tempP2.y = y;
            break;
        case  CV_EVENT_LBUTTONUP:
            if (x >= m_instance->m_originalImage->cols)
                x = m_instance->m_originalImage->cols - 1;

            if (y >= m_instance->m_originalImage->rows)
                y = m_instance->m_originalImage->rows - 1;

            m_instance->m_tempP2.x = x;
            m_instance->m_tempP2.y = y;
            m_instance->m_clicked = false;
            m_instance->AddRegion();
            break;
        case  CV_EVENT_MOUSEMOVE:
            if (m_instance->m_clicked)
            {
                m_instance->m_tempP2.x = x;
                m_instance->m_tempP2.y = y;
            }
            break;
        default:   
            break;
    }
}

void AppCore::AddRegion()
{
    cv::Rect tempRect(m_tempP1, m_tempP2);
    m_regions.push_back(tempRect);
    // Draw region
    cv::rectangle(*m_image, tempRect, cv::Scalar(cv::Vec3b(0, 255, 0)), 1, 1, 0);
    m_tempP1.x = 0;
    m_tempP1.y = 0;
    m_tempP2.x = 0;
    m_tempP2.y = 0;
    // Refresh the image
    cv::imshow("IMAGE", *m_image);
    std::cout << "Region added" << std::endl;
}

void AppCore::StartClustering()
{
    for (auto& v : m_imageGraph->GetImageGraph())
    {
        v.SetCluster(-1);
    }
    m_clustering->ClearSeeds();
    for (auto rect : m_regions)
    {
        m_clustering->AddSeed(*FindRegionMedian(rect));
    }
    m_clustering->StartSegmentation();
    RefreshCvWindows();
    // Show the clustered image
    for (int i = 0; i < m_clustering->GetClusterCount(); i++)
    {
        cv::imshow("CLUSTER - " + std::to_string(i),
                   m_imageGraph->GetImageMat(i));
    }
}

void AppCore::RefreshCvWindows()
{
    cv::destroyAllWindows();
    for (auto r : m_regions)
        cv::rectangle(*m_image, r, cv::Scalar(cv::Vec3b(0, 255, 0)), 1, 1, 0);
    // Create a window
    cv::namedWindow("IMAGE", CV_WINDOW_AUTOSIZE);
    // Set the callback function for any mouse event
    // We do not need a context here since we are working in a singleton
    cv::setMouseCallback("IMAGE", MouseCallback, nullptr);

    cv::createTrackbar("E", "IMAGE", &m_instance->m_euclideanThreshold, 
            m_euclideanThresholdLimit, EuclideanTrackbarCallback);
    cv::setTrackbarPos("E", "IMAGE", m_euclideanThreshold);

    cv::createTrackbar("G(x 0.01)", "IMAGE", &m_instance->m_geodesicThreshold, 
            m_geodesicThresholdLimit, GeodesicTrackbarCallback);
    cv::setTrackbarPos("G(x 0.01)", "IMAGE", m_geodesicThreshold);

    cv::createTrackbar("R(x 0.01)", "IMAGE", &m_instance->m_resistanceThreshold,
        m_resistanceThresholdLimit, ResistanceTrackbarCallback);
    cv::setTrackbarPos("R(x 0.01)", "IMAGE", m_resistanceThreshold);
    // Show the image
    cv::imshow("IMAGE", *m_image);
}

RGBVertex* AppCore::FindRegionMedian(const cv::Rect& region)
{
    double rChannelSum = 0;
    double gChannelSum = 0;
    double bChannelSum = 0;
    cv::Point center = (region.br() + region.tl()) * 0.5;

    for (int y = region.tl().y; y < region.br().y; y++)
    {
        for (int x = region.tl().x; x < region.br().x; x++)
        {
            rChannelSum += m_originalImage->at<cv::Vec3b>(y, x)[2];
            gChannelSum += m_originalImage->at<cv::Vec3b>(y, x)[1];
            bChannelSum += m_originalImage->at<cv::Vec3b>(y, x)[0];
        }
    }
    RGBVertex regionMean(
        center.x, center.y,
        (unsigned char)(rChannelSum / region.area()),
        (unsigned char)(gChannelSum / region.area()),
        (unsigned char)(bChannelSum / region.area())
    );
    
    // find median in the pixel in the region
    double smallestDistance = DOUBLE_MAX;
    RGBVertex* regionMedian = new RGBVertex(0, 0, 0, 0, 0);
    
    for (auto& v : m_imageGraph->GetImageGraph())
    {
        // Pixel is in region
        if (v.GetX() >= region.tl().x && v.GetX() < region.br().x
            && v.GetY() >= region.tl().y && v.GetY() < region.br().y)
        {
            double tempDistance = GetEuclideanDistance(regionMean, v);
            if (tempDistance < smallestDistance)
            {
                smallestDistance = tempDistance;
                regionMedian = &v;
            }
        }
    }
    
    return regionMedian;
}

AppCore* AppCore::GetInstance(char* imgName)
{
    if (m_instance == nullptr)
    {
        m_instance = new AppCore(imgName);
    }
    return m_instance;
}

void AppCore::MainLoop()
{
    std::cout << "Tips:" << std::endl;
    std::cout << "- Draw rectangles using mouse in the IMAGE window to select regions for segmentation" << std::endl;
    std::cout << "- Set threshold using slider for particlar algorithm" << std::endl;
    std::cout << "- Click number from options below to select segmentation method" << std::endl;
    std::cout << "----------------------------------------------------------------------------" << std::endl;
    std::cout << "[1] - EUCLIDEAN SEED GROW CLUSTERING" << std::endl;
    std::cout << "[2] - EUCLIDEAN CLUSTERING" << std::endl;
    std::cout << "[3] - GEODESIC CLUSTERING" << std::endl;
    std::cout << "[4] - RESISTANCE CLUSTERING" << std::endl;
    std::cout << "[5] - CLEAR REGIONS AND MEMORY" << std::endl;
    std::cout << "[0] - QUIT" << std::endl << std::endl;
    char input = '0';
    for(;;)
    {
        input = cv::waitKey();
        if (m_regions.size() > 0)
        {
            switch (input)
            {
            case '1':
            {
		if (m_clustering == nullptr)
                	m_clustering = new EuclideanSeedGrow(*m_imageGraph, m_euclideanThreshold);
		else
			m_clustering->SetThreshold(m_euclideanThreshold);

                std::cout << "Euclidean seed growing clustering started for " << m_regions.size() << " region(s) "
                    << "with threshold " << m_euclideanThreshold << std::endl;
                StartClustering();
                break;
            }
            case '2':
            {
		if (m_clustering == nullptr)
                	m_clustering = new EuclideanClustering(*m_imageGraph, m_euclideanThreshold);
		else
			m_clustering->SetThreshold(m_euclideanThreshold);

                std::cout << "Euclidean clustering started for " << m_regions.size() << " region(s) "
                    << "with threshold " << m_euclideanThreshold << std::endl;
                StartClustering();
                break;
            }
            case '3':
            {
                // Due to fact that OpenCV sliders can't do float values we need to keep them in
                // integer, here we will actually create a decimal point
		if (m_clustering == nullptr)
                	m_clustering = new GeodesicClustering(*m_imageGraph, (double)m_geodesicThreshold / 100);
		else
			m_clustering->SetThreshold((double)m_geodesicThreshold / 100);

                std::cout << "Geodesic clustering started for " << m_regions.size() << " region(s) "
                    << "with threshold " << (double)m_geodesicThreshold / 100 << std::endl;
                StartClustering();
                break;
            }
            case '4':
            {
		if (m_clustering == nullptr)
                	m_clustering = new ResistanceClustering(*m_imageGraph, (double)m_resistanceThreshold / 100);
		else
			m_clustering->SetThreshold((double)m_resistanceThreshold / 100);

                std::cout << "Resistance clustering started for " << m_regions.size() << " region(s) "
                    << "with threshold " << (double)m_resistanceThreshold / 100 << std::endl;
                StartClustering();
                break;
            }
            case '5':
            {
                m_regions.clear();

                if (m_clustering != nullptr)
                    m_clustering->ClearSeeds();

                delete m_clustering;
                m_clustering = nullptr;

                for (auto& v : m_imageGraph->GetImageGraph())
                {
                    v.SetCluster(-1);
                }
                // Refresh the image
                *m_image = m_originalImage->clone();
                RefreshCvWindows();

                std::cout << "Regions cleared" << std::endl;
                break;
            }
            case '0':
            {
                return;
            }
            default:
                std::cout << std::endl << "Invalid input!" << std::endl << std::endl;
                return;
            }
        }
        else if (input == '0' || input == -1)
        {
            return;
        }
        else
        {
            std::cout << "No regions selected!" << std::endl;
        }
    }
}

double AppCore::GetEuclideanDistance(RGBVertex& seed, RGBVertex& target)
{
    double tempSum = 0;
    tempSum += (pow(seed.GetR() - target.GetR(), 2));
    tempSum += (pow(seed.GetG() - target.GetG(), 2));
    tempSum += (pow(seed.GetB() - target.GetB(), 2));
    return sqrt(tempSum);
}
