#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <vector>
#include <map>


typedef unsigned char byte;
typedef std::vector<byte> bytes;
typedef byte block [16];
typedef byte matrix [4][4];
typedef byte word [4];

static const std::string hex_chars =
             "0123456789abcdefABCDEF";
             
static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

#endif // TYPES_H