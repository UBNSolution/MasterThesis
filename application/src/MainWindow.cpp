#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_viewModel = viewmodel::MainWindowVM::GetInstance();
    m_viewModel->SetUpdateFunction(m_updateFunction);
    m_viewModel->ChangeUI(viewmodel::AppStatus::NoImageLoaded);

    ui->setupUi(this);
}

void MainWindow::UpdateControls()
{
    ui->btn_segmentationStart->
            setEnabled(m_viewModel->
                       GetStartSegmentationEnabled());

    ui->btn_segmentationStop->
            setEnabled(m_viewModel->
                       GetStopSegmentationEnabled());
}


MainWindow::~MainWindow()
{
    delete ui;
}
