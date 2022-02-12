#include <iostream>
#include <assert.h>
#include <string.h>
#include <math.h>

using namespace std;
const size_t startSize = 2;

class Queue {

private:
    int* buf;
    int head = -1;
    int tail = -1;

    int size = 0;
    size_t bufSize = startSize;

    bool needExtend();
    void extend();
    
public:
    Queue();
    Queue(const Queue&) = delete;
    ~Queue();
    Queue& operator = (const Queue&) = delete;

    void PushBack(const int& val);
    int PopFront();
    int Size() const { return size; }
    bool IsEmpty() const { return !size; }
};

Queue::Queue() 
{
    buf = new int[startSize];
}

bool Queue::needExtend()
{
    return (tail - head == bufSize - 1 || head - 1 == tail);
}

void Queue::extend()
{
    int* newBuf = new int[bufSize * 2];
    memcpy(newBuf, buf + head, ((int)bufSize - head) * sizeof(int));
    memcpy(newBuf + ((int)bufSize - head), buf, head * sizeof(int));

    int* tmpPtr = buf;
    buf = newBuf;
    delete[] tmpPtr;
    bufSize *= 2;
    head = 0;
    tail = size - 1;
}

void Queue::PushBack(const int& val) 
{
    if (needExtend())
        extend();

    if (IsEmpty()) {
        head = 0;
        tail = 0;
        buf[head] = val;
    }
    else {
        if (tail < bufSize - 1)
            buf[++tail] = val;
        else {
            buf[0] = val;
            tail = 0;
        }
    }
    size++;
}

int Queue::PopFront()
{
    assert(!IsEmpty());

    int result = buf[head];

    size--;
    if (IsEmpty()) {
        head = -1;
        tail = -1;
    }
    else {
        if (head < bufSize - 1)
            head++;
        else {
            head = 0;
        }
    }
    return result;
}

Queue::~Queue()
{
    delete[] buf;
}

int main()
{
    Queue queue;
    int n; 
    cin >> n;
    bool res = true;
    for (int i = 0; i < n; i++) {
        int cmd, val;
        cin >> cmd >> val;
        switch (cmd) {
        case 2:
            res = res && (queue.IsEmpty() ? val == -1 : val == queue.PopFront());
            break;

        case 3:
            queue.PushBack(val);
            break;
        default:
            assert(false);
        }
    }
    cout << (res ? "YES" : "NO");

    return 0;
}