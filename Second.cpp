#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

// ----------------------------------------
// Utility Functions
// ----------------------------------------
namespace Utils {
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    bool isNumber(const string &s) {
        for (char c : s) if (!isdigit(c)) return false;
        return !s.empty();
    }
}

// ----------------------------------------
// Patient Class
// ----------------------------------------
class Patient {
public:
    int id;
    string name;
    string illness;
    int age;

    Patient() : id(0), age(0) {}
    Patient(int _id, string _name, string _illness, int _age)
        : id(_id), name(_name), illness(_illness), age(_age) {}

    string toFileString() const {
        stringstream ss;
        ss << id << "," << name << "," << illness << "," << age;
        return ss.str();
    }

    void display() const {
        cout << left << setw(10) << id
             << setw(20) << name
             << setw(25) << illness
             << setw(5) << age << "\n";
    }
};

// ----------------------------------------
// Doctor Class
// ----------------------------------------
class Doctor {
public:
    int id;
    string name;
    string specialization;

    Doctor() : id(0) {}
    Doctor(int _id, string _name, string _specialization)
        : id(_id), name(_name), specialization(_specialization) {}

    string toFileString() const {
        stringstream ss;
        ss << id << "," << name << "," << specialization;
        return ss.str();
    }

    void display() const {
        cout << left << setw(10) << id
             << setw(20) << name
             << setw(25) << specialization << "\n";
    }
};

// ----------------------------------------
// Hospital Manager Class
// ----------------------------------------
class HospitalManager {
private:
    const string PATIENT_FILE = "patients.txt";
    const string DOCTOR_FILE = "doctors.txt";
    vector<Patient> patients;
    vector<Doctor> doctors;
    int nextPatientId = 1;
    int nextDoctorId = 1;

    void loadPatients() {
        patients.clear();
        ifstream file(PATIENT_FILE);
        string line;

        while (getline(file, line)) {
            vector<string> seg;
            stringstream ss(line);
            string part;
            while (getline(ss, part, ',')) seg.push_back(part);

            if (seg.size() == 4 && Utils::isNumber(seg[0]) && Utils::isNumber(seg[3])) {
                int id = stoi(seg[0]);
                int age = stoi(seg[3]);
                patients.emplace_back(id, seg[1], seg[2], age);
                nextPatientId = max(nextPatientId, id + 1);
            }
        }
    }

    void savePatients() const {
        ofstream file(PATIENT_FILE);
        for (const auto &p : patients) file << p.toFileString() << "\n";
    }

    void loadDoctors() {
        doctors.clear();
        ifstream file(DOCTOR_FILE);
        string line;
        while (getline(file, line)) {
            vector<string> seg;
            stringstream ss(line);
            string part;
            while (getline(ss, part, ',')) seg.push_back(part);

            if (seg.size() == 3 && Utils::isNumber(seg[0])) {
                int id = stoi(seg[0]);
                doctors.emplace_back(id, seg[1], seg[2]);
                nextDoctorId = max(nextDoctorId, id + 1);
            }
        }
    }

    void saveDoctors() const {
        ofstream file(DOCTOR_FILE);
        for (const auto &d : doctors) file << d.toFileString() << "\n";
    }

    // Enhanced validation for names
    string inputString(const string &prompt) {
        string s;
        cout << prompt;
        Utils::clearInput();
        getline(cin, s);
        return s;
    }

    int inputPositiveNumber(const string &prompt) {
        int n;
        cout << prompt;
        while (!(cin >> n) || n <= 0) {
            cout << "Invalid input. Enter a positive number: ";
            Utils::clearInput();
        }
        return n;
    }

public:
    HospitalManager() {
        loadPatients();
        loadDoctors();
    }

    ~HospitalManager() {
        savePatients();
        saveDoctors();
    }

    // ------------------ Patient Operations ------------------
    void addPatient() {
        cout << "\n--- Add New Patient ---\n";
        string name = inputString("Enter Name: ");
        string illness = inputString("Enter Illness/Ailment: ");
        int age = inputPositiveNumber("Enter Age: ");

        patients.emplace_back(nextPatientId++, name, illness, age);
        cout << "\n✔ Patient added successfully! Assigned ID: " << nextPatientId - 1 << "\n";
    }

    void displayAllPatients() const {
        if (patients.empty()) {
            cout << "\nNo patient records found.\n";
            return;
        }
        cout << "\n--- All Patient Records ---\n";
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(25) << "Illness" << setw(5) << "Age" << "\n";
        cout << string(60, '-') << "\n";
        for (const auto &p : patients) p.display();
    }

    // ------------------ Doctor Operations ------------------
    void addDoctor() {
        cout << "\n--- Add New Doctor ---\n";
        string name = inputString("Enter Name: ");
        string specialization = inputString("Enter Specialization: ");

        doctors.emplace_back(nextDoctorId++, name, specialization);
        cout << "\n✔ Doctor added successfully! Assigned ID: " << nextDoctorId - 1 << "\n";
    }

    void displayAllDoctors() const {
        if (doctors.empty()) {
            cout << "\nNo doctor records found.\n";
            return;
        }
        cout << "\n--- All Doctor Records ---\n";
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(25) << "Specialization" << "\n";
        cout << string(55, '-') << "\n";
        for (const auto &d : doctors) d.display();
    }

    // ------------------ System Menu ------------------
    void run() {
        int choice;
        do {
            cout << "\n=============================================\n";
            cout << " HOSPITAL MANAGEMENT SYSTEM\n";
            cout << "=============================================\n";
            cout << "1. Add New Patient\n";
            cout << "2. Display All Patients\n";
            cout << "3. Add New Doctor\n";
            cout << "4. Display All Doctors\n";
            cout << "5. Exit System (Data Saved)\n";
            cout << "---------------------------------------------\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number.\n";
                Utils::clearInput();
                continue;
            }

            switch (choice) {
                case 1: addPatient(); break;
                case 2: displayAllPatients(); break;
                case 3: addDoctor(); break;
                case 4: displayAllDoctors(); break;
                case 5: cout << "\nExiting System. Data saved successfully.\n"; break;
                default: cout << "Invalid choice. Try again.\n"; break;
            }

        } while (choice != 5);
    }
};

// ----------------------------------------
// Main Function
// ----------------------------------------
int main() {
    HospitalManager manager;
    manager.run();
    return 0;
}

