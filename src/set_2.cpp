#include "set_2.h"

#include "input_handlers.h"
#include "convertions.h"
#include "common.h"
#include "AES.h"

bytes KEY_TASK_12 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12, 0x14, 0x56, 0x00, 0x05, 0x50, 0x77, 0x1a, 0x3b};
bytes UNKNOWN_STRING_TASK_12 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12, 0x14, 0x56, 0x00, 0x05};

bytes KEY_TASK_13 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12, 0x14, 0x56, 0x00, 0x05, 0x50, 0x77, 0x1a, 0x3b};

bytes KEY_TASK_14 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12, 0x14, 0x56, 0x00, 0x05, 0x50, 0x77, 0x1a, 0x3b};
bytes UNKNOWN_STRING_TASK_14 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12, 0x14, 0x56, 0x00, 0x05};
bytes RANDOM_PREFIX_TASK_14 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12, 0x14, 0x56, 0x00, 0x05};


bytes KEY_TASK_16 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12, 0x14, 0x56, 0x00, 0x05, 0x50, 0x77, 0x1a, 0x3b};
bytes IV_TASK_16 {0x01, 0x45, 0x18, 0xff, 0x0f, 0x0e, 0x23, 0x12};



void solve_task_9()
{
    std::string str = "YELLOW SUBMARINE";
    bytes key;
    ASCII_to_bytes(str, key);
    padding_PKCS7(key, 20);
    print_bytes(key);
}


