#include"Trie.h"

int main(void) {

	Trie trie1("jeevan");
	trie1.insert("gaikwad");
	std::cout<<std::endl;
	trie1.insert("gaikpdsb");
	
	trie1.display();
	return 0;
}
