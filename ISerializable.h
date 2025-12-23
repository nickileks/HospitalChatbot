//interface for saving and loading
#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <iostream>


class ISerializable {
public:

    virtual ~ISerializable() = default; //ensures proper cleanup
    virtual void Save(std::ostream&) const = 0; //to save objects to file, without const, I encountered errors
    virtual void Load(std::istream&) = 0; //to load already existing data from file
};


#endif //ISERIALIZABLE_H
