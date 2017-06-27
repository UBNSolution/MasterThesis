#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewmodels/MainWindowVM.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void UpdateControls();

private:
    Ui::MainWindow *ui;
    viewmodel::MainWindowVM* m_viewModel = nullptr;
    std::function<void(void)> m_updateFunction;
};

#endif // MAINWINDOW_H
