#include "set_1.h"

#include "input_handlers.h"
#include "convertions.h"
#include "common.h"
#include "AES.h"

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


void solve_task_7()
{
    std::string ciphertext_base64 = "";
    read_from_file(ciphertext_base64, get_input_file_name());
    
    bytes ciphertext;
    base64_to_bytes(ciphertext_base64, ciphertext);

    std::string key_ASCII = "YELLOW SUBMARINE";
    bytes key;
    ASCII_to_bytes(key_ASCII, key);

    params algorithm = {4, 4, 10};


    bytes plaintext;
    decrypt_text_ECB(ciphertext, plaintext, key, algorithm);

    std::string plaintext_ASCII = "";

    bytes_to_ASCII(plaintext, plaintext_ASCII);

    std::cout << "Result: " << plaintext_ASCII << "\n";

}


void solve_task_8()
{
    std::string file_name = get_input_file_name();

    std::ifstream file (file_name);
    std::vector<bytes> inputs;

    if (file.is_open()) {
        std::string temp = "";
        while (std::getline(file, temp)) {
            bytes temp_bytes;
            hex_to_bytes(temp, temp_bytes);
            inputs.push_back(temp_bytes);
        }
        file.close();
    } else {
        std::cout << "Error during opening the file!\n"; 
    }

    std::cout << inputs.size() << "\n";

    std::vector<bytes> suitable;
    for (bytes input : inputs) {
        std::vector<bytes> blocks;
        bool flag = false;
        
        for (int i = 0; i < input.size(); i += 16) {
            bytes temp = slice(input, i, 16);
            
            for (int j = 0; j < blocks.size(); ++j) {
                if (compare_bytes(temp, blocks[j]) && !flag) {
                    suitable.push_back(input);
                    flag = true;
                }
            }
            blocks.push_back(temp);
        }
    }

    for (int i = 0; i < suitable.size(); ++i) {
        std::cout << "Input №" << i << "(from suitable):\n";
        for (int j = 0; j < suitable[i].size(); j += 16) {
            bytes temp = slice(suitable[i], j, 16);
            print_bytes(temp);
        }
    }

}

