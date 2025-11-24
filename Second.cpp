#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip> // For formatting output

using namespace std;

// --- 1. Patient Class ---
class Patient {
public:
    int id;
    string name;
    string illness;
    int age;

    Patient() : id(0), age(0) {} // Default constructor

    // Parameterized constructor
    Patient(int _id, string _name, string _illness, int _age) 
        : id(_id), name(_name), illness(_illness), age(_age) {}

    // Method to get a string representation for file storage (CSV-like)
    string toFileString() const {
        stringstream ss;
        ss << id << "," << name << "," << illness << "," << age;
        return ss.str();
    }

    // Method to display patient details
    void display() const {
        cout << left << setw(10) << id 
             << setw(20) << name 
             << setw(25) << illness 
             << setw(5) << age << endl;
    }
};

// --- 2. Doctor Class ---
class Doctor {
public:
    int id;
    string name;
    string specialization;

    Doctor() : id(0) {}

    // Parameterized constructor
    Doctor(int _id, string _name, string _specialization)
        : id(_id), name(_name), specialization(_specialization) {}

    // Method to get a string representation for file storage
    string toFileString() const {
        stringstream ss;
        ss << id << "," << name << "," << specialization;
        return ss.str();
    }

    // Method to display doctor details
    void display() const {
        cout << left << setw(10) << id 
             << setw(20) << name 
             << setw(25) << specialization << endl;
    }
};

// --- 3. Hospital Manager Class (Core Logic and File Handling) ---
class HospitalManager {
private:
    const string PATIENT_FILE = "patients.txt";
    const string DOCTOR_FILE = "doctors.txt";
    vector<Patient> patients;
    vector<Doctor> doctors;
    int nextPatientId = 1;
    int nextDoctorId = 1;

    // Helper function to read patients from file
    void loadPatients() {
        patients.clear();
        ifstream file(PATIENT_FILE);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string segment;
            vector<string> seglist;
            
            // Split line by comma
            while(getline(ss, segment, ',')) {
                seglist.push_back(segment);
            }

            if (seglist.size() == 4) {
                try {
                    int id = stoi(seglist[0]);
                    int age = stoi(seglist[3]);
                    patients.push_back(Patient(id, seglist[1], seglist[2], age));
                    if (id >= nextPatientId) nextPatientId = id + 1;
                } catch (const std::exception& e) {
                    // Handle conversion error if file is corrupted
                    cerr << "Error loading patient data: " << e.what() << endl;
                }
            }
        }
        file.close();
    }

    // Helper function to save patients to file
    void savePatients() const {
        ofstream file(PATIENT_FILE);
        for (const auto& p : patients) {
            file << p.toFileString() << "\n";
        }
        file.close();
    }

    // Helper function to read doctors from file (Similar to loadPatients)
    void loadDoctors() {
        doctors.clear();
        ifstream file(DOCTOR_FILE);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string segment;
            vector<string> seglist;
            while(getline(ss, segment, ',')) {
                seglist.push_back(segment);
            }

            if (seglist.size() == 3) {
                try {
                    int id = stoi(seglist[0]);
                    doctors.push_back(Doctor(id, seglist[1], seglist[2]));
                    if (id >= nextDoctorId) nextDoctorId = id + 1;
                } catch (const std::exception& e) {
                    cerr << "Error loading doctor data: " << e.what() << endl;
                }
            }
        }
        file.close();
    }
    
    // Helper function to save doctors to file (Similar to savePatients)
    void saveDoctors() const {
        ofstream file(DOCTOR_FILE);
        for (const auto& d : doctors) {
            file << d.toFileString() << "\n";
        }
        file.close();
    }

public:
    // Constructor loads data upon system start
    HospitalManager() {
        loadPatients();
        loadDoctors();
    }

    // Destructor saves data upon system exit
    ~HospitalManager() {
        savePatients();
        saveDoctors();
    }

    // --- Patient Operations ---
    void addPatient() {
        string name, illness;
        int age;

        cout << "\n--- Add New Patient ---\n";
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Illness/Ailment: ";
        getline(cin, illness);
        cout << "Enter Age: ";
        while (!(cin >> age) || age <= 0) {
            cout << "Invalid age. Enter a positive number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        patients.push_back(Patient(nextPatientId++, name, illness, age));
        cout << "\nPatient added successfully! ID: " << nextPatientId - 1 << endl;
    }

    void displayAllPatients() const {
        if (patients.empty()) {
            cout << "\nNo patient records found.\n";
            return;
        }
        cout << "\n--- All Patient Records ---\n";
        cout << left << setw(10) << "ID" 
             << setw(20) << "Name" 
             << setw(25) << "Illness" 
             << setw(5) << "Age" << endl;
        cout << string(60, '-') << endl;
        for (const auto& p : patients) {
            p.display();
        }
    }

    // --- Doctor Operations ---
    void addDoctor() {
        string name, specialization;

        cout << "\n--- Add New Doctor ---\n";
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Specialization: ";
        getline(cin, specialization);

        doctors.push_back(Doctor(nextDoctorId++, name, specialization));
        cout << "\nDoctor added successfully! ID: " << nextDoctorId - 1 << endl;
    }

    void displayAllDoctors() const {
        if (doctors.empty()) {
            cout << "\nNo doctor records found.\n";
            return;
        }
        cout << "\n--- All Doctor Records ---\n";
        cout << left << setw(10) << "ID" 
             << setw(20) << "Name" 
             << setw(25) << "Specialization" << endl;
        cout << string(55, '-') << endl;
        for (const auto& d : doctors) {
            d.display();
        }
    }

    // --- Menu and Main Loop ---
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
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            switch (choice) {
                case 1: addPatient(); break;
                case 2: displayAllPatients(); break;
                case 3: addDoctor(); break;
                case 4: displayAllDoctors(); break;
                case 5: cout << "\nExiting System. Data saved to files.\n"; break;
                default: cout << "Invalid choice. Please try again.\n"; break;
            }

        } while (choice != 5);
    }
};

// --- 4. Driver Code ---
int main() {
    HospitalManager manager;
    manager.run();
    return 0;
}
