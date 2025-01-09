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


void insert_symbol(std::string& str, char symbol, int count)
{
    while (count != 0) {
        str = symbol + str;
        count -= 1;
    }
}


bytes slice(bytes& vec, int start, int count)
{
    bytes sliced_vec;

    while (count != 0) {
        sliced_vec.push_back(vec[start]);
        ++start;
        --count;
    }

    return sliced_vec;
}


void split_into_blocks(bytes& orig, int size, std::vector<bytes>& blocks)
{
    int count = orig.size() / size;
    for (int i = 0; i < count; ++i) {
        blocks.push_back(slice(orig, i * size, size));
    }

    int diff = orig.size() - (size * count);
    if (diff != 0) {
        bytes last (size, 0x00);

        int ind_last = last.size() - 1;
        int ind_orig = orig.size() - 1;

        while (diff > 0) {
            last[ind_last] = orig[ind_orig];
            diff -= 1;
        }
    }

}


void transpose_blocks(std::vector<bytes>& orig, std::vector<bytes>& transposed)
{
    int rows = orig[0].size();
    int cols = orig.size();
    bytes temp (cols, 0x00);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            temp[col] = orig[col][row];
        }
        transposed.push_back(temp);
    }
}


bool compare_bytes(const bytes& a, const bytes& b)
{
    for (int i = 0; i < 16; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
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


void print_bytes(bytes& arr)
{
    std::cout << "Bytes: \n\t";
    for (unsigned char byte : arr) {
        std::cout << format_hex(byte) << " ";
    }
    std::cout << "\n";
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


int compute_hamming_distance(std::string bin_str_1, std::string bin_str_2)
{
    int dist = 0;

    int diff = bin_str_1.length() - bin_str_2.length();
    if (diff > 0) {
        insert_symbol(bin_str_2, '0', diff);
    } else if (diff < 0) {
        insert_symbol(bin_str_1, '0', diff * (-1));
    }

    int size = (bin_str_1.length() + bin_str_2.length()) / 2;

    for (int i = 0; i < size; ++i) {
        if (bin_str_1[i] != bin_str_2[i]) {
            dist += 1;
        }
    }

    return dist;
}


int guess_key_length(int min_length, int max_length, bytes& vec)
{
    std::map<int, double> key_length_probabilities;
    std::vector<bytes> blocks (4);
    
    for (int key_length = min_length; key_length <= max_length; ++key_length) {
        blocks[0] = slice(vec, 0, key_length);
        blocks[1] = slice(vec, key_length, key_length);
        blocks[2] = slice(vec, key_length * 2, key_length);
        blocks[3] = slice(vec, key_length * 3, key_length);
        double avgEditDist = 0.0;
        for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j) {
                std::string i_binary = "";
                std::string j_binary = "";
                bytes_to_binary(blocks[i], i_binary);
                bytes_to_binary(blocks[j], j_binary);
                avgEditDist += compute_hamming_distance(i_binary, j_binary);
            }
        }
        avgEditDist /= key_length;
        key_length_probabilities[key_length] = avgEditDist;
    }
    std::vector<std::pair<int, double>> pairs; 
  
    for (std::pair<int, double> it : key_length_probabilities) { 
        pairs.push_back(it); 
    } 
  
    sort(pairs.begin(), pairs.end(), [](auto& a, auto& b) { return a.second < b.second; }); 
    
    return pairs[0].first;
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