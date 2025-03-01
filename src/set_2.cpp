#include "set_2.h"

#include "input_handlers.h"
#include "convertions.h"
#include "common.h"
#include "AES.h"

bytes KEY_TASK_12 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12, 0x14, 0x56, 0x00, 0x05, 0x50, 0x77, 0x1a, 0x3b};
bytes UNKNOWN_STRING_TASK_12;

void solve_task_9()
{
    std::string str = "YELLOW SUBMARINE";
    bytes key;
    ASCII_to_bytes(str, key);
    padding_PKCS7(key, 20);
    print_bytes(key);
}


void solve_task_10()
{
    std::string str = "YELLOW SUBMARINE";
    bytes key;
    ASCII_to_bytes(str, key);
    bytes text;
    str = "";
    read_from_file(str, get_input_file_name());
    base64_to_bytes(str, text);
    bytes iv {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bytes dec;
    params algorithm {4, 4, 10};
    decrypt_text_CBC(text, dec, key, iv, algorithm);
    str = "";
    bytes_to_ASCII(dec, str);
    write_to_file(str, get_output_file_name());
}


bytes encryption_oracle_11(const bytes& text)
{
    bytes ciphertext;
    bytes plaintext;
    plaintext.insert(plaintext.end(), text.begin(), text.end());

    bytes key = generate_random_bytes_sequence(16);
    std::cout << "Key: \n\t";
    print_bytes(key);

    int randNumBytesToAdd = generate_random_number(11);
    if (randNumBytesToAdd < 5) {
        randNumBytesToAdd += 5;
    }

    bytes front = generate_random_bytes_sequence(randNumBytesToAdd);
    plaintext.insert(plaintext.begin(), front.begin(), front.end());
    int textSizeReq = plaintext.size();
    while (textSizeReq % 16 != 0) {
        textSizeReq += 1;
    }

    bytes back = generate_random_bytes_sequence(textSizeReq - plaintext.size());
    plaintext.insert(plaintext.end(), back.begin(), back.end());
    std::cout << "Text after padding random bytes: (" << plaintext.size() << ")\n\t";
    print_bytes(plaintext);

    int mode = generate_random_number(2);
    params algorithm {4, 4, 10};

    if (mode == 0) {
        std::cout << "ECB mode has been choosen\n";
        encrypt_text_ECB(plaintext, ciphertext, key, algorithm);
    } else {
        std::cout << "CBC mode has been choosen\n";
        bytes IV = generate_random_bytes_sequence(16);
        encrypt_text_CBC(plaintext, ciphertext, key, IV, algorithm);
    }

    return ciphertext;
}


void solve_task_11()
{ 
    if (is_oracle_encrypt_ECB_mode(encryption_oracle_11)) {
        std::cout << "This ciphertext has been ecnrypted with ECB!\n\n";
    } else {
        std::cout << "This ciphertext has been ecnrypted with CBC!\n\n";
    }
    
}


bytes encryption_oracle_12(const bytes& text)
{
    bytes ciphertext;
    params algorithm {4, 4, 10};

    bytes plaintext;
    plaintext.insert(plaintext.end(), text.begin(), text.end());
    plaintext.insert(plaintext.end(), UNKNOWN_STRING_TASK_12.begin(), UNKNOWN_STRING_TASK_12.end());

    encrypt_text_ECB(plaintext, ciphertext, KEY_TASK_12, algorithm);

    return ciphertext;
}


void solve_task_12()
{
    KEY_TASK_12 = generate_random_bytes_sequence(16);

    std::string base64 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
    bytes additional_bytes;
    base64_to_bytes(base64, UNKNOWN_STRING_TASK_12);

    int block_size = discover_block_size(encryption_oracle_12);

    std::cout << "Size: " << block_size << "\n";

    //check ECB mode
    if (is_oracle_encrypt_ECB_mode(encryption_oracle_12)) {
        std::cout << "This ciphertext has been ecnrypted with ECB!\n\n";
    } else {
        std::cout << "This ciphertext has been ecnrypted with CBC!\n\n";
    }

    bytes test_len = {0x00};
    int x_str_len = encryption_oracle_12(test_len).size();
    //std::cout << "x len: " << x_str_len << "\n";
    int attack_block_start_ind = ((x_str_len / block_size) - 1) * 16;
    //std::cout << "ind attack block: " << attack_block_start_ind << "\n";
    bytes user_input (x_str_len, 0x00);
    bytes encrypted;
    bytes enc_attack_block;

    byte finded_byte;

    bytes x_text (x_str_len, 0x00);

    bytes temp;

    for (int i = 3; i > 0; --i) {
        user_input = slice(user_input, 1, user_input.size() - 1);
        x_text = slice(x_text, 1, x_text.size() - 1);
        //print_bytes(user_input);
        temp = slice(x_text, attack_block_start_ind, block_size - 1);

        std::map<bytes, byte> last_byte_dict = produce_last_byte_dict(temp, block_size, encryption_oracle_12);
        print_cipher_dict(last_byte_dict);

        //std::cout << "Plain user input: \n";
        //print_bytes(user_input);
        encrypted = encryption_oracle_12(user_input);
        //std::cout << "Encrypted user input: \n";
        //print_bytes(encrypted);

        enc_attack_block = slice(encrypted, attack_block_start_ind, block_size);
        print_bytes(enc_attack_block);

        finded_byte = last_byte_dict[enc_attack_block];
        std::cout << "Byte: " << format_hex(finded_byte) << "\n";

        x_text.push_back(finded_byte);

    }

    print_bytes(x_text);


}