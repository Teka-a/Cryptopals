#pragma once
#ifndef TYPES_H
#define TYPES_H


#include <vector>
#include <iostream>

typedef unsigned char byte;
typedef std::vector<byte> bytes;

static const std::string hex_chars =
             "0123456789abcdefABCDEF";
             
static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

#endif // TYPES_H