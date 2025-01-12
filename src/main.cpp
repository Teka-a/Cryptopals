#include <iostream>
#include <vector>
#include <algorithm>


#include "input_handlers.h"

#include "set_1.h"
#include "set_2.h"


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
        } else if (opt == 9) {
            solve_task_9();
        } else if (opt == 10) {
            solve_task_10();
        } else if (opt == 11) {
            solve_task_11();
        } else if (opt == 12) {
            //solve_task_12();
        } else if (opt == 13) {
            //solve_task_13();
        } else if (opt == 14) {
            //solve_task_14();
        } else if (opt == 15) {
            //solve_task_15();
        } else if (opt == 16) {
            //solve_task_16();
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
                "Set 2: Block crypto. \n\t"
                "9. Implement PKCS#7 padding\n\t"
                "10. Implement CBC mode\n\t"
                "11. An ECB/CBC detection oracle\n\t"
                "12. Byte-at-a-time ECB decryption (Simple)\n\t"
                "13. ECB cut-and-paste\n\t"
                "14. Byte-at-a-time ECB decryption (Harder)\n\t"
                "15. PKCS#7 padding validation\n\t"
                "16. CBC bitflipping attacks\n\t"
                "0 - to exit\n";
}


int select_option()
{
    int opt = -1;
    const std::vector<std::string> options {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
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
