#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <iostream>

#include "types.h"

void padding(std::string&, char, int, int);
void padding_bytes(bytes&, int);

std::string format_hex(byte);

void print_bytes(bytes&);


bool is_hex(std::string&);
bool is_ASCII(std::string&);
bool is_base64(std::string&);
bool is_binary(std::string&);

#endif // COMMON_H