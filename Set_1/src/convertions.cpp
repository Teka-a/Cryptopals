#include "convertions.h"
#include "common.h"

// Base64 <-> Bytes
void base64_to_bytes(std::string& base64, bytes& raw)
{
    raw = bytes();

    base64.erase(std::remove(base64.begin(), base64.end(), '='), base64.end());

    std::string binary = "";
    base64_to_binary(base64, binary);

    binary_to_bytes(binary, raw);
}


void bytes_to_base64(bytes& raw, std::string& base64)
{
    base64 = "";
    std::string binary = "";

    bytes_to_binary(raw, binary);

    binary_to_base64(binary, base64);
}


// Base64 <-> Hex
void base64_to_hex(std::string& base64, std::string& hex)
{
    hex = "";
    std::string binary = "";

    base64_to_binary(base64, binary);

    binary_to_hex(binary, hex);
}


void hex_to_base64(std::string& hex, std::string& base64)
{
    base64 = "";
    std::string binary = "";

    hex_to_binary(hex, binary);

    binary_to_base64(binary, base64);
}


// Base64 <-> Binary
void base64_to_binary(std::string& base64, std::string& binary)
{
    binary = "";

    for (char ch : base64) {
        long unsigned ind = base64_chars.find(ch);
        std::bitset<6> section(ind);
        binary += section.to_string();
    }

    //remove last 0's
    while (binary[binary.length() - 1] == '0') {
        binary.pop_back();
    }
}


void binary_to_base64(std::string& binary, std::string& base64)
{
    base64 = "";

    padding(binary, '0', 6, 1);

    for (int i = 0; i < binary.length(); i += 6) {
        std::bitset<6> char_ind {binary.substr(i, 6)};
        base64 += base64_chars[char_ind.to_ulong()];
    }

    int blocks = binary.length() / 6;

    if (blocks % 2 == 0 && blocks % 4 != 0) {
        base64 += "==";
    } else if (blocks % 3 == 0) {
        base64 += "=";
    }

}


// ASCII <-> Bytes
void ASCII_to_bytes(std::string& text, bytes& raw)
{
    raw = bytes();
    for (char ch : text) {
        raw.push_back(ch);
    }
}


void bytes_to_ASCII(bytes& raw, std::string& text)
{
    text = "";
    for (byte b : raw) {
        text += (char)b;
    }
}


// ASCII <-> Hex
void ASCII_to_hex(std::string& text, std::string& hex)
{
    hex = "";
    for (char ch : text) {
        hex += format_hex(ch);
    }
}


void hex_to_ASCII(std::string& hex, std::string& text)
{
    text = "";
    std::string part = "";

    for (int i = 0; i < hex.length(); i += 2) {
        part = hex.substr(i, 2);
        char ch = std::stoul(part, nullptr, 16);
        text += ch;
    }
}


// Hex <-> Bytes
void hex_to_bytes(std::string& str, bytes& vec)
{
    vec.clear();
    std::string binary = "";

    hex_to_binary(str, binary);

    binary_to_bytes(binary, vec);
}


void bytes_to_hex(bytes& vec, std::string& str)
{
    str = "";
    
    for (byte b : vec) {
        str += format_hex(b);
    }
}


// Hex <-> Binary
void hex_to_binary(std::string& hex, std::string& binary)
{
    binary = "";

    for (int i = 0; i < hex.length(); ++i) {
        std::bitset<4> b {(unsigned int)(0x00 ^ ((hex[i] > '9') ? hex[i] - 87 : hex[i]))};
        binary += b.to_string();
    }
}


void binary_to_hex(std::string& binary, std::string& hex)
{
    hex = ""; 
    padding(binary, '0', 4, 0);

    for (int i = 0; i < binary.length(); i += 4) {
        std::string bits = binary.substr(i, 4);
        std::bitset<4> hex_char (bits);
        std::stringstream ss;
        ss << std::hex << hex_char.to_ulong();
        hex += ss.str();
    }
}


// Binary <-> Bytes
void binary_to_bytes(std::string& binary, bytes& vec)
{
    vec.clear();
    padding(binary, '0', 8, 0);

    for (int i = 0; i < binary.length(); i += 8) {
        std::bitset<8> bits (binary.substr(i, 8));
        vec.push_back((unsigned char)bits.to_ulong());
    }
}


void bytes_to_binary(bytes& vec, std::string& binary)
{
    binary = "";
    
    for (byte b : vec) {
        std::bitset<8> bits (b);
        binary += bits.to_string();
    }
}
