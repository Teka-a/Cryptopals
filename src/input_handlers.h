#pragma once
#ifndef HANDLERS_H
#define HANDLERS_H

#include <iostream>


void read_from_user(std::string, std::string&);
std::string get_input_file_name();
std::string get_output_file_name();
void read_from_file(std::string&, std::string);
void write_to_file(std::string&, std::string);

#endif // HANDLERS_H