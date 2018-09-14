#include <unordered_map>
#include <deque>
#include <string>
#include <fstream>

using namespace std;

extern string last_string;                      //Contains the last identifier found
extern string tmp;                              //Used to manipulate register identifiers
extern deque<int> v;                            //Contains the values for memlocs
extern int lc;                                  //Location Counter
extern ofstream outfile;                        //Contains the resulting binary code
extern ofstream reg_out;
extern ofstream mem_out;

void initRegister(int r, int v);
void initMemloc(int l, int v);
void initMemlocs(int l, deque<int> v);


/*  Symbol Table    */

extern unordered_map<string, int> symTable;
int find_symbol(string);
void insert_symbol(string,int);

/*  Syntax Tree     */

struct node{
    string type;        //Indicates type of node(ADD,MUL,LOAD,LABEL...)
    int value;          //For VALUE type nodes, it stores the associated numerical value
    int lc;             //Location counter
    node* param1;       //Pointer to first child node
    node* param2;       //Pointer to second child node
    node* param3;       //Pointer to third child node
};

typedef struct node* tree;
extern tree SynTree;

tree make_node(string type, int value, node* p1, node* p2, node* p3);
void append_tree(node* node);
void visit_tree(node*);

/*  OpCode Table    */

extern unordered_map<string,string> opTable;
void load_optable();