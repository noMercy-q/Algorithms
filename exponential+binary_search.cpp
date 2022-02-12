#include <iostream>
#include <math.h>

using namespace std;

int binSearch(int mas[], int value, int left, int right)
{
    while (left < right - 1) {
        int mid = (left + right) / 2;
        if (mas[mid] > value)
            right = mid;
        else {
            left = mid;
        }
    }
    return (((value - mas[left]) <= (mas[right] - value)) ? left : right);
}

int expSearch(int mas[], int size, int value)
{
    int i = 1;
    while (i <= size && value > mas[i])
        i *= 2;

    int left = i / 2;
    if (left >= size)
        left /= 2;
    int right;
    if (i >= size)
        right = size - 1;
    else
        right = i;
    
    return binSearch(mas, value, left, right);
}


int main()
{
    int n = 0, m = 0;

    cin >> n;
    int* mas1 = new int[n];
    for (int i = 0; i < n; i++)
        cin >> mas1[i];

    cin >> m;
    int* mas2 = new int[m];
    for (int i = 0; i < m; i++)
        cin >> mas2[i];

    for (int i = 0; i < m; i++)
        cout << expSearch(mas1, n, mas2[i]) << ' ';

    delete[] mas1;
    delete[] mas2;

    return 0;
}