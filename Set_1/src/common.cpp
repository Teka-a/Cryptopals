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


std::string repeating_XOR(std::string& plaintext, std::string& key)
{
    std::string ciphertext = "";
    int mod = key.length();
    int diff = plaintext.length() - mod;

    if (diff > 0) {
        for (int i = 0; i < diff; ++i) {
            key += key[i % mod];
        }
    }

    std::string plaintext_hex = "";
    std::string key_hex = "";

    ASCII_to_hex(plaintext, plaintext_hex);
    ASCII_to_hex(key, key_hex);
    ciphertext = XOR_hex_strs(plaintext_hex, key_hex);

    return ciphertext;
}


std::vector<std::pair<char, int>> sort_map(std::map<char, int> mp)
{
    std::vector<std::pair<char, int>> pairs; 
  
    for (std::pair<char, int> it : mp) { 
        pairs.push_back(it); 
    } 
  
    sort(pairs.begin(), pairs.end(), [](auto& a, auto& b) { return a.second > b.second; }); 

    return pairs;
}


int get_score(std::string text)
{
    std::vector<char> first_13_chars {'e', 't', 'a', 'o', 'i', 'n', ' ', 's', 'h', 'r', 'd', 'l', 'u'};

    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c){ return std::tolower(c); });

    std::map<char, int> frequency;
    for (int i = 0; i < text.length(); ++i) { 
        if (frequency[text[i]]) {
            frequency[text[i]] += 1;
        } else {
            frequency[text[i]] = 1;
        }
    }

    std::vector<std::pair<char, int>> pairs = sort_map(frequency);
    std::vector<char> chars (13, '0');
    for (int i = 0; i < 13; ++i) {
        chars[i] = pairs[i].first;
    }

    int score = 0;
    for (char ch : chars) {
        int cnt = std::count(first_13_chars.begin(), first_13_chars.end(), ch); 
        if (cnt > 0) {
            score += frequency[ch];
        }
    }

    return score;
}

// {byte, string, score}
std::vector<std::string> find_single_byte(std::string& hex_str)
{
    int size_required = hex_str.length();

    std::vector<std::string> best_results (3, "0");
    int best_score = 0;

    for (int i = 0; i < 256; ++i) {
        std::string single_byte = format_hex((byte)i);
        std::string byte_str = single_byte;
        
        while (byte_str.length() < size_required) {
            byte_str += single_byte;
        }

        std::string xored = XOR_hex_strs(byte_str, hex_str);
        std::string result = "";
        hex_to_ASCII(xored, result);

        if (is_ASCII(result)) {
            int score = get_score(result);
            if (score > best_score) {
                best_results[0] = single_byte;
                best_results[1] = result;
                best_results[2] = std::to_string(score);
                best_score = score;
            }
        }           
    }

    return best_results;
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
