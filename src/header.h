#pragma once

#ifndef HEADER_H
#define HEADER_H

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include <cstdio>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <regex>
#include <cstring>

struct GPTJParams {
    int32_t seed = -1;
    int32_t n_threads = std::min(4, (int32_t)std::thread::hardware_concurrency());
    int32_t n_predict = 200;

    int32_t top_k = 40;
    float top_p = 0.9f;
    float temp = 0.9f;

    int32_t n_batch = 8;
    std::string model = "models/ggml-gpt4all-j.bin";
};

enum ConsoleColor {
    DEFAULT = 0,
    PROMPT,
    USER_INPUT,
    BOLD
};

struct ConsoleState {
    bool use_color = false;
    ConsoleColor color = DEFAULT;
};



//utils.h functions
void set_console_color(ConsoleState &con_st, ConsoleColor color);
std::string random_prompt(int32_t seed);
std::string readFile(const std::string& filename);
std::map<std::string, std::string> parse_json_string(const std::string& jsonString);
std::string removeQuotes(const std::string& input);

//parse_json.h functions
void get_params_from_json(GPTJParams & params, std::string& filename);
void print_usage(int argc, char** argv, const GPTJParams& params, std::string& prompt, int& memory);
bool parse_params(int argc, char** argv, GPTJParams& params, std::string& prompt, bool& interactive, bool& continuous, int& memory);

#endif