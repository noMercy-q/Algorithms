#include <iostream>
#include <string.h>
#include <assert.h>

int getDigit(unsigned long long val, int digit) {
	return ((val >> digit * 8) & 0b11111111);
}

void countingSort(unsigned long long* arr, int size, int digit) {
	assert(size > 0);
	const int k = 256;

	int* repeats = new int[k];
	//for (int i = 0; i < k; i++)
	//	repeats[i] = 0;
	memset(repeats, 0, k * sizeof(int));

	for (int i = 0; i < size; i++)
		++repeats[getDigit(arr[i], digit)];
	for (int i = 1; i < k; i++)
		repeats[i] += repeats[i - 1];

	unsigned long long* newArr = new unsigned long long[size];
	for (int i = size - 1; i >= 0; i--) {
		int idx = --(repeats[getDigit(arr[i], digit)]);
		newArr[idx] = arr[i];
	}
	
	delete[] repeats;
	memcpy(arr, newArr, size * sizeof(unsigned long long));
	delete[] newArr;
}

void LSDSort(unsigned long long* arr, int size) {
	for (int i = 0; i < sizeof(unsigned long long); i++) {
		countingSort(arr, size, i);
	}
}

int main() {
	int n = 0;
	std::cin >> n;
	unsigned long long* arr = new unsigned long long[n];
	//unsigned long long arr[] = { 1025, 1, 344, 4, 278, 3, 1 };
	for (int i = 0; i < n; i++)
		std::cin >> arr[i];

	LSDSort(arr, n);

	for (int i = 0; i < n; i++)
		std::cout << arr[i] << ' ';
	delete[] arr;
	return 0;
}
