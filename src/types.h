#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>


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

struct VectorHash {
    template <typename T>
    std::size_t operator ()(const std::vector<T>& vec) const {
        std::size_t seed = 0;
        for (const T& elem : vec) {
            seed ^= std::hash<T>{}(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);  // Универсальный хэш
        }
        return seed;
    }
};

#endif // TYPES_H