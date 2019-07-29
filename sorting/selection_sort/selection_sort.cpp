#include<iostream>
#include<vector>


void swap(std::vector<int>& arr,int firstIndex, int secondIndex) {
	int temp = arr[firstIndex];
	arr[firstIndex] = arr[secondIndex];
	arr[firstIndex] = temp;
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

int main(void) {
	
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
	selectionSort(inputElements);
	printArray(inputElements);

	return 0;
}
