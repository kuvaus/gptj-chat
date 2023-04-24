#pragma once

#ifndef UTILS_H
#define UTILS_H

#include "header.h" 


void set_console_color(ConsoleState &con_st, ConsoleColor color) {
    if (con_st.use_color && con_st.color != color) {
        //Windows handles colors differently.
        #ifdef _WIN32
          WORD windows_colors[] = {
            7, 14, 10, 15
        };          
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, windows_colors[color]);
        #else
        //ANSI colors, works for unix.
        const char* ansi_colors[] = {
            //DEFAULT, PROMPT, USER_INPUT,   BOLD
            //default, yellow, bright_green, bold
            "\x1b[0m", "\x1b[33m", "\x1b[1m\x1b[32m", "\x1b[1m"
        };
        printf("%s", ansi_colors[color]);
        #endif
        con_st.color = color;
    }
}

std::string random_prompt(int32_t seed) {
    const std::vector<std::string> prompts = {
        "So", "Once upon a time", "When", "The", "After", "If", "import", "He", "She", "They"
    };

    std::mt19937 rng(seed);
    return prompts[rng() % prompts.size()];
}


void print_usage(int argc, char** argv, const GPTJParams& params, std::string& prompt, int& memory) {
    // Print usage information
    fprintf(stderr, "usage: %s [options]\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "A simple chat program for GPT-J based models.\n");
    fprintf(stderr, "You can set specific initial prompt with the -p flag.\n");
    fprintf(stderr, "Runs default in interactive and continuous mode.\n");
    fprintf(stderr, "Type 'quit', 'exit' or, 'Ctrl+C' to quit.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "options:\n");
    fprintf(stderr, "  -h, --help            show this help message and exit\n");
    fprintf(stderr, "  --run-once            disable continuous mode\n");
    fprintf(stderr, "  --no-interactive      disable interactive mode altogether (uses given prompt only)\n");
    fprintf(stderr, "  -s SEED, --seed SEED  RNG seed (default: -1)\n");
    fprintf(stderr, "  -t N, --threads N     number of threads to use during computation (default: %d)\n", params.n_threads);
    fprintf(stderr, "  -p PROMPT, --prompt PROMPT\n");
    fprintf(stderr, "                        prompt to start generation with (default: empty)\n");
    fprintf(stderr, "  --random-prompt       start with a randomized prompt.\n");
    fprintf(stderr, "  -n N, --n_predict N   number of tokens to predict (default: %d)\n", params.n_predict);
    fprintf(stderr, "  --top_k N             top-k sampling (default: %d)\n", params.top_k);
    fprintf(stderr, "  --top_p N             top-p sampling (default: %.1f)\n", params.top_p);
    fprintf(stderr, "  --temp N              temperature (default: %.1f)\n", params.temp);
    fprintf(stderr, "  -b N, --batch_size N  batch size for prompt processing (default: %d)\n", params.n_batch);
    fprintf(stderr, "  -r N, --remember N    number of chars to remember from start of previous answer (default: %d)\n", memory);
    fprintf(stderr, "  -j,   --load_json FNAME\n");
    fprintf(stderr, "                        load options instead from json at FNAME (default: empty/no)\n");
    fprintf(stderr, "  -m FNAME, --model FNAME\n");
    fprintf(stderr, "                        model path (current: %s)\n", params.model.c_str());
    fprintf(stderr, "\n");
}

bool parse_params(int argc, char** argv, GPTJParams& params, std::string& prompt, bool& interactive, bool& continuous, int& memory) {
    std::string json_filename = "";

    // Parse parameters from a json file
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-j" || arg == "--json_load") {
            json_filename = argv[++i];
            if (!json_filename.empty()) {
                std::cout << "gptj_chat: parsing options from json: " << json_filename << std::endl;
                get_params_from_json(params, json_filename);
            } else {
                std::cout << "gptj_chat: trying to parse options from json but got empty filename." << std::endl;
            }
        } else {
            fprintf(stderr, "error: unknown argument: %s\n", arg.c_str());
            print_usage(argc, argv, params, prompt, memory);
            exit(0);
        }
    }

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--run-once") {
            continuous = false;
        } else if (arg == "--no-interactive") {
            interactive = false;
        } else if (arg == "-s" || arg == "--seed") {
            params.seed = static_cast<int32_t>(std::stoi(argv[++i]));
        } else if (arg == "-t" || arg == "--threads") {
            params.n_threads = static_cast<int32_t>(std::stoi(argv[++i]));
        } else if (arg == "-p" || arg == "--prompt") {
            prompt = argv[++i];
        } else if (arg == "--random-prompt") {
            prompt = random_prompt(params.seed);
        } else if (arg == "-n" || arg == "--n_predict") {
            params.n_predict = static_cast<int32_t>(std::stoi(argv[++i]));
        } else if (arg == "--top_k") {
            params.top_k = static_cast<int32_t>(std::stoi(argv[++i]));
        } else if (arg == "--top_p") {
            params.top_p = static_cast<float>(std::stof(argv[++i]));
        } else if (arg == "--temp") {
            params.temp = static_cast<float>(std::stof(argv[++i]));
        } else if (arg == "-b" || arg == "--batch_size") {
            params.n_batch = static_cast<int32_t>(std::stoi(argv[++i]));
        } else if (arg == "-r" || arg == "--remember") {
            memory = static_cast<int>(std::stoi(argv[++i]));
        } else if (arg == "-m" || arg == "--model") {
            params.model = argv[++i];
        } else if (arg == "-j" || arg == "--json_load") {
            json_filename = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            print_usage(argc, argv, params, prompt, memory);
            exit(0);
        } else {
            fprintf(stderr, "error: unknown argument: %s\n", arg.c_str());
            print_usage(argc, argv, params, prompt, memory);
            exit(0);
        }
    }

    return true;
}

#endif
