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

//
void convert_RSA_key_2_bytes();

void solve_task_1();
void solve_task_2();
void solve_task_3();
void solve_task_4();
void solve_task_5();
void solve_task_6();
/*void solve_task_7();
void solve_task_8();
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
            solve_task_3();
        } else if (opt == 4) {
            solve_task_4();
        } else if (opt == 5) {
            solve_task_5();
        } else if (opt == 6) {
            solve_task_6();
        } else if (opt == 7) {
            //solve_task_7();
        } else if (opt == 8) {
            //solve_task_8();
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


void solve_task_3()
{
    std::string hex = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    std::vector<std::string> results = find_single_byte(hex);
    std::cout << "Result.\n\t" << 
                 "Byte: " << results[0] << "\n\t"
                 "String: " << results[1] << "\n\t"
                 "Score: " << results[2] << "\n";
}


void solve_task_4()
{
    std::ifstream file (get_input_file_name());
    std::string temp = "";
    if (!file.is_open()) { 
        std::cout << "Error during opening the file!\n"; 
        return;
    }

    std::vector<std::vector<std::string>> best_results;
    while (std::getline(file, temp)) {
        std::vector<std::string> best_suitable_result = find_single_byte(temp);
        best_results.push_back(best_suitable_result);
    } 
    file.close();

    int best_score = 0;
    std::vector<std::string> best_result;
    int score = 0;
    for (std::vector<std::string> res : best_results) {
        score = std::stoi(res[2]);
        if (score > best_score) {
            best_result = res;
            best_score = score;
        }
    }

    std::cout << "Result:\n\t" << 
                 "Byte: " << best_result[0] << "\n\t"
                 "String: " << best_result[1] << "\n\t"
                 "Score: " << best_result[2] << "\n";
}


void solve_task_5()
{
    std::string text = "";
    read_from_file(text, get_input_file_name());
    std::string key = "";
    read_from_user("Enter key: ", key);

    std::string xored = repeating_XOR(text, key);
    std::cout << "Result: " << xored << "\n";
}


void solve_task_6()
{
    std::string text = "";
    std::string text_ASCII = "";
    read_from_file(text, get_input_file_name());

    bytes ciphertext_bytes; 
    base64_to_bytes(text, ciphertext_bytes);
    bytes_to_ASCII(ciphertext_bytes, text_ASCII);

    int key_size = guess_key_length(2, 40, ciphertext_bytes);
    std::cout << "Guessed key length: " << key_size << "\n";

    std::vector<bytes> ciphertext_blocks;
    split_into_blocks(ciphertext_bytes, key_size, ciphertext_blocks);


    std::vector<bytes> ciphertext_transposed_blocks;
    transpose_blocks(ciphertext_blocks, ciphertext_transposed_blocks);


    std::string key = "";
    std::string key_hex = "";
    for (bytes block : ciphertext_transposed_blocks) {
        std::string hex_block = "";
        bytes_to_hex(block, hex_block);
        key_hex += find_single_byte(hex_block)[0];
    }

    hex_to_ASCII(key_hex, key);
    std::cout << "Key: " << key << "\n";

    bytes key_bytes;
    ASCII_to_bytes(key, key_bytes);
    std::string plaintext = "";
    for (bytes& block : ciphertext_blocks) {
        for (int i = 0; i < block.size(); ++i) {
            block[i] ^= key_bytes[i];
        }
        for (unsigned char ch : block) {
            plaintext += (char)ch;
        }
    }

    write_to_file(plaintext, get_output_file_name());
}