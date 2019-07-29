#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<vector>

const int MIN = 0;
const int MAX = 30000;

void generateRandomNumber(std::vector<int>& randomNumbers, long noOfElements){

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for(long i = 0; i< noOfElements ; i++) {
		/* Generate random number within a range */
		static const double fraction = 1.0 / (RAND_MAX + 1.0);
		long randomNumber = MIN + static_cast<int> ((MAX - MIN +1) * (std::rand() * fraction));
		randomNumbers.push_back(randomNumber);
	}
}

bool saveRandomNumbersIntoFile(std::ofstream& file, std::vector<int>& randomNumbers) {
	if(file.fail())
		return false;
	/* FORMAT:
	line1: <number of elements>
	<actual elements seperated by \n>
	*/
	file<<randomNumbers.size()<<"\n";
	std::vector<int>::iterator itr = randomNumbers.begin();
	for( ;  itr != randomNumbers.end(); itr++) {
		file<<*itr<<"\n";
	}
	if(file.fail()) {
		return false;
	}else
		return true;
}

int main(int argc, char* argv[]) {

	if(argc < 2) {
		std::cerr<<"Usage: Please specify a file name to save the output.\n  "<<argv[0]<<" <file_name>"<<std::endl;
		return 1;
	}

	std::ofstream outputFile(argv[1]);
	if(outputFile.fail()) {
		std::cerr<<"Failed open file "<<argv[1]<<" for writting"<<std::endl;
	}else {
		std::cerr<<"File opened successfully for writting"<<std::endl;
	}

	long noOfElements;
	std::cout<<"Enter no of elements to generate:";
	std::cin>>noOfElements;
	
	std::vector<int> randomNumbers;
	generateRandomNumber(randomNumbers, noOfElements);
	bool isSuccess = saveRandomNumbersIntoFile(outputFile, randomNumbers);
	if(isSuccess) {
		std::cout<<"All numbers have been written to the file "<<argv[1]<<" successfully!"<<std::endl;
	}else
		std::cerr<<"Failed to random numbers to the file "<< argv[1]<<std::endl;
	outputFile.close();
	return 0;
}
