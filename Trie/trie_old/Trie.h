#ifndef _TRIE_H
#define _TRIE_H
#include<iostream>
#include<map>

class _TrieNode
{

public:
    std::string data;
    std::map<char, _TrieNode*> *children;
	_TrieNode() {
		std::cout<<"Default constructor of _TrieNode called."<<std::endl;
		children = new std::map<char, _TrieNode*>();
	}
};

class Trie
{
    _TrieNode *root; //root will be a empty string

public:
    Trie() {
        root = new _TrieNode();
    }

	Trie(std::string data) {
	_TrieNode();
		//Create a Trie for the data
		std::string::iterator itr=data.begin();
		std::map<char, _TrieNode*> *children = root->children;
		if(children == NULL) {
			std::cout<<"No point in going ahead. Exit."<<std::endl;
			exit(0);
		}else {
			std::cout<<"Map size:"<<children->size()<<std::endl;
		}
		children->find('A');
		for(; itr!=data.end(); itr++) {
			std::map<char, _TrieNode*>::iterator it = children->find('A');
			if(it == children->end() ) { //element not present in the map
				_TrieNode *newNode = new _TrieNode();
				newNode->data[0] = *itr;
				children->insert(std::make_pair(*itr, newNode));
				std::cout<<*itr<<" inserted to the Trie."<<std::endl;
			}//otherwise skip the character

		}
	}

};
#endif

