#include <iostream>

using namespace std;

template <class T>
bool isMoreDefault(const T& left, const T& right) {
	return left > right;
}

template <class T>
int supportElemPos(const T a, const T b, const T c, bool(*isMore)(const T&, const T&) = isMoreDefault) {
	if (isMore(a, b)) {
		if (isMore(c, a))
			return 1;
		return (isMore(b, c)) ? 2 : 3;
	}
	if (isMore(c, b))
		return 2;
	return (isMore(a, c)) ? 1 : 3;
}

template <class T>
int partition(T* arr, int l, int r, bool(*isMore)(const T&, const T&) = isMoreDefault) {
	int num = 1;
	int n = r - l + 1;
	if (n > 2)
		num = supportElemPos(arr[l], arr[l + n / 2 + 1], arr[r], isMore);

	int pos = 0;
	switch (num) {
	case 1:
		pos = 0;
		break;
	case 2:
		pos = (n > 1 ? n / 2 + 1 : 0);
		break;
	case 3:
		pos = n - 1;
	}

	swap(arr[l + pos], arr[r]);

	T pivot = arr[r];
	int i = 0, j = 0;
	while (j < r) {
		if (isMore(arr[j], pivot))
			j++;
		else {
			swap(arr[i], arr[j]);
			i++;
			j++;
		}
	}

	swap(arr[r], arr[i]);
	return i;
}

template <class T>
T KStat(T* arr, int l, int r, int k, bool(*isMore)(const T&, const T&) = isMoreDefault) {
	int pivotPos = partition(arr, l, r, isMore);
	while (pivotPos != k) {
		if (k < pivotPos)
			r = pivotPos - 1;
		else
			l = pivotPos + 1;
		pivotPos = partition(arr, l, r, isMore);
	}
	return arr[k];
}

int main() {
	int n = 0;
	cin >> n;
	int* arr = new int[n];
	for (int i = 0; i < n; i++)
		cin >> arr[i];
		//arr[i] = n - i;

	int perc10 = n / 10;
	int med = n / 2;
	int perc90 = n - (n + 9) / 10;

	int stat10 = KStat(arr, 0, n - 1, perc10);
	int statMed = KStat(arr, 0, n - 1, med);
	int stat90 = KStat(arr, 0, n - 1, perc90);

	//for (int i = 0; i < n; i++)
	//	cout << arr[i] << ' ';

	cout << stat10 << '\n' << statMed << '\n' << stat90;

	//cout << '\n' << "Stat10: " << stat10;
	//cout << '\n' << "Stat50: " << statMed;
	//cout << '\n' << "Stat90: " << stat90;
	
	delete[] arr;
	return 0;
}