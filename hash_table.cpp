#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <utility>

class StringHasher {
public:
    int operator()(const std::string& str, int num = 7) const {
        int hash = 0;
        for (int i = 0; i < str.size(); i++)
            hash = (hash * num + str[i]);
        return hash + (1 - hash % 2);
    }
};


template <class T, class H>
class HashTable {
public:
    explicit HashTable(const H& Hasher) : hasher(Hasher), table(8), emptCount(8) {}
    HashTable(const HashTable&) = delete;
    HashTable& operator= (const HashTable&) = delete;

    bool Has(const T& data) const;
    bool Add(const T& data);
    bool Del(const T& data);

    ~HashTable() = default;

private:
    struct HashTableNode {
        T data;
        int hash = 0;
        int status = 0; // -1/0/1 = Deleted/Empty/Data

        HashTableNode() = default;
        HashTableNode(const T& Data, int Hash): data(Data), hash(Hash) {}
    };

    H hasher;
    std::vector<HashTableNode> table;

    int delCount = 0;
    int dataCount = 0;
    int emptCount;

    void reHashing(int grow);
}; 

template <class T, class H>
bool HashTable<T, H>::Has(const T& data) const {
    int h1 = hasher(data, 11);
    int h2 = hasher(data, 13);

    for (int i = 0; i < table.size(); i++) {
        int curHash = (h1 + i * h2) % table.size();
        switch (table[curHash].status) {

        case -1:
            break;

        case 0:
            return false;

        case 1: 
            if (data == table[curHash].data)
                return true;
            break;

        default: 
            assert(false);
        }
    }
}

template <class T, class H>
bool HashTable<T, H>::Add(const T& data) {

    if (dataCount + 1 > table.size() / 2)
        reHashing(2);

    int h1 = hasher(data, 11);
    int h2 = hasher(data, 13);

    for (int i = 0; i < table.size(); i++) {
        int firstDel = -1;
        int curHash = (h1 + i * h2) % table.size();
        switch (table[curHash].status) {

        case -1: 
            if (firstDel < 0)
                firstDel = curHash;
            break;

        case 0:
            if (firstDel < 0) {
                table[curHash].data = data;
                table[curHash].status = 1;
                emptCount--;
            }
            else {
                table[firstDel].data = data;
                table[firstDel].status = 1;
                delCount--;
            }
            dataCount++;
            return true;

        case 1:
            if (data == table[curHash].data)
                return false;
            break;
           
        default: 
            assert(false);
        }
    }
}

template <class T, class H>
bool HashTable<T, H>::Del(const T& data) {

    if (delCount + 1 > table.size() / 3)
        reHashing(1);

    int h1 = hasher(data, 11);
    int h2 = hasher(data, 13);

    for (int i = 0; i < table.size(); i++) {
        int curHash = (h1 + i * h2) % table.size();
        switch (table[curHash].status) {

        case -1:
            break;

        case 0:
            return false;

        case 1:
            if (data == table[curHash].data) {
                table[curHash].status = -1;
                delCount++;
                dataCount--;
                return true;
            }
            break;

        default:
            assert(false);
        }
    }
}

template <class T, class H>
void HashTable<T, H>::reHashing(int grow) {

    std::vector<HashTableNode> newTable(table.size() * grow);
    std::vector<HashTableNode> bufTable(table);
    table = std::move(newTable);

    for (int i = 0; i < bufTable.size(); i++) {
        if (bufTable[i].status == 1) {
            Add(bufTable[i].data);
        }
    }
}

int main()
{
    StringHasher sh;
    HashTable<std::string, StringHasher> hashT(sh);

    char cmd = 0;
    std::string value = "";
    bool res = 0;
    
    while (std::cin >> cmd >> value) {
        switch (cmd) {

        case '+':
            res = hashT.Add(value);
            break;

        case '-': 
            res = hashT.Del(value);
            break;

        case '?':
            res = hashT.Has(value);
            break;

        default:
            assert(false && "Incorrect input!");
        }
        
        std::cout << (res ? "OK" : "FAIL") << std::endl;                
    }

    return 0;
}
