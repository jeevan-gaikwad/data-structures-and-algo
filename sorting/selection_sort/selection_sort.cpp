#include<iostream>
#include<vector>
#include<chrono>
#include<fstream>

void swap(std::vector<int>& arr,int firstIndex, int secondIndex) {
	int temp = arr[firstIndex];
	arr[firstIndex] = arr[secondIndex];
	arr[secondIndex] = temp;
}

int findMinimumIndex(std::vector<int>& arr, int startIndex) {

	int minValue = arr[startIndex];
	long minIndex = startIndex;
	
	for(long i =  minIndex +1; i < arr.size(); i++) {
		if(arr[i] < minValue) {
			minValue = arr[i];
			minIndex = i;
		}
	}

	return minIndex;
}

void selectionSort(std::vector<int>& inputElements) {
	
	for(long i=0; i< inputElements.size(); i++) {
		long minIndex = findMinimumIndex(inputElements,i);
		swap(inputElements,i,minIndex);
	}

}

void printArray(std::vector<int>& inputElements) {
	std::vector<int>::iterator itr = inputElements.begin();

	std::cout<<"[";
	for( ; itr != inputElements.end(); itr++) {
		std::cout<<*itr<<",";	
	}

	std::cout<<"]"<<std::endl;
}

bool saveNumbersIntoFile(std::string fileName, std::vector<int>& elements) {
	std::ofstream file(fileName);
	if(file.fail())
		return false;
	/* FORMAT:
	line1: <number of elements>
	<actual elements seperated by \n>
	*/
	file<<elements.size()<<"\n";
	std::vector<int>::iterator itr = elements.begin();
	for( ;  itr != elements.end(); itr++) {
		file<<*itr<<"\n";
	}
	if(file.fail()) {
		file.close();
		return false;
	}else {
		file.close();
		return true;
	}
}

int main(void) {
	std::string outputFileName("selection_sort_out.txt");
	std::vector<int> inputElements;
	long noOfInputElements;
	std::cin>>noOfInputElements;
	//inputElements.resize(noOfInputElements);

	for(long i=0;i<noOfInputElements; i++) {
		int element;
		std::cin>>element;
		inputElements.push_back(element);
	}
	//inputElements.shrink_to_fit();
	auto start =  std::chrono::high_resolution_clock::now();
	selectionSort(inputElements);
	auto end =  std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout<<"No of microseconds taken to sort "<<noOfInputElements<<" are:"<<duration.count()<<std::endl;
	bool isSuccess = saveNumbersIntoFile(outputFileName, inputElements);
	if(isSuccess) {
		std::cout<<"Sorted elements saved to the file "<<outputFileName<<" successfully!"<<std::endl;
	}else {
		std::cerr<<"Failed to save sorted elements to the file "<<outputFileName<<std::endl;
	}
	//printArray(inputElements);

	return 0;
}
