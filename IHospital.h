
#ifndef IHOSPITAL_H
#define IHOSPITAL_H

#include <string>
#include <vector>

//an interface for opening hours
class Hours {

public:

    virtual ~Hours() = default; //destructor - ensures cleanup for the derived class
    virtual std::string getOpeningHours() const = 0;
    virtual bool isOpen(const std::string& day, const std::string& time) const = 0; //checks if hospital is open at a given day and time
};

//interface for departments in the hospital
class Department {

public:

    virtual ~Department() = default; //virtual destructor
    virtual std::string getName() const = 0; //getter - retrieves the name department names
    virtual std::vector<std::string> getDoctors() const = 0; //getter- retrieves doctor's working in given department
};


//interface which provides information for all departments
class DepartmentsInfo {
public:

    virtual ~DepartmentsInfo() = default;
    virtual std::vector<Department*> getDepartments() const = 0; //gets the pointers to all departments
    virtual std::string listDepartments() const = 0; //returns the sstring which lists the departments
};


//interface which manages appointments
class AppointmentsManager {
public:

    virtual ~AppointmentsManager() = default;
    virtual std::string booking() = 0;  //method which books new appointments
    virtual bool Availability(const std::string& department, const std::string& doctor,
                                   const std::string& day, const std::string& time) const = 0;  //checks for the availability of doctor at given time
    virtual std::string listAppointments() const = 0;
};


//interface containing hours and department
class HospitalInfo : public Hours, public DepartmentsInfo {
public:

    virtual ~HospitalInfo() = default;
    virtual std::string getFAQ() const = 0; //gives faq's
    virtual std::string processQuestion(const std::string& query) = 0; //checks questions an dreturns an answer, prexisting answersh
};


#endif //IHOSPITAL_H
