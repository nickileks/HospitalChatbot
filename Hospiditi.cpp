
#include "Hospiditi.h"
#include <sstream>
#include <algorithm>
#include <ctime>


Hospiditi::Hospiditi() : appointmentCount(0), maxAppointments(10) {
    try {

        appointments = new Appointment[maxAppointments]; //allocates memory for 10 appointments

    } catch (const std::bad_alloc& e) { //handles crash when memory allocation fails

        std::cerr << "Memory allocation failed during Hospiditi initialization: " << e.what() << std::endl;

        throw; //stops program
    }

    //loads all departments with their respective doctors
    departments.emplace_back("Pediatrics", std::vector<std::string>{"Ariana", "Grace", "Kovács"});
    departments.emplace_back("GP", std::vector<std::string>{"Adrian", "Gabriela", "Jonathan"});
    departments.emplace_back("Gynaecology", std::vector<std::string>{"Olivia", "Ese", "Folusho"});
    departments.emplace_back("Gastroenterology", std::vector<std::string>{"Joshua", "Tarek", "Sinem"});
    departments.emplace_back("Pulmonology", std::vector<std::string>{"Elsada", "Nicole", "Angelina"});
    departments.emplace_back("Emergency", std::vector<std::string>{"Marcus", "Elena"});
}

Hospiditi::~Hospiditi() {
    delete[] appointments; //destructor for cleaning up dynamic array preventing leaks
}

void Hospiditi::expandAppointments() {
    int newSize = maxAppointments * 2; //doubles the capacity for when it reaches the maximum

    Appointment* newArr = nullptr; //temporary pointer

    try {
        newArr = new Appointment[newSize];

        for (int i = 0; i < appointmentCount; ++i) { //copies each appointment
            newArr[i] = appointments[i];
        }
        delete[] appointments;

        appointments = newArr;

        maxAppointments = newSize;
    } catch (const std::bad_alloc& e) {
        delete[] newArr;
        std::cerr << "Failed to expand appointments array: " << e.what() << std::endl;
        throw; //for error handling
    }
}

void Hospiditi::Save(std::ostream& os) const {
    try {
        os << appointmentCount << "\n"; //firstly saves the appointments

        for (int i = 0; i < appointmentCount; ++i) {
            const Appointment& a = appointments[i];
            os << a.name << ";" << a.department << ";" << a.doctor << ";" << a.day << ";" << a.time << "\n"; //<<for readability in the txt file
        }
    } catch (const std::exception& e) { //for file errors
        std::cerr << "Error saving appointments: " << e.what() << std::endl;
        throw;
    }
}

