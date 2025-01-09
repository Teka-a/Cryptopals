#include "AES.h"

void padding_PKCS7(bytes& vec, int block_size)
{
    int req_size = vec.size();
    while (req_size % block_size != 0) {
        req_size += 1;
    }

    int amount = req_size - vec.size();
    byte value = amount ^ 0x00;
    for (int i = 0; i < amount; ++i) {
        vec.push_back(value);
    }
}


void unpadding_PKCS7(bytes& vec)
{
    byte item = vec[vec.size() - 1];
    int amount = item;
    bool flag = true;
    for (int i = 0; i < amount; ++i) {
        if (vec[vec.size() - i - 1] != item) {
            flag = false;
        }
    }

    if (flag) {
        while (amount > 0) {
            vec.pop_back();
            amount -= 1;
        }
    }
}


void sub_bytes(matrix& state)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = S_BOX[(int) state[i][j]];
        }
    }
}


void inv_sub_bytes(matrix& state)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = (unsigned char) (find(S_BOX.begin(), S_BOX.end(), state[i][j]) - S_BOX.begin());
        }
    }
}


void shift_row_by_one_byte_forward(unsigned char (&row)[4])
{
    for (int i = 0; i < 3; ++i) {
        unsigned char temp = row[i + 1];
        row[i + 1] = row[i];
        row[i] = temp;
    }

}


void shift_row_by_one_byte_backward(unsigned char (&row)[4])
{
    for (int i = 3; i > 0; --i) {
        unsigned char temp = row[i - 1];
        row[i - 1] = row[i];
        row[i] = temp;
    }

}


void shift_rows(matrix& state)
{
    for (int i = 1; i < 4; ++i) {
        for (int count = i; count > 0; --count) {
            shift_row_by_one_byte_forward(state[i]);
        }
    }
}


void inv_shift_rows(matrix& state)
{
    for (int i = 1; i < 4; ++i) {
        for (int count = i; count > 0; --count) {
            shift_row_by_one_byte_backward(state[i]);
        }
    }
}


unsigned char multiplicate_in_galua_field(unsigned char a, unsigned char b)
{
    unsigned char c = 0x00;
    while (b > 0) {
        if (b & 1) {
            c ^= a;
        }
        a = (a << 1) ^ (a & 0x80 ? 0x1b : 0x00);
        b >>= 1;
    }
    return c;
}


void mix_columns(matrix& state)
{
    matrix temp_state;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp_state[i][j] = state[i][j];
        }
    }
    for (int i = 0; i < 4; ++i) { 
        for (int j = 0; j < 4; ++j) {
            unsigned char temp = 0x00;
            //std::cout << i << ", " << j << ": \n";
            for (int row = 0; row < 4; ++row) {
                //std::cout << formatHex(MIX[i][row]) << " * " << formatHex(tempState[row][j]);
                temp ^= multiplicate_in_galua_field(MIX[i][row], temp_state[row][j]);        
                //std::cout << " = " << formatHex(temp) << "\n";
            }
            state[i][j] = temp;
        }
    }
}


void inv_mix_columns(matrix& state)
{
    matrix temp_state;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp_state[i][j] = state[i][j];
        }
    }
    for (int i = 0; i < 4; ++i) { 
        for (int j = 0; j < 4; ++j) {
            unsigned char temp = 0x00;
            //std::cout << i << ", " << j << ": \n";
            for (int row = 0; row < 4; ++row) {
                //std::cout << formatHex(MIX[i][row]) << " * " << formatHex(tempState[row][j]);
                temp ^= multiplicate_in_galua_field(INV_MIX[i][row], temp_state[row][j]);        
                //std::cout << " = " << formatHex(temp) << "\n";
            }
            state[i][j] = temp;
        }
    }
}


void sub_word(word& w)
{
    for (int i = 0; i < 4; ++i) {
        w[i] = S_BOX[w[i]];
    }
}


void rot_word(word& w)
{
    shift_row_by_one_byte_forward(w);
}


void r_con(word& w, int ind)
{
    unsigned char first_byte = 0x01;
    for (int i = 1; i < ind; ++i) {
        first_byte = multiplicate_in_galua_field(first_byte, 0x02);
    }
    w[0] = first_byte;
    for (int i = 1; i < 4; ++i) {
        w[i] = 0x00;
    }
}


void XOR_words(word& a, word& b)
{
    for (int i = 0; i < 4; ++i) {
        a[i] ^= b[i];
    }
}


void XOR_blocks(block& a, block& b)
{
    for (int i = 0; i < 16; ++i) {
        a[i] ^= b[i];
    }
}


void copy_word(word& from, word& to)
{
    for (int i = 0; i < 4; ++i) {
        to[i] = from[i];
    }
}


void copy_block(block& from, block& to)
{
    for (int i = 0; i < 16; ++i) {
        to[i] = from[i];
    }
}


void expand_key(bytes& key, std::vector<word>& key_schedule, int Nk)
{
    for (int i = 0; i < Nk; ++i) {
        for (int j = 0; j < 4; ++j) {
            key_schedule[i][j] = key[4 * i + j];
        }
    }
    
    word r {0x00, 0x00, 0x00, 0x00};
    for (int i = Nk; i < key_schedule.size(); ++i) {
        //std::cout << "Iter: " << i << "\n";
        word temp {0x00, 0x00, 0x00, 0x00};
        copy_word(key_schedule[i - 1], temp);
        //printWord(temp);
        if (i % Nk == 0) {
            rot_word(temp);
            //printWord(temp);
            sub_word(temp);
            //printWord(temp);
            r_con(r, i / Nk);
            //printWord(r);
            XOR_words(temp, r);
            //printWord(temp);
        } else if (Nk > 6 && i % Nk == 4) {
            sub_word(temp);
        }
        XOR_words(temp, key_schedule[i - Nk]);
        //printWord(temp);
        copy_word(temp, key_schedule[i]);

    }
}


