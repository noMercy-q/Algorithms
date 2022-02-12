#include <iostream>
#include <string.h>
#include <assert.h>

template <class T>
bool isLessDefault(const T& left, const T& right) {
	return left < right;
}

template <class T>
T* growBuffer(T* array, int& bufSize) {
	T* newArr = new T[bufSize * 2];
	memcpy(newArr, array, bufSize * sizeof(T));
	delete[] array;
	bufSize *= 2;
	return newArr;
}

template <class T>
void merge(T* leftArr, int leftSize, T* rightArr, int rightSize, T* destArr, bool (isLess)(const T&, const T&)) {
	int i = 0, j = 0;
	int sortSize = 0; // размер слитой части
	while (i < leftSize && j < rightSize) {
		if (isLess(leftArr[i], rightArr[j])) {
			destArr[sortSize++] = leftArr[i++];
		}
		else {
			destArr[sortSize++] = rightArr[j++];
		}
	}
	memcpy(destArr + sortSize, leftArr + i, (leftSize - i) * sizeof(T));
	memcpy(destArr + sortSize, rightArr + j, (rightSize - j) * sizeof(T));
}

template <class T>
void mergeSort(T* array, int arrSize, bool (isLess)(const T&, const T&) = isLessDefault) {
	if (arrSize == 1)
		return;
	int leftSize = arrSize / 2;
	int rightSize = arrSize - leftSize;
	mergeSort(array, leftSize, isLess);
	mergeSort(array + leftSize, rightSize, isLess);

	T* tmpArr = new T[arrSize];
	merge(array, leftSize, array + leftSize, rightSize, tmpArr, isLess);
	memcpy(array, tmpArr, arrSize * sizeof(T));
	delete[] tmpArr;
}

struct Consumer {
	int in = 0;
	int out = 0;

	void init(int inTime, int outTime) {
		in = inTime;
		out = outTime;
	}
};

bool isLessIn(const Consumer& left, const Consumer& right) {
	return left.in < right.in;
}

bool isLessOut(const Consumer& left, const Consumer& right) {
	return left.out < right.out;
}

Consumer* withoutRepeatOut(Consumer* srcArr, int srcSize, Consumer* dstArr, int& dstSize, int& dstBuf) {
	for (int i = 0; i < srcSize - 1; i++) {
		if (srcArr[i].out != srcArr[i + 1].out) {
			if (dstSize == dstBuf)
				dstArr = growBuffer(dstArr, dstBuf);
			dstArr[dstSize++] = srcArr[i];
		}
	}
	dstArr[dstSize++] = srcArr[srcSize-1];
	return dstArr;
}

int countOfAdds(Consumer* arr, int size) {
	int lastAdd = arr[0].out;
	int preLastAdd = arr[0].out - 1;
	int cnt = 2;

	for (int i = 1; i < size; i++)
		if (arr[i].in <= lastAdd) {
			if (arr[i].in > preLastAdd) {
				preLastAdd = lastAdd;
				lastAdd = arr[i].out;
				cnt++;
			}
		}
		else {
			preLastAdd = arr[i].out - 1;
			lastAdd = arr[i].out;
			cnt += 2;
		}
	return cnt;
}

int main() {
	int n = 0;
	int in = 0, out = 0;
	std::cin >> n;
	Consumer* arr = new Consumer[n];
	for (int i = 0; i < n; i++) {
		std::cin >> in >> out;
		arr[i].init(in, out);
	}

	mergeSort(arr, n, isLessIn);
	int newBuf = 4;
	Consumer* newArr = new Consumer[newBuf]; // исключаю повторения по времени выхода
	int newSize = 0;
	newArr = withoutRepeatOut(arr, n, newArr, newSize, newBuf);

	mergeSort(newArr, newSize, isLessOut);

	/*int addsBuf = 4;
	int* adds = new int[addsBuf];
	adds[0] = newArr[0].out - 1;
	adds[1] = newArr[0].out;
	int addsSize = 2;
	for (int i = 1; i < newSize; i++) {
		if (addsSize + 2 > addsBuf)
			adds = growBuffer(adds, addsBuf);
		if (newArr[i].in <= adds[addsSize-1]) {
			if (newArr[i].in > adds[addsSize - 2])
				adds[addsSize++] = newArr[i].out;
		}
		else {
			adds[addsSize++] = newArr[i].out - 1;
			adds[addsSize++] = newArr[i].out;
		}
	}*/
	//std::cout << addsSize;

	// ^ на всякий случай оставил вариант с запоминанием всех реклам, хотя здесь не требуется

	std::cout << countOfAdds(newArr, newSize);

	delete[] arr;
	delete[] newArr;
	return 0;
}