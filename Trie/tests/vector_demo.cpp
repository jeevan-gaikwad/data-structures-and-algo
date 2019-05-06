#include<iostream>
#include<vector>
#include<memory>

class test
{
	int num;
public:
	test() { }
};
int main(void) {

	std::vector<test*> tests(26);
	std::vector<test*>::iterator itr = tests.begin();
	int i=0;
	for(; itr != tests.end() ; itr++) {
		if(*itr == nullptr) {
			std::cout<<"Location "<<i<<" is empty"<<std::endl;
		}else {
			std::cout<<"Location "<<i<<" is NOT empty"<<std::endl;
		}
		i++;
	}

	return 0;
}
