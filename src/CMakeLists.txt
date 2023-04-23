add_executable(chat chat.cpp header.h utils.h parse_json.h)
target_link_libraries(chat PRIVATE ggml)
