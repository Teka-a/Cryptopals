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

