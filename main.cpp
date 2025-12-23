#include "Hospiditi.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    try {                           //allows program to catch any unwanted errors
        Hospiditi hospiditi;

        std::ifstream inFile("appointments.txt");           //loads existing data from file
        if (inFile) {
            try {

                hospiditi.Load(inFile);

            } catch (const std::exception& e) {
                std::cerr << "Failed to load appointments: " << e.what() << std::endl;
            }
            inFile.close();
            std::cout << "Loaded appointments.\n";
        }

        std::cout << "Welcome to Hospiditi Chatbot!\n Type (help) to start.\n";

        std::string statement;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, statement);

            std::string response = hospiditi.processQuestion(statement);
            if (response == "exit") break;                              //for exiting file and ending program
            std::cout << response << "\n";
        }

        std::ofstream outFile("appointments.txt");
        if (outFile) {
            try {
                hospiditi.Save(outFile);
                std::cout << "Appointments were successfully saved\n";

            } catch (const std::exception& e) {
                std::cerr << "Unable to save appointments: " << e.what() << std::endl; //finds any unhandled errors in the program
            }
            outFile.close();
        } else {
            std::cerr << "Unable to open appointments.txt for saving.\n";
        }

        std::cout << "Goodbye!\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
