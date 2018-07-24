#include <vector>

struct opcode{
    std::string literal;
    int code;
};

extern std::vector<struct opcode> optable;