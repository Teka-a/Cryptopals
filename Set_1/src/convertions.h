#pragma once
#ifndef CONVERTIONS_H
#define CONVERTIONS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <sstream>
#include <map>
#include <algorithm>

#include "types.h"



// Base64 <-> Bytes
void base64_to_bytes(std::string&, bytes&); 
void bytes_to_base64(bytes&, std::string&); 

// Base64 <-> Hex
void base64_to_hex(std::string&, std::string&); 
void hex_to_base64(std::string&, std::string&); 

// Base64 <-> Binary
void base64_to_binary(std::string&, std::string&); 
void binary_to_base64(std::string&, std::string&); 

// ASCII <-> Bytes
void ASCII_to_bytes(std::string&, bytes&); 
void bytes_to_ASCII(bytes&, std::string&); 

// ASCII <-> Hex
void ASCII_to_hex(std::string&, std::string&); 
void hex_to_ASCII(std::string&, std::string&); 

// Hex <-> Bytes
void hex_to_bytes(std::string&, bytes&); 
void bytes_to_hex(bytes&, std::string&); 

// Hex <-> Binary
void hex_to_binary(std::string&, std::string&); //
void binary_to_hex(std::string&, std::string&); //

// Binary <-> Bytes
void binary_to_bytes(std::string&, bytes&); //
void bytes_to_binary(bytes&, std::string&); //

#endif // CONVERTIONS_H