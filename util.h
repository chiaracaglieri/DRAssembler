#include <vector>
#include <unordered_map>

struct opcode{
    std::string literal;
    int code;
};

extern std::vector<struct opcode> optable;

extern std::unordered_map<std::string, int> symTable;

