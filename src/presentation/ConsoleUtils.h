#pragma once
#include <cstdlib>  // Required for std::system
#include <iostream> // Required for cin, cout
#include <string>   // Required for std::string
#include <limits>   // Required for numeric_limits

namespace ConsoleUtils
{
    // Automatically detects OS and clears the screen
    inline void clearScreen()
    {
#ifdef _WIN32
        std::system("cls"); // Windows
#else
        std::system("clear"); // Linux / Mac OS
#endif
    }

    // String Input (Prevents empty inputs)
    inline std::string getValidString(const std::string &prompt)
    {
        std::string input;
        while (true)
        {
            std::cout << prompt;
            std::getline(std::cin, input);

            
            if (!input.empty())
            {
                return input;
            }
            std::cout << " Error: This field cannot be empty. Please try again.\n";
        }
    }

    // Integer Input (Prevents letters, handles minimum values)
    inline int getValidInt(const std::string &prompt, int minValue = 0)
    {
        int value;
        while (true)
        {
            std::cout << prompt;

            
            if (std::cin >> value && value >= minValue)
            {
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }

            
            std::cin.clear();                                                   
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << " Error: Invalid input. Please enter a whole number greater than or equal to " << minValue << ".\n";
        }
    }

    // Double Input (For Money, Prices, and Fines)
    inline double getValidDouble(const std::string &prompt, double minValue = 0.0)
    {
        double value;
        while (true)
        {
            std::cout << prompt;

            if (std::cin >> value && value >= minValue)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << " Error: Invalid input. Please enter a decimal number greater than or equal to " << minValue << ".\n";
        }
    }
}