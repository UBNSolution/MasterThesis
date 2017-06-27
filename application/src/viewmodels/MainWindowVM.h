#pragma once
#include <functional>

#include "AppStatus.h"
#include "../models/Log.h"

using namespace model;

namespace viewmodel
{

class MainWindowVM
{
public:
    static MainWindowVM* GetInstance();
    void ChangeUI(AppStatus status);

    // Setters
    void SetUpdateFunction(std::function<void(void)> &func);

    // Getters
    bool GetStartSegmentationEnabled();
    bool GetStopSegmentationEnabled();
private:
    MainWindowVM();

private:
    bool m_startSegmentationEnabled = false;
    bool m_stopSegmentationEnabled = false;
    std::vector<std::string>* m_log = nullptr;
    std::function<void(void)> m_updateFunction = nullptr;

};

}

