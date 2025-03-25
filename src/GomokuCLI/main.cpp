#include "GomokuCLI/GomokuCLI.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    try
    {
        GomokuCLI cli;
        cli.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }

    return 0;
}
