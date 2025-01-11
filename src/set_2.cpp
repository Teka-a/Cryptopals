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