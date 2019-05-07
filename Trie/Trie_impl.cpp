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
	//std::cout<<"Printing children of:'"<<node->data<<"':";//<<std::endl;
	if(node->isCompleteWord) {
		std::cout<<"EOW"<<std::endl;
	}
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

bool Trie::delete_character(std::string::iterator currentCharItr,std::string::iterator endOfStringItr, std::shared_ptr<_TrieNode> existing_node) {
	char currentChar = *currentCharItr;

	if(currentCharItr != endOfStringItr) {
		char currentChar = *currentCharItr;
		std::shared_ptr<_TrieNode> child_node = existing_node->children[currentChar-'a'];
		if(child_node != nullptr) {
			if((currentCharItr + 1) == endOfStringItr) {
				std::shared_ptr<_TrieNode> next_node = child_node->children[*(currentCharItr+1)-'a'];
				if(next_node->degree == 0) {
					//free next node slot
					std::cout<<"Deleted:"<<next_node->data<<std::endl;
					//next_node->reset();
					child_node->children[*(currentCharItr+1)-'a'] = nullptr;
					child_node->degree -= 1; //Decrement degree by 1 
					return true;
				} else {
					//Skip the deletion
					std::cout<<"Skipped deletion of:"<<next_node->data<<std::endl;
				}
			} else {
				delete_character(++currentCharItr,endOfStringItr,child_node);
			}
		} else {
			return false;
		}
	}

}
bool Trie::delete_word(std::string word) {
	std::cout<<"Deleting word:"<<word<<std::endl;
	return delete_character(word.begin(),word.end(), root);
}
/*
void print(std::ostream text) {
	std::cout<<text<<std::endl;
}

*/

