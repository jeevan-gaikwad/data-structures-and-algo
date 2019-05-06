#include <iostream>
#include<map>
#include<string>
using namespace std;

class _TrieNode {
public:
    std::string data;
    std::map<std::string, _TrieNode*> children;
    
    
};
int main() {
	_TrieNode *root = new _TrieNode();
	_TrieNode *node1 = new _TrieNode();
	node1->data = "Asad";
	root->children.insert(std::pair<std::string, _TrieNode*>(node1->data, node1) );
	return 0;
}
