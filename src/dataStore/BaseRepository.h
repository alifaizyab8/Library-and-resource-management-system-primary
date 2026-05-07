#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

template <typename T>
class BaseRepository
{
protected:
    std::vector<T> arr;
    std::string filename;

     void loadFromFile()
    {
        std::ifstream persistentStorage(filename);
        if (!persistentStorage.is_open())
        {
            std::cerr << "Warning: Could not open " << filename << " for reading.\n";
            return;
        }
        T item;
        while (readFromFile(persistentStorage, item))
        {
            arr.push_back(item);
        }
        persistentStorage.close();
    }

    void saveToFile()
    {
        std::ofstream persistentStorage(filename, std::ios::trunc);
        if (!persistentStorage.is_open())
            return;
        for (const auto &item : arr)
        {
            writeToFile(persistentStorage, item) << '\n'; //
        }
        persistentStorage.close();
    }

public:
    BaseRepository(const std::string &persistentStorage) : filename(persistentStorage)
    {
        loadFromFile();
    }
    virtual ~BaseRepository() {}
};