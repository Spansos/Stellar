#pragma once

#include <iostream>

#define DO_LOGGING

#ifdef DO_LOGGING
    #define INFO_PRINT(info) do {std::cout << "[INFO] " << info << std::endl;} while(0)
    #define WARNING_PRINT(info) do {std::cout << "[WARING] " << info << std::endl;} while(0)
    #define ERROR_PRINT(info) do {std::cout << "[ERROR] " << info << std::endl;} while(0)
#else
    #define INFO_PRINT(info)
    #define WARNING_PRINT(info)
    #define ERROR_PRINT(info)
#endif