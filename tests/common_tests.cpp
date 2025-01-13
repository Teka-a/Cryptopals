#include <gtest/gtest.h>
#include "common.h"


TEST(XOR_hex_strs, BasicAssertions)
{
    std::string a {"1c0111001f010100061a024b53535009181c"};
    std::string b {"686974207468652062756c6c277320657965"};
    std::string c {"746865206b696420646f6e277420706c6179"};
    
    std::string res = XOR_hex_strs(a, b);

    EXPECT_EQ(res, c);
}


TEST(padding_left, BasicAssertions)
{
    std::string a {"1111111111"};
    std::string c {"00000000001111111111"};
    
    padding(a, '0', 20, 0);

    EXPECT_EQ(a, c);
}


TEST(padding_right, BasicAssertions)
{
    std::string a {"1111111111"};
    std::string c {"11111111110000000000"};
    
    padding(a, '0', 20, 1);

    EXPECT_EQ(a, c);
}


TEST(padding_bytes, BasicAssertions)
{
    bytes test_vec {0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80};
    bytes res_vec {0x00, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80};

    padding_bytes(test_vec, 16);

    EXPECT_EQ(test_vec, res_vec);
}


TEST(insert_symbol, BasicAssertions)
{
    std::string a {"1111111111"};
    std::string c {"00000000001111111111"};
    
    insert_symbol(a, '0', 10);

    EXPECT_EQ(a, c);
}


TEST(slice, BasicAssertions)
{
    bytes test_vec {0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80};
    bytes sliced_vec = slice(test_vec, 3, 5);
    bytes res_vec {0x89, 0x56, 0x80, 0x89, 0x56};


    EXPECT_EQ(sliced_vec, res_vec);
}