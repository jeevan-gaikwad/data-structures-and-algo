#include<iostream>
#include<vector>
#include<chrono>
#include<fstream>

void swap(std::vector<int>& arr, int firstIndex, int lastIndex) {
	int temp = arr[firstIndex];
	arr[firstIndex] = arr[lastIndex];
	arr[lastIndex] = temp;
}

int partition(std::vector<int>& arr, int firstIndex, int lastIndex) {

	int pivot = arr[lastIndex];
	//std::cout<<"Pivot element is:"<<pivot;
	int i=firstIndex;
	int j=lastIndex-1;
	while(i < j) {
		while(arr[i] <= pivot && i < lastIndex){
			i++;
		}

		while(arr[j] > pivot && j >0) {
			j--;
		}
		if(i < j) {
			swap(arr,i,j);
		}
	}
	swap(arr,i,lastIndex);
	return i;
}

void quickSort(std::vector<int>& arr, int firstIndex, int lastIndex) {
	if(firstIndex < lastIndex) {
		int pivotPlace = partition(arr, firstIndex, lastIndex);
		quickSort(arr, firstIndex, pivotPlace-1);
		quickSort(arr, pivotPlace + 1, lastIndex);
	}
}

void printArray(std::vector<int>& arr) {
	std::vector<int>::iterator itr = arr.begin();
	std::cout<<"Array is: [";
	for( ; itr != arr.end(); itr++) {
		std::cout<<*itr<<", ";
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

	std::string outputFileName("quick_sort_out.txt");
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
	quickSort(inputElements, 0, inputElements.size()-1);
	auto end =  std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout<<"No of microseconds taken to sort "<<noOfInputElements<<" are:"<<duration.count()<<std::endl;
	bool isSuccess = saveNumbersIntoFile(outputFileName, inputElements);
	if(isSuccess) {
		std::cout<<"Sorted elements saved to the file "<<outputFileName<<" successfully!"<<std::endl;
	}else {
		std::cerr<<"Failed to save sorted elements to the file "<<outputFileName<<std::endl;
	}

	return 0;
}
