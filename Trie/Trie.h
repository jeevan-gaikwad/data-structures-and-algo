#ifndef _TRIE_H
#define _TRIE_H
#include<iostream>
#include<string>
#include<cstdlib>
#include<vector>
#include<memory>

const short int MAX_CHARS=26;

class _TrieNode
{

public:
	char data;
	bool isCompleteWord;
	unsigned short int degree;
	std::shared_ptr<_TrieNode> children[MAX_CHARS]={nullptr};
	
	_TrieNode() { }
	_TrieNode(char character, bool isCompleteWord) {
		this->data = character;
		this->isCompleteWord = isCompleteWord;
	}
};

class Trie
{
private:
	int noOfWords;
	std::shared_ptr<_TrieNode> root;

	void insert_character(std::string::iterator currentCharItr,std::string::iterator endOfStringItr, std::shared_ptr<_TrieNode> existing_node);
	//Internal recuresive function to traverse the tree
	void display_rec(std::shared_ptr<_TrieNode> node);

	int delete_character(std::string::iterator currentCharItr,std::string::iterator endOfStringItr, std::shared_ptr<_TrieNode> existing_node);
	int delete_character_v2(std::string::iterator currentCharItr,std::string::iterator endOfStringItr, std::shared_ptr<_TrieNode> existing_node);

public:
	Trie();
	Trie(std::string word);
	void insert(std::string word);
	void display();
	bool delete_word(std::string word);
};
#endif
