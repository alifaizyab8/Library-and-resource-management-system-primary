#pragma once
#include <string>
#include <iostream>

class Category
{
private:
    int categoryId;
    std::string name;
    std::string description;

public:
    Category() : categoryId(0) {}

    Category(int id, const std::string &name, const std::string &desc)
        : categoryId(id), name(name), description(desc) {}

    // Getters
    int getCategoryId() const { return categoryId; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

    // Setters
    void setCategoryId(int id) { categoryId = id; }
    void setName(const std::string &n) { name = n; }
    void setDescription(const std::string &d) { description = d; }

    friend std::ostream &operator<<(std::ostream &os, const Category &c)
    {
        os << "ID: " << c.getCategoryId()
           << " | Name: " << c.getName()
           << " | Desc: " << c.getDescription();
        return os;
    }
};
