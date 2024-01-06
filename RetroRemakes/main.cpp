#include <stdio.h>
#include <stdexcept>
#include <iostream>

#include "RRWindow.h"

using std::exception;
using std::cerr;
using std::endl;

int main() {
    RRWindow app;

    try {
        app.run();
    }
    catch (const exception& except) {
        cerr << except.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}