#include "MainWindowVM.h"

viewmodel::MainWindowVM* viewmodel::MainWindowVM::GetInstance()
{
    static MainWindowVM instance;
    return &instance;
}

void viewmodel::MainWindowVM::ChangeUI(viewmodel::AppStatus status)
{
    switch (status)
    {
    case NoImageLoaded:
        m_startSegmentationEnabled = false;
        m_startSegmentationEnabled = false;
        break;

    case PreComputationInProgress:
        m_startSegmentationEnabled = false;
        m_startSegmentationEnabled = false;
        break;

    case SegmentationReady:
        m_startSegmentationEnabled = true;
        m_startSegmentationEnabled = false;
        break;

    case SegmentationInProgress:
        m_startSegmentationEnabled = false;
        m_startSegmentationEnabled = true;
        break;
    }
    if (m_updateFunction != nullptr)
    {
        m_updateFunction();
    }
}

// Setters
void viewmodel::MainWindowVM::SetUpdateFunction(
        std::function<void(void)> &func)
{
    m_updateFunction = func;
}

// Getters
bool viewmodel::MainWindowVM::GetStartSegmentationEnabled()
{
    return m_startSegmentationEnabled;
}

bool viewmodel::MainWindowVM::GetStopSegmentationEnabled()
{
    return m_stopSegmentationEnabled;
}

viewmodel::MainWindowVM::MainWindowVM()
{

}
