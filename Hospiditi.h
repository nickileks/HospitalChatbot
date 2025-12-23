#ifndef HOSPIDITI_H
#define HOSPIDITI_H

#include "ISerializable.h"
#include "IHospital.h"
#include "IDepartment.h"
#include <string>
#include <vector>
#include <iostream>

//storing all the info for booking
struct Appointment {

    std::string name;
    std::string department;
    std::string doctor;
    std::string day;
    std::string time;

    bool operator==(const Appointment& other) const { //avoids clashing appointments

        return department == other.department &&
               doctor == other.doctor &&
               day == other.day &&
               time == other.time;
    }
};

//printing appointments
inline std::ostream& operator<<(std::ostream& os, const Appointment& a) {  //operator overloading //inline- without it i encountered linker error(linker command failed)
    os << a.name << " with Dr. " << a.doctor << " in " << a.department
       << " on " << a.day << " at " << a.time;

    return os;
}

//class template, no need for repeated code with this
template<typename T> //T-for any type of data

bool contains(const std::vector<T>& vec, const T& item) {
    return std::find(vec.begin(), vec.end(), item) != vec.end();
}

class Hospiditi : public HospitalInfo, public AppointmentsManager, public ISerializable {

    std::vector<IDepartment> departments;
    Appointment* appointments;//dynamic array of appointments

    int appointmentCount; //current number of appointments
    int maxAppointments;


    void expandAppointments();//this doubles the array size if it runs out of space


public:
    Hospiditi();
    ~Hospiditi() override;//we have virtual destructor base so we need override

    //file saving and loading
    void Save(std::ostream&) const override;
    void Load(std::istream&) override;

    std::string getOpeningHours() const override;
    bool isOpen(const std::string& day, const std::string& time) const override;

    std::vector<Department*> getDepartments() const override;
    std::string listDepartments() const override;

    std::string booking() override;

    bool Availability(const std::string& department, const std::string& doctor,
                           const std::string& day, const std::string& time) const override;

    std::string listAppointments() const override;

    std::string getFAQ() const override;
    std::string processQuestion(const std::string& query) override;

    static std::string lowercase(std::string s);//converting to lower case
};

#endif //HOSPIDITI_H