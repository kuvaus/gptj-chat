#include "header.h"
#include "utils.h"
#include "parse_json.h"
#include "gptj.cpp"


std::string get_input(ConsoleState& con_st, auto context, std::string& input) {
    set_console_color(con_st, USER_INPUT);

    std::cout << "\n> ";
    std::getline(std::cin, input);
    set_console_color(con_st, DEFAULT);

    if (input == "exit" || input == "quit") {
        free_model(context);
        exit(0);
    }

    return input;
}

int main(int argc, char* argv[]) {
    ConsoleState con_st;
    con_st.use_color = true;
    set_console_color(con_st, DEFAULT);

    bool interactive = true;
    bool continuous = true;

    std::string response;
    response.reserve(10000);
    int memory = 200;
    GPTJParams params;
    std::string prompt = "";
    std::string input = "";
    std::string answer = "";
   
    parse_params(argc, argv, params, prompt, interactive, continuous, memory);

    std::cout << "gptj-chat" << std::endl;
    auto context = load_model(params.model.c_str());
    set_console_color(con_st, PROMPT);
    std::cout << " " << prompt.c_str() << std::endl;
    set_console_color(con_st, DEFAULT);

    std::string default_prefix = "The prompt below is a question to answer, a task to complete, or a conversation to respond to; decide which and write an appropriate response.";
    std::string default_header = "\n### Prompt: ";
    std::string default_footer = "\n### Response: ";

    if (interactive) {
        input = get_input(con_st, context, input);
        if (prompt != "") {
            auto a = generate(context, (default_prefix + default_header + prompt + " " + input + default_footer).c_str(), params, &response[0]);
        } else {
            auto a = generate(context, (default_prefix + default_header + input + default_footer).c_str(), params, &response[0]);
        }
        answer = response.c_str();

        while (continuous) {
            std::string memory_string = default_prefix;
            if (memory > 1) {
                memory_string = default_prefix + default_header + input.substr(0, memory) + default_footer + answer.substr(0, memory);
            }
            input = get_input(con_st, context, input);
            auto a = generate(context, (memory_string + default_header + input + default_footer).c_str(), params, &response[0]);
            answer = response.c_str();
        }
    } else {
        auto a = generate(context, (default_prefix + default_header + prompt + default_footer).c_str(), params, &response[0]);
    }

    set_console_color(con_st, DEFAULT);
    free_model(context);
    return 0;
}
