#include"Trie.h"
//#include<ostream>

Trie::Trie() {
	//Allocate memory for root node.
	root = std::make_shared<_TrieNode>();
}

Trie::Trie(std::string word):Trie()/*Invoke defalut constr first */{
	std::string::iterator itr = word.begin();
	
	for(;itr != word.end(); itr++){
		char character = *itr;
		int index = character - 'a';
		if(root->children[index] == nullptr ) {
			std::cout<<"Element "<<*itr<<" is not present in the Trie"<<std::endl;
			std::shared_ptr<_TrieNode> new_node = std::make_shared<_TrieNode>(character, (itr+1)==word.end()?true:false);
			root->children[index] = new_node;
			root->children[index]->data = character;
		} else
			std::cout<<"Location is NOT empty."<<character<< " is already presnet."<<std::endl;

	}

}

void Trie::insert_character(std::string::iterator currentCharItr,std::string::iterator endOfStringItr, std::shared_ptr<_TrieNode> existing_node) {
		char currentChar = *currentCharItr;
		std::shared_ptr<_TrieNode> child_node = existing_node->children[currentChar-'a'];
		if(currentCharItr != endOfStringItr) {
				if(child_node == nullptr) {//new_character is not present in the subtree. Insert it
					bool isLastCharOfWord = (currentCharItr + 1) == endOfStringItr ? true:false;
					child_node = std::make_shared<_TrieNode>(currentChar, isLastCharOfWord);
					existing_node->children[currentChar-'a'] = child_node;
				}
				
				//continue
				insert_character(++currentCharItr,endOfStringItr,child_node);
		}
}

void Trie::insert(std::string word) {
	std::cout<<"trying to insert:"<<word<<std::endl;	
	insert_character(word.begin(), word.end(), root);
}
/*
void print(std::ostream text) {
	std::cout<<text<<std::endl;
}

*/

