#include "set_2.h"

#include "input_handlers.h"
#include "convertions.h"
#include "common.h"
#include "AES.h"

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


bytes encryption_oracle_task11(bytes& text)
{
    bytes ciphertext;

    bytes key = generate_random_bytes_sequence(16);
    std::cout << "Key: \n\t";
    print_bytes(key);

    int randNumBytesToAdd = generate_random_number(11);
    if (randNumBytesToAdd < 5) {
        randNumBytesToAdd += 5;
    }

    bytes front = generate_random_bytes_sequence(randNumBytesToAdd);
    text.insert(text.begin(), front.begin(), front.end());
    int textSizeReq = text.size();
    while (textSizeReq % 16 != 0) {
        textSizeReq += 1;
    }
    bytes back = generate_random_bytes_sequence(textSizeReq - text.size());
    text.insert(text.end(), back.begin(), back.end());
    std::cout << "Text after padding random bytes: (" << text.size() << ")\n\t";
    print_bytes(text);

    int mode = generate_random_number(2);
    params algorithm {4, 4, 10};
    if (mode == 0) {
        std::cout << "ECB mode has been choosen\n";
        encrypt_text_ECB(text, ciphertext, key, algorithm);
    } else {
        std::cout << "CBC mode has been choosen\n";
        bytes IV = generate_random_bytes_sequence(16);
        encrypt_text_CBC(text, ciphertext, key, IV, algorithm);
    }


    return ciphertext;
}


void solve_task_11()
{
    bytes text {0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89,
                0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89,
                0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89};
    bytes enc = encryption_oracle_task11(text);
    std::cout << "Ciphertext: \n\t";
    print_bytes(enc);

    if (is_ECB(enc)) {
        std::cout << "This ciphertext has been ecnrypted with ECB!\n\n";
    } else {
        std::cout << "This ciphertext has been ecnrypted with CBC!\n\n";
    }

}
