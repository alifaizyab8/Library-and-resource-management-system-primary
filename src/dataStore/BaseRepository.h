#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <memory>
using namespace std;

template <typename T>
class BaseRepository
{
protected:
    vector<T> arr;
    string filename;

    void loadFromFile()
    {
        ifstream file(filename);
        if (!file.is_open())
            return; // If file did not open
        T item;
        while (file >> item) // Overloaded Operator used here
        {
            arr.push_back(item);
        }
        file.close();
    }

    void saveToFile()
    {
        ofstream file(filename, ios::trunc); 
        /* ios::trunc opens the file in write mode,
        all previous data deleted and current state is preserved */ 
        if (!file.is_open())
            return; // If file did not open
        for (const auto &item : arr)
        {
            file << item << "\n"; // Overloaded Operator used here
        }
        file.close();
    }

public:
    BaseRepository(const string &file) : filename(file)
    {
        loadFromFile();
    }
    virtual ~BaseRepository() {}
};