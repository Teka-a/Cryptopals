#include "input_handlers.h"

#include <fstream>

void read_from_user(std::string text_to_print, std::string& text_to_read)
{
    std::cout << text_to_print;
    getline(std::cin, text_to_read);
}


std::string get_input_file_name()
{
    std::string file_name = "";

    read_from_user("Enter file name to read from: ", file_name);

    if (file_name == "") {
        file_name = "src/texts/input.txt";
    }

    return file_name;
}


std::string get_output_file_name()
{
    std::string file_name = "";

    read_from_user("Enter file name to write to: ", file_name);

    if (file_name == "") {
        file_name = "src/texts/output.txt";
    }

    return file_name;
}


void read_from_file(std::string& text, std::string file_name)
{
    std::ifstream file (file_name);
    if (file.is_open()) {
        std::string temp = "";
        while (std::getline(file, temp)) {
            text += temp;
        }
        file.close();
    } else {
        std::cout << "Error during opening the file!\n"; 
    }
}


void write_to_file(std::string& text, std::string file_name)
{
    std::ofstream file (file_name, std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        file << text;
        file.close();
    } else {
        std::cout << "Error during opening the file!\n"; 
    }
}
