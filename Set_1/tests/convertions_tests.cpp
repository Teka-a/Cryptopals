#include <gtest/gtest.h>
#include "convertions.h"

TEST(hex_to_base64, BasicAssertions)
{
    std::string hex {"49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"};
    std::string base64;
    hex_to_base64(hex, base64);

    std::string base64_correct {"SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"};
    EXPECT_EQ(base64, base64_correct);
}