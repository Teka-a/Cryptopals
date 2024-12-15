#include <sstream>

#include "common.h"

/*
    side = 0 - left-padding
    side = 1 - right-padding
*/
void padding(std::string& str, char symbol, int module, int side)
{
    if (side == 0) {
        while (str.length() % module != 0) {
            str = symbol + str;
        }
    } else if (side == 1) {
        while (str.length() % module != 0) {
            str += symbol;
        }
    }
}

void padding_bytes(bytes& vec, int required_size)
{
    while (vec.size() < required_size) {
        vec.insert(vec.begin(), 0x00);
    }
}


std::string format_hex(byte ch)
{
    std::string hex = "";

    std::stringstream ss;
    ss << std::hex << (int)ch;

    hex += ss.str();

    padding(hex, '0', 2, 0);

    return hex;
}


void printBytes(bytes& arr)
{
    std::cout << "Bytes: \n\t";

    for (unsigned char byte : arr) {
        std::cout << format_hex(byte) << " ";
    }

    std::cout << "\n";
}


std::string XOR_hex_strs(std::string& a, std::string& b)
{
    while (a.length() < b.length()) {
        a = "0" + a;
    }
    while (a.length() > b.length()) {
        b = "0" + b;
    }
    bytes a_bytes, b_bytes;
    hex_to_bytes(a, a_bytes);
    hex_to_bytes(b, b_bytes);

    for (int i = 0; i < a_bytes.size(); ++i) {
        a_bytes[i] ^= b_bytes[i];
    }

    a = "";
    bytes_to_hex(a_bytes, a);

    return a;
}



bool is_hex(std::string& text)
{
    for (char ch : text) {
        bool flag = false;
        for (char r : hex_chars) {
            if (ch == r) {
                flag = true;
            }
        }
        if (!flag) {
            return false;
        }
    }

    return true;
}


bool is_ASCII(std::string& text)
{
    for (int c : text) {
        if (c > 127 || c < 0 ) {
            return false;
        }
    }

    return true;
}


bool is_base64(std::string& text)
{
    for (char ch : text) {
        bool flag = false;
        for (char r : base64_chars) {
            if (ch == r) {
                flag = true;
            }
        }
        if (!flag) {
            return false;
        }
    }

    return true;
}


bool is_binary(std::string& text)
{
    for (int c : text) {
        if (c != '0' || c != '1' ) {
            return false;
        }
    }

    return true;
}
