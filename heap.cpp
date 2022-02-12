#include <iostream>
#include <assert.h>
#include <string.h>

using namespace std;


struct Process {
	int T = 0;
	int t = 0;
	int P = 0;

	void init(int time, int priority) { // решил, что массив структур будет проще инициализировать
										// отдельным методом, поэтому выкинул всё из правила 3
		T = time;
		P = priority;
	}

	//Process(int time, int priority) : T(time), P(priority) {};
	//Process() = default;
	//Process(const Process&) = default;
	//~Process() = default;

	//Process& operator=(const Process&) = default;
};

template <class T>
bool isMoreDefault(const T& left, const T& right) {
	return left < right;
}

bool isMorePriority(const Process& left, const Process& right) {
	return (left.P * (left.t + 1) < right.P * (right.t + 1));
}

template <class T>
class Heap {
public:
	Heap() { isMore = isMoreDefault; };
	Heap(T* arr, int size, int memsize, bool (*isMore)(const T&, const T&) = isMoreDefault);
	Heap(const Heap&) = delete;
	~Heap() { delete[] buffer; };
	Heap& operator=(const Heap&) = delete;

	void Add(const T& el);
	T getMin();
	T popMin();
	int Size() { return size; };

private:
	T* buffer = nullptr;
	int bufSize = 0;
	int size = 0;

	void growBuffer();
	void siftDown(int i);
	void siftUp(int i);
	bool (*isMore)(const T&, const T&);
};

template <class T>
Heap<T>::Heap(T* arr, int arrSize, int memSize, bool (*IsMore)(const T&, const T&)) {
	isMore = IsMore;
	buffer = new T[memSize];
	memcpy(buffer, arr, arrSize * sizeof(T));
	bufSize = memSize;
	size = arrSize;
	for (int i = size / 2 - 1; i >= 0; i--)
		siftDown(i);
}

template <class T>
void Heap<T>::growBuffer() {
	T* newBuf = new T[bufSize * 2];
	memcpy(newBuf, buffer, bufSize * sizeof(T));
	T* tmp = buffer;
	buffer = newBuf;
	delete[] tmp;
	bufSize *= 2;
}

template <class T>
void Heap<T>::siftDown(int i) {
	int big = i;
	do {
		i = big;
		int left = 2 * i + 1;
		int right = left + 1;
		if (left < size && isMore(buffer[i], buffer[left]))
			big = left;
		if (right < size && isMore(buffer[big], buffer[right]))
			big = right;

		swap(buffer[i], buffer[big]);
	} while (big != i);
}

template <class T>
void Heap<T>::siftUp(int i) {
	bool stop = 0;
	while (!stop && i > 0) {
		int parent = (i - 1) / 2;
		if (isMore(buffer[parent], buffer[i])) {
			swap(buffer[i], buffer[parent]);
			i = parent;
		}
		else
			stop = 1;
	}
}

template <class T>
void Heap<T>::Add(const T& elem) {
	if (size == bufSize) {
		growBuffer();
	}
	buffer[size++] = elem;
	siftUp(size - 1);
}

template <class T>
T Heap<T>::popMin() {
	assert(size != 0);
	T res = buffer[0];
	buffer[0] = buffer[--size];

	siftDown(0);
	return res;
}

template <class T>
T Heap<T>::getMin() {
	assert(size != 0);
	return buffer[0];
}


int main()
{
	//cout << "Input size: ";
	int n = 0;
	cin >> n;
	Process* arr = new Process[n];

	int time = 0, pr = 0;
	for (int i = 0; i < n; i++) {
		cin >> pr >> time;
		arr[i].init(time, pr);
	}

	Heap<Process> heap(arr, n, n, isMorePriority);

	int counter = 0;
	while (heap.Size() > 0) {
		counter++;
		Process a = heap.popMin();
		a.t += a.P;
		if (a.t < a.T) {
			heap.Add(a);
		}
	}
	cout << counter;
	delete[] arr;
	return 0;
}