void Hospiditi::Load(std::istream& is) {
    try {
        appointmentCount = 0;
        int count = 0;
        is >> count;  //reads alll the appointments

        is.ignore(); //skips the newline


        for (int i = 0; i < count; ++i) {
            std::string line;
            std::getline(is, line);

            std::istringstream iss(line);

            std::string name, department, doctor, day, time;

            std::getline(iss, name, ';');
            std::getline(iss, department, ';');
            std::getline(iss, doctor, ';');
            std::getline(iss, day, ';');
            std::getline(iss, time);

            if (appointmentCount >= maxAppointments) expandAppointments();
            appointments[appointmentCount++] = {name, department, doctor, day, time}; //stores already existing files into current memory
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading appointments: " << e.what() << std::endl; //for error handling
        throw;
    }
}

std::string Hospiditi::getOpeningHours() const {
    return "Mon-Fri: 08:00-20:00\nSat-Sun: 08:30-18:00\nEmergency: 24/7";
}

bool Hospiditi::isOpen(const std::string& day, const std::string& time) const {
    std::string dayLower = lowercase(day);


    int h = std::stoi(time.substr(0, 2)); //gets a part of the string
    int m = std::stoi(time.substr(3, 2));
    int totalMins = h * 60 + m; //converts all time into total minutes, for easier understanding

    if (dayLower == "saturday" || dayLower == "sunday") {
        return totalMins >= 510 && totalMins < 1080; // 8:30 - 18:00
    } else {
        return totalMins >= 480 && totalMins < 1200; // 8:00 - 20:00
    }
}

std::vector<Department*> Hospiditi::getDepartments() const {
    std::vector<Department*> ptrs;

    for (const auto& d : departments) {
        ptrs.push_back(const_cast<IDepartment*>(&d));
    }
    return ptrs;
}

std::string Hospiditi::listDepartments() const {
    std::ostringstream oss;
    oss << "Departments:\n";

    for (size_t i = 0; i < departments.size(); ++i) {
        oss << i + 1 << ". " << departments[i].getName() << "\n";
    }
    return oss.str();
}

bool Hospiditi::Availability(const std::string& department, const std::string& doctor,
                                 const std::string& day, const std::string& time) const {
    for (int i = 0; i < appointmentCount; ++i) {
        const auto& a = appointments[i];
        if (lowercase(a.department) == lowercase(department) &&
            lowercase(a.doctor) == lowercase(doctor) &&
            lowercase(a.day) == lowercase(day) &&
            a.time == time) {
            return false;
        }
    }
    return true;
}

std::string Hospiditi::booking() {


    std::string name, department, doctor, day, time; //temporary variable holding users statements
    std::cout << "Booking appointment. Enter your name:\n";
    std::getline(std::cin, name);

    std::cout << listDepartments();

    std::cout << "Enter department:\n";
    std::getline(std::cin, department);

    auto it = std::find_if(departments.begin(), departments.end(),  //iterates through
                           [&](const IDepartment& d) { return lowercase(d.getName()) == lowercase(department); });
    if (it == departments.end()) return "Invalid department.";


    std::cout << "Doctors in " << it->getName() << ":\n"; //shows doctors and verifies
    for (const auto& d : it->getDoctors()) std::cout << "- " << d << "\n";

    std::cout << "Enter doctor name:\n";
    std::getline(std::cin, doctor);

    if (std::find_if(it->getDoctors().begin(), it->getDoctors().end(),
                     [&](const std::string& d) { return lowercase(d) == lowercase(doctor); }) == it->getDoctors().end())
        return "Doctor not found in department.";

    std::cout << "Enter day:\n";
    std::getline(std::cin, day);

    std::cout << "Enter time (Hour:Minutes):\n";
    std::getline(std::cin, time);

    if (!isOpen(day, time)) return "Hospital is closed at that time.";

    if (!Availability(department, doctor, day, time))
        return "Doctor is not available at that time.";

    if (appointmentCount >= maxAppointments) expandAppointments(); //saves appointment to memory

    appointments[appointmentCount++] = {name, department, doctor, day, time};
    return "Appointment booked successfully.";
}

std::string Hospiditi::listAppointments() const {
    std::ostringstream oss;
    oss << "Appointments:\n";

    for (int i = 0; i < appointmentCount; ++i) {
        oss << appointments[i].time << " - " << appointments[i].name << " with Dr. " << appointments[i].doctor << " (" << appointments[i].department << ")\n";
    }
    return oss.str();
}

std::string Hospiditi::getFAQ() const {
    return "FAQ:\n"
           "- Type 'book appointment' to schedule.\n"
           "- Type 'doctors' to list doctors.\n"
           "- Type 'departments' to list departments.\n"
           "- Type 'opening hours' to see hours.\n"
           "- Type 'today's appointments' to see today's bookings.\n"
           "- Type 'exit' to quit.\n";
}

std::string Hospiditi::processQuestion(const std::string& query) {

    std::string q = lowercase(query);

    if (q == "exit") return "exit";
    if (q == "help" || q == "faq") return getFAQ();
    if (q == "book appointment") return booking();
    if (q == "doctors") {

        std::ostringstream oss;
        oss << "Doctors:\n";

        for (const auto& d : departments) {
            oss << d.getName() << ":\n";

            for (const auto& doc : d.getDoctors()) oss << "- " << doc << "\n";
        }
        return oss.str();
    }

    if (q == "departments") return listDepartments();
    if (q == "opening hours") return getOpeningHours();
    if (q == "today's appointments") return listAppointments();
    return "Sorry, I didn't understand that. Type 'help' for options.";
}

//for ensuring text is lowercase, and case sensitive
std::string Hospiditi::lowercase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
