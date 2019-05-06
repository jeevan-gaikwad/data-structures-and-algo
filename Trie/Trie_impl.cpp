#include"Trie.h"

Trie::Trie() {
	//Allocate memory for root node.
	root = std::make_shared<_TrieNode>();
}

Trie::Trie(std::string word):Trie()/*Invoke defalut constr first */{
	insert(word);
}

void Trie::insert_character(std::string::iterator currentCharItr,std::string::iterator endOfStringItr, std::shared_ptr<_TrieNode> existing_node) {
		char currentChar = *currentCharItr;
		std::shared_ptr<_TrieNode> child_node = existing_node->children[currentChar-'a'];
		if(currentCharItr != endOfStringItr) {
				if(child_node == nullptr) {//new_character is not present in the subtree. Insert it
					bool isLastCharOfWord = (currentCharItr + 1) == endOfStringItr ? true:false;
					child_node = std::make_shared<_TrieNode>(currentChar, isLastCharOfWord);
					existing_node->children[currentChar-'a'] = child_node;
					existing_node->degree += 1;
				}
				
				//continue
				insert_character(++currentCharItr,endOfStringItr,child_node);
		}
}

void Trie::insert(std::string word) {
	std::cout<<"Inserting:"<<word<<std::endl;	
	insert_character(word.begin(), word.end(), root);
}

//Internal recuresive function to traverse the tree
void Trie::display_rec(std::shared_ptr<_TrieNode> node ){
	std::shared_ptr<_TrieNode> *children = node->children;
	std::cout<<"Printing children of:'"<<node->data<<"':"<<std::endl;
	for(int i=0; i< MAX_CHARS; i++) {
		if(children[i] != nullptr) { //char is present
			std::cout<<"'"<<children[i]->data<<"'->";
			//print its children recursively
			display_rec(children[i]);
		}
	}
}

void Trie::display() {
	//root is a empty string.
	display_rec(root);

}
/*
void print(std::ostream text) {
	std::cout<<text<<std::endl;
}

*/

