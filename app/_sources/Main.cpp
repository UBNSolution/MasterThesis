/* System */
#include <iostream>

/* Local */
#include "AppCore.h"

int main(int argc, char** argv)
{
    std::cout << "MASTER THESIS APPLICATION: \t" << "Measuring the Distances for the Use in Image Segmentation" << std::endl;
    std::cout << "YEAR: \t\t\t\t" << "2017" << std::endl;
    std::cout << "AUTHOR: \t\t\t" << "Bc. Marek Urban (URB0061)" << std::endl;
    std::cout << "SUPERVISOR: \t\t\t" << "doc. Dr. Ing. Eduard Sojka" << std::endl;
    std::cout << std::endl;
    if (argc <= 1)
    {
        std::cout << "Run application with name of the image to load as an argument!" 
                << std::endl;
    }
    else
    {
        AppCore* core = AppCore::GetInstance(argv[1]);
        try
        {
            core->AppMain();
            std::cout << "Application has ended sucessufully." << std::endl;
        }
        catch (std::runtime_error e)
        {
            std::cout << "Error has occurred: \"" << e.what() << "\""
                << std::endl << "Process has been terminated! Click Enter to close." << std::endl;
            std::cin.get();
        }
    }
    return 0;
}
