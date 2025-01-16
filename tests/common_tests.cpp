#include <gtest/gtest.h>
#include "common.h"


TEST(padding_left, BasicAssertions)
{
    std::string produced_padding {"1111111111"};
    
    padding(produced_padding, '0', 20, 0);

    std::string expected_padding {"00000000001111111111"};
    EXPECT_EQ(expected_padding, produced_padding);
}


TEST(padding_right, BasicAssertions)
{
    std::string produced_padding {"1111111111"};
    
    padding(produced_padding, '0', 20, 1);

    std::string expected_padding {"11111111110000000000"};
    EXPECT_EQ(expected_padding, produced_padding);
}


TEST(padding_bytes, BasicAssertions)
{
    bytes produced_padding {0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80};

    padding_bytes(produced_padding, 16);

    bytes expected_padding {0x00, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80};
    EXPECT_EQ(expected_padding, produced_padding);
}


TEST(insert_symbol, BasicAssertions)
{
    std::string produced_insert {"1111111111"};
    
    insert_symbol(produced_insert, '0', 10);

    std::string expected_insert {"00000000001111111111"};
    EXPECT_EQ(expected_insert, produced_insert);
}


TEST(slice, BasicAssertions)
{
    bytes vec {0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80};

    bytes produced_sliced = slice(vec, 3, 5);

    bytes expected_sliced {0x89, 0x56, 0x80, 0x89, 0x56};
    EXPECT_EQ(expected_sliced, produced_sliced);
}


TEST(split_into_blocks, BasicAssertions)
{
    bytes vec {0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80, 0x89, 0x56, 0x80};

    std::vector<bytes> produced_splitted;
    split_into_blocks(vec, 3, produced_splitted);

    std::vector<bytes> expected_splitted {{0x89, 0x56, 0x80}, {0x89, 0x56, 0x80}, {0x89, 0x56, 0x80}, {0x89, 0x56, 0x80}, {0x89, 0x56, 0x80}};
    EXPECT_EQ(expected_splitted, produced_splitted);
}


TEST(transpose_blocks, BasicAssertions)
{
    std::vector<bytes> arr {{0x89, 0x56, 0x80}, {0x89, 0x56, 0x80}, {0x89, 0x56, 0x80}, {0x89, 0x56, 0x80}, {0x89, 0x56, 0x80}};

    std::vector<bytes> produced_transposed;
    transpose_blocks(arr, produced_transposed);

    std::vector<bytes> expected_transposed {{0x89, 0x89, 0x89, 0x89, 0x89}, {0x56, 0x56, 0x56, 0x56, 0x56}, {0x80, 0x80, 0x80, 0x80, 0x80}};
    EXPECT_EQ(expected_transposed, produced_transposed);
}


TEST(compare_bytes, BasicAssertions)
{
    bytes a_vec {0x79, 0x89, 0x23, 0xf3, 0x2};
    bytes b_vec {0x79, 0x89, 0x23, 0xf3, 0x2};
    bytes c_vec {0x79, 0x89, 0x23, 0xf4, 0x25};

    bool equal_a_b = compare_bytes(a_vec, b_vec);
    bool equal_a_c = compare_bytes(a_vec, c_vec);

    bool produced_comparison = (equal_a_b && !equal_a_c) ? true : false;

    EXPECT_EQ(true, produced_comparison);
}


TEST(format_hex, BasicAssertions)
{
    char h = 0x89;

    std::string produced_hex = format_hex(h);

    std::string expected_hex = "89";
    EXPECT_EQ(expected_hex, produced_hex);
}


TEST(XOR_hex_strs, BasicAssertions)
{
    std::string a_str {"1c0111001f010100061a024b53535009181c"};
    std::string b_str {"686974207468652062756c6c277320657965"};
    
    std::string produced_XOR = XOR_hex_strs(a_str, b_str);

    std::string expected_XOR {"746865206b696420646f6e277420706c6179"};
    EXPECT_EQ(expected_XOR, produced_XOR);
}


TEST(repeating_XOR, BasicAssertions)
{
    std::string text {"Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal"};
    std::string key {"ICE"};
    
    std::string produced_XOR = repeating_XOR(text, key);

    std::string expected_XOR {"0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f"};
    EXPECT_EQ(expected_XOR, produced_XOR);
}



