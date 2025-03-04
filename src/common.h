#pragma once
#ifndef COMMON_H
#define COMMON_H


#include "types.h"
#include "convertions.h"

void padding(std::string&, char, int, int);
void padding_bytes(bytes&, int);

void insert_symbol(std::string&, char, int);

bytes slice(bytes&, int, int);
void split_into_blocks(bytes&, int, std::vector<bytes>&);
void transpose_blocks(std::vector<bytes>&, std::vector<bytes>&);

bool compare_bytes(const bytes&, const bytes&);

std::string format_hex(byte);

void print_bytes(bytes&);
void print_cipher_dict(std::map<bytes, byte>&);

std::string XOR_hex_strs(std::string&, std::string&);
std::string repeating_XOR(std::string& plaintext, std::string& key);

std::vector<std::pair<char, int>> sort_map(std::map<char, int>);

int get_score(std::string);

std::vector<std::string> find_single_byte(std::string&);

int compute_hamming_distance(std::string, std::string);
int guess_key_length(int, int, bytes&);

int generate_random_number(int);
bytes generate_random_bytes_sequence(int);

bool contains_repeating_blocks(bytes&, int);
bool is_oracle_encrypt_ECB_mode(bytes (*oracle)(const bytes&));

int discover_block_size(bytes (*oracle)(const bytes&));

std::map<bytes, byte> produce_last_byte_dict(bytes, int, bytes (*oracle)(const bytes&));

std::string create_profile_for(std::string);
std::vector<std::pair<std::string, std::string>> parse(std::string&);

bool is_hex(std::string&);
bool is_ASCII(std::string&);
bool is_base64(std::string&);
bool is_binary(std::string&);

#endif // COMMON_H