void add_round_key(matrix& state, std::vector<word>& keys, int round) 
{
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            state[row][col] ^= keys[round * 4 + col][row];
        }
    }
}



void encrypt_block(block& plaintext, block& ciphertext, std::vector<word>& keys, int rounds)
{
    matrix state;
    block_to_matrix(plaintext, state);
    add_round_key(state, keys, 0);

    for (int round = 1; round < rounds; ++round) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, keys, round);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, keys, rounds);

    matrix_to_block(state, ciphertext);

}


void decrypt_block(block& ciphertext, block& plaintext, std::vector<word>& keys, int rounds)
{
    matrix state;
    block_to_matrix(ciphertext, state);
    add_round_key(state, keys, rounds);

    for (int round = 9; round > 0; --round) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, keys, round);
        inv_mix_columns(state);
    }

    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, keys, 0);

    matrix_to_block(state, plaintext);

}


void encrypt_text_ECB(bytes& plaintext, bytes& ciphertext, bytes& key, params& algorithm)
{
    std::vector<word> key_schedule (algorithm.Nb * (algorithm.Nr + 1));
    expand_key(key, key_schedule, algorithm.Nk); 
    ciphertext.clear();
    padding_PKCS7(plaintext, 16);
    for (int i = 0; i < plaintext.size(); i += 16) {
        bytes temp_bytes;
        bytes block_in = slice(plaintext, i, 16);
        bytes block_out (block_in.size());
        block aes_block_in;
        block aes_block_out;
        bytes_to_block(block_in, aes_block_in);
        bytes_to_block(block_out, aes_block_out);
        encrypt_block(aes_block_in, aes_block_out, key_schedule, algorithm.Nr);
        
        block_to_bytes(aes_block_out, temp_bytes);
        ciphertext.insert(ciphertext.end(), temp_bytes.begin(), temp_bytes.end());
    }    
}


void decrypt_text_ECB(bytes& ciphertext, bytes& plaintext, bytes& key, params& algorithm)
{
    std::vector<word> key_schedule (algorithm.Nb * (algorithm.Nr + 1));
    expand_key(key, key_schedule, algorithm.Nk); 
    plaintext.clear();
    std::cout << "Here\n";
    for (int i = 0; i < ciphertext.size(); i += 16) {
        bytes temp_bytes;
        
        bytes block_in = slice(ciphertext, i, 16);
        bytes block_out (block_in.size());
        block aes_block_in;
        block aes_block_out;
        bytes_to_block(block_in, aes_block_in);
        bytes_to_block(block_out, aes_block_out);

        decrypt_block(aes_block_in, aes_block_out, key_schedule, algorithm.Nr);

        
        block_to_bytes(aes_block_out, temp_bytes);
        plaintext.insert(plaintext.end(), temp_bytes.begin(), temp_bytes.end());
    }  
    unpadding_PKCS7(plaintext);
}


void encrypt_text_CBC(bytes& plaintext, bytes& ciphertext, bytes& key, bytes& initVector, params& algorithm)
{
    std::vector<word> key_schedule (algorithm.Nb * (algorithm.Nr + 1));
    expand_key(key, key_schedule, algorithm.Nk); 
    ciphertext.clear();
    block IV;
    bytes_to_block(initVector, IV);
    padding_PKCS7(plaintext, 16);
    for (int i = 0; i < plaintext.size(); i += 16) {
        bytes temp_bytes;
        bytes block_in = slice(plaintext, i, 16);
        bytes block_out (block_in.size());
        block aes_block_in;
        block aes_block_out;
        bytes_to_block(block_in, aes_block_in);
        bytes_to_block(block_out, aes_block_out);
        XOR_blocks(aes_block_in, IV);
        encrypt_block(aes_block_in, aes_block_out, key_schedule, algorithm.Nr);
        block_to_bytes(aes_block_out, temp_bytes);
        ciphertext.insert(ciphertext.end(), temp_bytes.begin(), temp_bytes.end());
        copy_block(aes_block_out, IV);
    }    
}


void decrypt_text_CBC(bytes& ciphertext, bytes& plaintext, bytes& key, bytes&initVector, params& algorithm)
{
    std::vector<word> key_schedule (algorithm.Nb * (algorithm.Nr + 1));
    expand_key(key, key_schedule, algorithm.Nk); 
    plaintext.clear();
    block IV;
    bytes_to_block(initVector, IV);
    for (int i = 0; i < ciphertext.size(); i += 16) {
        bytes temp_bytes;
        bytes block_in = slice(ciphertext, i, 16);
        bytes block_out (block_in.size());
        block aes_block_in;
        block aes_block_out;
        bytes_to_block(block_in, aes_block_in);
        bytes_to_block(block_out, aes_block_out);
        decrypt_block(aes_block_in, aes_block_out, key_schedule, algorithm.Nr);
        XOR_blocks(aes_block_out, IV);
        copy_block(aes_block_in, IV);
        block_to_bytes(aes_block_out, temp_bytes);
        plaintext.insert(plaintext.end(), temp_bytes.begin(), temp_bytes.end());
    }  
    unpadding_PKCS7(plaintext);
}