#ifndef IDEPARTMENT_H
#define IDEPARTMENT_H

#include "IHospital.h"
#include <vector>
#include <string>

class IDepartment: public Department {
    
    std::string name;
    std::vector<std::string> doctors; //list of the doctors

public:

    IDepartment(const std::string& name, const std::vector<std::string>& doctors)
        : name(name), doctors(doctors) {}

    std::string getName() const override { return name; }
    std::vector<std::string> getDoctors() const override { return doctors; }//to get the list of the doctors

};

#endif //IDEPARTMENT_H