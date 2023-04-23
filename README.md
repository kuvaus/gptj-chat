[![CMake](https://github.com/kuvaus/gptj-chat/actions/workflows/cmake.yml/badge.svg)](https://github.com/kuvaus/gptj-chat/actions/workflows/cmake.yml)
# GPTJ-Chat
Simple command line chat program for [GPT-J](https://en.wikipedia.org/wiki/GPT-J) models written in C++. Based on [ggml](https://github.com/ggerganov/ggml/) and [gptj.cpp](https://github.com/marella/gptj.cpp/).

# Table of contents
<!-- TOC -->
* [GPT-J model](#gpt-j-model)
* [Installation](#installation)
* [Usage](#usage)
* [Detailed command list](#detailed-command-list)
* [License](#license)
<!-- TOC -->

## GPT-J model
You need to download a GPT-J model first. Here are direct links to models:

>- The default version is **v1.0**: [ggml-gpt4all-j.bin](https://gpt4all.io/models/ggml-gpt4all-j.bin)
>- At the time of writing the newest is **1.3-groovy**: [ggml-gpt4all-j-v1.3-groovy.bin](https://gpt4all.io/models/ggml-gpt4all-j-v1.3-groovy.bin)

They're around 3.8 Gb each. The chat program stores the model in RAM on runtime so you need enough memory to run. You can get more details on GPT-J models from [gpt4all.io](https://gpt4all.io/) or [nomic-ai/gpt4all](https://github.com/nomic-ai/gpt4all) github.

## Installation
### Download

```sh
git clone --recurse-submodules https://github.com/kuvaus/gptj-chat
cd gptj-chat
```

### Build

```sh
mkdir build
cd build
cmake ..
cmake --build . --parallel
```

## Usage

After compiling, the binary is located at:

```sh
build/bin/chat
```
But you're free to move it anywhere. Simple command for 4 threads to get started:
```sh
./chat -m "/path/to/modelfile/ggml-gpt4all-j.bin" -t 4
```

Happy chatting!

## Detailed command list
You can view the help and full parameter list with:
`
./chat -h
`

```sh
usage: ./bin/chat [options]

A simple chat program for GPT-J based models.
You can set specific initial prompt with the -p flag.
Runs default in interactive and continuous mode.
Type 'quit', 'exit' or, 'Ctrl+C' to quit.

options:
  -h, --help            show this help message and exit
  --run-once            disable continuous mode
  --no-interactive      disable interactive mode altogether (uses given prompt only)
  -s SEED, --seed SEED  RNG seed (default: -1)
  -t N, --threads N     number of threads to use during computation (default: 4)
  -p PROMPT, --prompt PROMPT
                        prompt to start generation with (default: empty)
  --random-prompt       start with a randomized prompt.
  -n N, --n_predict N   number of tokens to predict (default: 200)
  --top_k N             top-k sampling (default: 40)
  --top_p N             top-p sampling (default: 0.9)
  --temp N              temperature (default: 0.9)
  -b N, --batch_size N  batch size for prompt processing (default: 8)
  -r N, --remember N    number of chars to remember from start of previous answer (default: 200)
  -l,   --load_json FNAME
                        load options instead from json at FNAME (default: empty/no)
  -m FNAME, --model FNAME
                        model path (current: models/ggml-gpt4all-j.bin)
```

You can also fetch parameters from a json file with `--load_json "/path/to/file.json"` flag.  The json file has to be in following format:

```javascript
{"top_p": 0.9, "top_k": 40, "temp": 0.9, "n_batch": 8}
```
This is useful when you want to store different temperature and sampling settings.

## License

This project is licensed under the MIT [License](https://github.com/kuvaus/gptj-chat/blob/main/LICENSE)
