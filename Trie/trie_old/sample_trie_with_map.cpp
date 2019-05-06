#include <iostream>
#include<map>
#include<string>
using namespace std;

class _TrieNode {
public:
    std::string data;
    std::map<char, _TrieNode*> children;
    
    
};
int main() {
	_TrieNode *root = new _TrieNode();
	_TrieNode *node1 = new _TrieNode();
	node1->data = "Asad";
	if(root->children.find('X') == root->children.end()) {
	    std::cout<<"X is not present in the map"<<endl;
	    
	}
	root->children.insert(std::make_pair('A', node1) );
	;
	std:;cout<<"First element:"<<root->children.find('A')->second->data<<std::endl;
	return 0;
}

