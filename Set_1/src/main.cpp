#include <iostream>

#include "convertions.h"
#include "common.h"


void print_menu();
int select_option();

void read_from_user(std::string, std::string&);
std::string get_input_file_name();
std::string get_output_file_name();
void read_from_file(std::string&, std::string);
void write_to_file(std::string&, std::string);



void solve_task_1();
void solve_task_2();
/*void solveTask3();
void solveTask4();
void solveTask5();
void solveTask6();
void solveTask7();
void solveTask8();
*/

int main() {
    print_menu();
    int opt = -1;
    while (opt != 0) {
        opt = select_option();
        if (opt == 1) {
            solve_task_1();
        } else if (opt == 2) {
            solve_task_2();
        } else if (opt == 3) {
            //solveTask3();
        } else if (opt == 4) {
            //solveTask4();
        } else if (opt == 5) {
            //solveTask5();
        } else if (opt == 6) {
            //solveTask6();
        } else if (opt == 7) {
            //solveTask7();
        } else if (opt == 8) {
            //solveTask8();
        }
    }

    return 0;
}


void print_menu()
{
    std::cout << "Set 1: Basics. \n\t"
                "1. Convert hex to base64\n\t"
                "2. Fixed XOR\n\t"
                "3. Single-byte XOR cipher\n\t"
                "4. Detect single-character XOR\n\t"
                "5. Implement repeating-key XOR\n\t"
                "6. Break repeating-key XOR\n\t"
                "7. AES in ECB mode\n\t"
                "8. Detect AES in ECB mode\n\t"
                "0. Exit\n";
}


int select_option()
{
    int opt = -1;
    const std::vector<std::string> options {"0", "1", "2", "3", "4", "5", "6", "7", "8"};
    while (opt == -1) {
        
        std::string option = "";
        read_from_user("To select option enter its number: ", option);

        auto it = find(options.begin(), options.end(), option);
        if (it != options.end()) {
            opt = (it - options.begin());
        } else {
            std::cout << "Try again!\n";
        }
    }  

    return opt;
}


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
        file_name = "input.txt";
    }

    return file_name;
}


std::string get_output_file_name()
{
    std::string file_name = "";

    read_from_user("Enter file name to write to: ", file_name);

    if (file_name == "") {
        file_name = "output.txt";
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

//--------------------------------------------------
// Task 1: Convert hex to base64
// 49 27 6d 20
// Convert hex to binary:
// 49 -> 01 00 10 01
// 27 -> 00 10 01 11
// 6d -> 01 10 11 01
// 20 -> 00 10 00 00
// Concatenate and divide into chunks by 6 bits
// (01 00 10) (01 00 10) (01 11 01) (10 11 01) (00 10 00) 00 - 2 bits left, so we have to add 00 00 to full chunk
// (01 00 10) (01 00 10) (01 11 01) (10 11 01) (00 10 00) (00 00 00)
// Convert binary into base64 (the result should be equal 0 by mod 4, if no - add '=')
//   (hex)    (dec) (char)
// -------------------
// 01 00 10 -> 18 -> S
// 01 00 10 -> 18 -> S
// 01 11 01 -> 29 -> d
// 10 11 01 -> 45 -> t
// -------------------
// 00 10 00 -> 8  -> I
// 00 00 00 -> 0  -> A
// -                 =
// -                 =
// -------------------
//--------------------------------------------------
void solve_task_1()
{
    std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    
    std::string base64 = "";
    hex_to_base64(hex, base64);

    std::cout << "Result: " << base64 << "\n";
}


void solve_task_2()
{
    std::string a = "1c0111001f010100061a024b53535009181c";
    std::string b = "686974207468652062756c6c277320657965";

    std::string res = XOR_hex_strs(a, b);

    std::cout << "Result: " << res << "\n";
}
