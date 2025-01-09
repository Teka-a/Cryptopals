#include <iostream>
#include <vector>
#include <algorithm>


#include "input_handlers.h"

#include "set_1.h"


void print_menu();
int select_option();


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
            solve_task_3();
        } else if (opt == 4) {
            solve_task_4();
        } else if (opt == 5) {
            solve_task_5();
        } else if (opt == 6) {
            solve_task_6();
        } else if (opt == 7) {
            solve_task_7();
        } else if (opt == 8) {
            solve_task_8();
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
