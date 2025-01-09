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