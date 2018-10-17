/**
 * @file parse_util.h
 * @author Chiara Caglieri
 * @brief Header containing
 *        utility functions for the analyzer
 */
#include <unordered_map>
#include <deque>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

extern string last_string;                      /**<Last identifier found */
extern string tmp;
extern deque<int> v;                            /**<Values for memlocs */
extern int lc;                                 /**<Program counter */
extern ofstream outfile;
extern ofstream reg_out;
extern ofstream mem_out;
extern vector<string> words;                    /**<32-bit binary instructions */


void initRegister(int r, int v);
void initMemloc(int l, int v);
void initMemlocs(int l, deque<int> v);


extern unordered_map<string, int> symTable;
int find_symbol(string);
void insert_symbol(string,int);


struct node{
    string type;         /**<Type of node */
    int value;           /**<Numerical value for VALUE-type nodes */
    int lc;              /**<Program Counter */
    node* param1;        /**<First child node */
    node* param2;        /**<Second child node */
    node* param3;        /**<Third child node */
};

typedef struct node* tree;
extern tree SynTree;

tree make_node(string type, int value, node* p1, node* p2, node* p3);
void append_tree(node* node);
void visit_tree(node*);


extern unordered_map<string,string> opTable;
void substitute(string f, string p1, string p2);
void load_optable();