void solve_task_10()
{
    std::string str = "YELLOW SUBMARINE";
    bytes key;
    ASCII_to_bytes(str, key);
    bytes text;
    str = "";
    read_from_file(str, get_input_file_name());
    base64_to_bytes(str, text);
    bytes iv {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bytes dec;
    params algorithm {4, 4, 10};
    decrypt_text_CBC(text, dec, key, iv, algorithm);
    str = "";
    bytes_to_ASCII(dec, str);
    write_to_file(str, get_output_file_name());
}


bytes encryption_oracle_11(const bytes& text)
{
    bytes ciphertext;
    bytes plaintext;
    plaintext.insert(plaintext.end(), text.begin(), text.end());

    bytes key = generate_random_bytes_sequence(16);
    std::cout << "Key: \n\t";
    print_bytes(key);

    int randNumBytesToAdd = generate_random_number(11);
    if (randNumBytesToAdd < 5) {
        randNumBytesToAdd += 5;
    }

    bytes front = generate_random_bytes_sequence(randNumBytesToAdd);
    plaintext.insert(plaintext.begin(), front.begin(), front.end());
    int textSizeReq = plaintext.size();
    while (textSizeReq % 16 != 0) {
        textSizeReq += 1;
    }

    bytes back = generate_random_bytes_sequence(textSizeReq - plaintext.size());
    plaintext.insert(plaintext.end(), back.begin(), back.end());
    std::cout << "Text after padding random bytes: (" << plaintext.size() << ")\n\t";
    print_bytes(plaintext);

    int mode = generate_random_number(2);
    params algorithm {4, 4, 10};

    if (mode == 0) {
        std::cout << "ECB mode has been choosen\n";
        encrypt_text_ECB(plaintext, ciphertext, key, algorithm);
    } else {
        std::cout << "CBC mode has been choosen\n";
        bytes IV = generate_random_bytes_sequence(16);
        encrypt_text_CBC(plaintext, ciphertext, key, IV, algorithm);
    }

    return ciphertext;
}


void solve_task_11()
{ 
    if (is_oracle_encrypt_ECB_mode(encryption_oracle_11)) {
        std::cout << "This ciphertext has been ecnrypted with ECB!\n\n";
    } else {
        std::cout << "This ciphertext has been ecnrypted with CBC!\n\n";
    }
    
}


bytes encryption_oracle_12(const bytes& text)
{
    bytes ciphertext;
    params algorithm {4, 4, 10};

    bytes plaintext;
    plaintext.insert(plaintext.end(), text.begin(), text.end());

    plaintext.insert(plaintext.end(), UNKNOWN_STRING_TASK_12.begin(), UNKNOWN_STRING_TASK_12.end());

    encrypt_text_ECB(plaintext, ciphertext, KEY_TASK_12, algorithm);

    return ciphertext;
}


void solve_task_12()
{
    KEY_TASK_12 = generate_random_bytes_sequence(16);

    std::string base64 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

    base64_to_bytes(base64, UNKNOWN_STRING_TASK_12);

    int block_size = discover_block_size(encryption_oracle_12);

    std::cout << "Size: " << block_size << "\n";

    //check ECB mode
    if (is_oracle_encrypt_ECB_mode(encryption_oracle_12)) {
        std::cout << "This ciphertext has been ecnrypted with ECB!\n\n";
    } else {
        std::cout << "This ciphertext has been ecnrypted with CBC!\n\n";
    }

    bytes test_len = {0x00};
    int x_str_len = encryption_oracle_12(test_len).size();

    int attack_block_start_ind = ((x_str_len / block_size) - 1) * 16;

    bytes user_input (x_str_len, 0x00);
    bytes encrypted;
    bytes enc_attack_block;


    byte finded_byte;

    bytes x_text (x_str_len, 0x00);

    bytes temp;

    for (int i = x_str_len; i > 0; --i) {
        user_input = slice(user_input, 1, user_input.size() - 1);
        x_text = slice(x_text, 1, x_text.size() - 1);

        temp = slice(x_text, attack_block_start_ind, block_size - 1);
        std::map<bytes, byte> last_byte_dict = produce_last_byte_dict(temp, block_size, 0, encryption_oracle_12);
        
        encrypted = encryption_oracle_12(user_input);

        enc_attack_block = slice(encrypted, attack_block_start_ind, block_size);

        finded_byte = last_byte_dict[enc_attack_block];

        x_text.push_back(finded_byte);
    }


    std::string ascii = "";
    bytes_to_ASCII(x_text, ascii);
    std::cout << "Target string: \n" << ascii << "\n";
}


bytes encryption_oracle_13(const bytes& text)
{
    bytes ciphertext;
    params algorithm {4, 4, 10};

    bytes plaintext;
    plaintext.insert(plaintext.end(), text.begin(), text.end());

    encrypt_text_ECB(plaintext, ciphertext, KEY_TASK_13, algorithm);

    return ciphertext;
}


bool is_profile_has_admin_role(const bytes& text)
{
    bytes plaintext;
    bytes ciphertext;

    params algorithm {4, 4, 10};

    ciphertext.insert(ciphertext.end(), text.begin(), text.end());

    decrypt_text_ECB(ciphertext, plaintext, KEY_TASK_13, algorithm);

    std::string profile = "";
    bytes_to_ASCII(plaintext, profile);
    std::cout << "Decrypted: "  << profile << "\n";

    std::vector<std::pair<std::string, std::string>> profile_dict = parse(profile);

    bool is_admin_role = false;
    for (std::pair<std::string, std::string> pair : profile_dict) {
        if (pair.first == "role") {
            if (pair.second == "admin") {
                is_admin_role = true;
            }
        }
    }


    return is_admin_role;
}


/*
    email=foo@bar.co
    m&uid=10&role=us
    admin(PKCS7)

    email=foo@bar.co
    admin(PKCS7)
    m00&uid=10&role=
    profile_for(foo@bar.coadmin(PKCS7)m00)
    Change C3 with C2

    e4 69 6e e4 99 9e 8b 38 a9 36 dc f3 58 5f f0 b1
    a4 ec 41 0b 88 37 1b 18 dd 1b 77 d8 0d 7d f0 f1 
    72 42 48 5a 73 9a 26 d8 92 3b ec 8b 5a 71 2d cb
*/
void solve_task_13()
{
    KEY_TASK_13 = generate_random_bytes_sequence(16);

    bytes profile_ciphertext_admin_midle;
    bytes profile_ciphertext_orig;

    bytes profile_plaintext_admin_midle;
    bytes profile_plaintext_orig;

    std::string admin_midle = "foo@bar.co";
    bytes admin_midle_bytes;
    ASCII_to_bytes(admin_midle, admin_midle_bytes);

    std::string admin = "admin";
    bytes admin_bytes;
    ASCII_to_bytes(admin, admin_bytes);
    padding_PKCS7(admin_bytes, 16);
    admin_midle_bytes.insert(admin_midle_bytes.end(), admin_bytes.begin(), admin_bytes.end());

    bytes x;
    std::string end_ascii = "m00";
    ASCII_to_bytes(end_ascii, x);
    admin_midle_bytes.insert(admin_midle_bytes.end(), x.begin(), x.end());
    bytes_to_ASCII(admin_midle_bytes, admin_midle);


    std::string profile_admin_midle = create_profile_for(admin_midle); 
    ASCII_to_bytes(profile_admin_midle, profile_plaintext_admin_midle);  
    profile_ciphertext_admin_midle = encryption_oracle_13(profile_plaintext_admin_midle);

    std::string profile_orig = create_profile_for("foo@bar.com00");
    ASCII_to_bytes(profile_orig, profile_plaintext_orig);
    profile_ciphertext_orig = encryption_oracle_13(profile_plaintext_orig);


    bytes c1_mid = slice(profile_ciphertext_admin_midle, 0, 16);
    bytes c2_mid = slice(profile_ciphertext_admin_midle, 16, 16);
    bytes c3_mid = slice(profile_ciphertext_admin_midle, 32, 16);
    bytes c4_mid = slice(profile_ciphertext_admin_midle, 48, 16);

    bytes c1_orig = slice(profile_ciphertext_orig, 0, 16);
    bytes c2_orig = slice(profile_ciphertext_orig, 16, 16);
    bytes c3_orig = slice(profile_ciphertext_orig, 32, 16);

    bytes changed;
    changed.insert(changed.end(), c1_orig.begin(), c1_orig.end());
    changed.insert(changed.end(), c2_orig.begin(), c2_orig.end());
    changed.insert(changed.end(), c2_mid.begin(), c2_mid.end());

    if (is_profile_has_admin_role(changed)) {
        std::cout << "Profile has admin role!\n";
    } else {
        std::cout << "Profile has user role!\n";
    }
}


bytes encryption_oracle_14(const bytes& text)
{
    bytes ciphertext;
    params algorithm {4, 4, 10};

    bytes plaintext;
    plaintext.insert(plaintext.end(), RANDOM_PREFIX_TASK_14.begin(), RANDOM_PREFIX_TASK_14.end());

    plaintext.insert(plaintext.end(), text.begin(), text.end());

    plaintext.insert(plaintext.end(), UNKNOWN_STRING_TASK_14.begin(), UNKNOWN_STRING_TASK_14.end());

    encrypt_text_ECB(plaintext, ciphertext, KEY_TASK_14, algorithm);

    return ciphertext;
}


void solve_task_14()
{
    KEY_TASK_14 = generate_random_bytes_sequence(16);

    std::string base64 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

    base64_to_bytes(base64, UNKNOWN_STRING_TASK_14);
    std::cout << "UNKNOWN_STRING_TASK_14 size(bytes): " << UNKNOWN_STRING_TASK_14.size() << "\n";

    RANDOM_PREFIX_TASK_14 = generate_random_bytes_sequence(generate_random_number(30));
    std::cout << "Random prefix size(bytes): " << RANDOM_PREFIX_TASK_14.size() << "\n";

    int block_size = discover_block_size(encryption_oracle_14);

    std::cout << "Size: " << block_size << "\n";

    //check ECB mode
    if (is_oracle_encrypt_ECB_mode(encryption_oracle_14)) {
        std::cout << "This ciphertext has been ecnrypted with ECB!\n\n";
    } else {
        std::cout << "This ciphertext has been ecnrypted with CBC!\n\n";
    }

    //Find start of attacker-bytes
    bytes temp_text {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    bytes temp_ciphertext = encryption_oracle_14(temp_text);

    bytes zeroes_block_enc = get_most_repeated_block(temp_ciphertext, block_size);

    temp_text = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    temp_ciphertext = encryption_oracle_14(temp_text);
    while (get_index_of_subvector(temp_ciphertext, zeroes_block_enc) == -1) {
        temp_text.push_back(0x00);
        temp_ciphertext = encryption_oracle_14(temp_text);
    }

    int bytes_to_cover_prefix = temp_text.size() - 16;
    std::cout << "Bytes to cover prefix: " << bytes_to_cover_prefix << "\n";

    int bytes_of_prefix = get_index_of_subvector(temp_ciphertext, zeroes_block_enc) - bytes_to_cover_prefix;
    std::cout << "Bytes of prefix: " << bytes_of_prefix << "\n";

    bytes zeroes_cover_prefix (bytes_to_cover_prefix, 0x00);
    temp_ciphertext = encryption_oracle_14(zeroes_cover_prefix);

    int bytes_of_x = temp_ciphertext.size() - bytes_of_prefix - bytes_to_cover_prefix;
    std::cout << "Bytes of x: " << bytes_of_x << "\n";

    int attack_block_start_ind = ((bytes_of_x / block_size) - 1) * 16 + bytes_of_prefix + bytes_to_cover_prefix;
    std::cout << "Start of attack block: " << attack_block_start_ind << "\n";

    bytes user_input (bytes_of_x + bytes_to_cover_prefix, 0x00);
    bytes encrypted;
    bytes enc_attack_block;

    byte finded_byte;

    bytes x_text (bytes_of_x, 0x00);
    bytes to_cover_prefix(bytes_to_cover_prefix, 0x00);

    bytes temp;


    for (int i = bytes_of_x; i > 0; --i) {
        user_input = slice(user_input, 1, user_input.size() - 1);

        x_text = slice(x_text, 1, x_text.size() - 1);

        temp = slice(x_text, attack_block_start_ind - (bytes_of_prefix + bytes_to_cover_prefix), block_size - 1);
        temp.insert(temp.begin(), to_cover_prefix.begin(), to_cover_prefix.end());

        std::map<bytes, byte> last_byte_dict = produce_last_byte_dict(temp, block_size, bytes_of_prefix + bytes_to_cover_prefix, encryption_oracle_14);
        
        encrypted = encryption_oracle_14(user_input);


        enc_attack_block = slice(encrypted, attack_block_start_ind, block_size);

        finded_byte = last_byte_dict[enc_attack_block];

        x_text.push_back(finded_byte);
    }

    std::string ascii = "";
    bytes_to_ASCII(x_text, ascii);
    std::cout << "Target string: \n" << ascii << "\n";
}


void solve_task_15()
{
    bytes correctly_padded {'I', 'C', 'E', ' ', 'I', 'C', 'E', ' ', 'B', 'A', 'B', 'Y', 0x04, 0x04, 0x04, 0x04};
    bytes incorrectly_padded {'I', 'C', 'E', ' ', 'I', 'C', 'E', ' ', 'B', 'A', 'B', 'Y', 0x05, 0x05, 0x05, 0x05};

    print_bytes(correctly_padded);
    if (is_PKCS7_padding_valide(correctly_padded)) {
        std::cout << "Correct padding!\n";
    } else {
        std::cout << "Incorrect padding!\n";
    }
    print_bytes(correctly_padded);

    print_bytes(incorrectly_padded);
    if (is_PKCS7_padding_valide(incorrectly_padded)) {
        std::cout << "Correct padding!\n";
    } else {
        std::cout << "Incorrect padding!\n";
    }
    print_bytes(incorrectly_padded);
}


bytes encryption_oracle_16(const std::string& user_data)
{
    std::vector<char> not_allowed_chars {';', '='};
    std::string quoted = quote_chars(user_data, not_allowed_chars);

    std::string prepend_str = "comment1=cooking%20MCs;userdata=";
    std::string append_str = ";comment2=%20like%20a%20pound%20of%20bacon";

    bytes prepend;
    ASCII_to_bytes(prepend_str, prepend);

    bytes text;
    ASCII_to_bytes(quoted, text);

    bytes append;
    ASCII_to_bytes(append_str, append);

    bytes plaintext;
    plaintext.insert(plaintext.end(), prepend.begin(), prepend.end());

    plaintext.insert(plaintext.end(), text.begin(), text.end());

    plaintext.insert(plaintext.end(), append.begin(), append.end());

    bytes ciphertext;
    params algorithm {4, 4, 10};
    encrypt_text_CBC(plaintext, ciphertext, KEY_TASK_16, IV_TASK_16, algorithm);

    return ciphertext;
}


void solve_task_16()
{
    KEY_TASK_16 = generate_random_bytes_sequence(16);
    IV_TASK_16 = generate_random_bytes_sequence(8);

    std::string data = "just = something";
    bytes res = encryption_oracle_16(data);

    print_bytes(res);
    
}