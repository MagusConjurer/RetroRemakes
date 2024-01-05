// RemakeConsoleApp.cpp : This file contains the 'main' function. 
//

#include <stdio.h>
#include <stdexcept>
#include <iostream>

#include "RetroRemakeWindow.h"

using std::exception;
using std::cerr;
using std::endl;

int main()
{
    RetroRemakeWindow app;

    try {
        app.run();
    } catch (const exception& except) {
        cerr << except.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
