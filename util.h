#include <unordered_map>
//#include <variant>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

extern std::string last_string;
extern std::string tmp;
extern int last_value;
extern std::unordered_map<std::string, int> symTable;
extern std::unordered_map<std::string,std::string> opTable;
extern int lc;
bool find_symbol(std::string);
void insert_symbol(std::string,int);

/*  AST structures  */

struct node{
    std::string type;
    int value;
    std::vector<int> seq;
    int lc;
    node* param1;
    node* param2;
    node* param3;
};

typedef struct node* tree;
extern tree SynTree;

tree make_node(std::string type, int value, node* p1, node* p2, node* p3);
tree make_seq_node(std::vector<int> seq);

void appendTree(node* node);

/*  Optable utilities */
void loadOptable();