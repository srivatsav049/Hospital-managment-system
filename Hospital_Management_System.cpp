#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

class Person {
    
protected:
    string name;
    int age; 
    string gender;
public:
    Person() {}
    Person(const string& _name, int _age, const string& _gender) : name(_name), age(_age), gender(_gender) {}

    virtual void displayDetails() const {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Gender: " << gender << endl;
    }

    virtual void inputDetails() {
        cout << "Enter name: ";
        getline(cin, name);
        cout << "Enter age: ";
        cin >> age;
        cin.ignore(); 
        cout << "Enter gender: ";
        getline(cin, gender);
    }

    const string& getName() const {
        return name;
    }
};

class Doctor : public Person {
protected:
    string specialization;
public:
    Doctor() {}
    Doctor(const string& _name, int _age, const string& _gender, const string& _specialization)
        : Person(_name, _age, _gender), specialization(_specialization) {}

    void displayDetails() const override {
        cout << "Doctor Details:" << endl;
        Person::displayDetails();
        cout << "Specialization: " << specialization << endl;
    }

    void inputDetails() override {
        Person::inputDetails();
        cout << "Enter specialization: ";
        getline(cin, specialization);
    }
};

class Patient : public Person {
private:
    string condition;
public:
    Patient() {}
    Patient(const string& _name, int _age, const string& _gender, const string& _condition)
        : Person(_name, _age, _gender), condition(_condition) {}

    void displayDetails() const override {
        cout << "Patient Details:" << endl;
        Person::displayDetails();
        cout << "Condition: " << condition << endl;
    }

    void inputDetails() override {
        Person::inputDetails();
        cout << "Enter condition: ";
        getline(cin, condition);
    }
};

class Appointment {
private:
    time_t dateTime;
    Doctor* doctor;
    Patient* patient;
public:
    Appointment(time_t _dateTime, Doctor* _doctor, Patient* _patient)
        : dateTime(_dateTime), doctor(_doctor), patient(_patient) {}

    void displayDetails() const {
        cout << "Appointment Details:" << endl;
        cout << "Date and Time: " << ctime(&dateTime);
        cout << "Doctor: " << doctor->getName() << endl;
        cout << "Patient: " << patient->getName() << endl;
    }
};

class Treatment {
private:
    string description;
    double cost;
    Patient* patient;
    friend class Hospital;
public:
    Treatment(const string& _description, double _cost, Patient* _patient)
        : description(_description), cost(_cost), patient(_patient) {}

    void displayDetails() const {
        cout << "Treatment Details:" << endl;
        cout << "Description: " << description << endl;
        cout << "Cost: Rs" << cost << endl;
        cout << "Patient: " << patient->getName() << endl;
    }

    double getCost() const {
        return cost;
    }
};

class Bill {
private:
    Patient* patient;
    double totalAmount;
public:
    Bill(Patient* _patient) : patient(_patient), totalAmount(0) {}

    void addTreatment(const Treatment& treatment) {
        totalAmount += treatment.getCost();
    }

    void displayDetails() const {
        cout << "Billing Details for Patient: " << patient->getName() << endl;
        cout << "Total Amount: Rs" << totalAmount << endl;
    }
};

class Hospital {
private:
    vector<Doctor> doctors;
    vector<Patient> patients;
    vector<Appointment> appointments;
    vector<Treatment> treatments;
    vector<Bill> bills;
public:
    Patient* getPatientAtIndex(int index) {
        if (index >= 0 && index < patients.size()) {
            return &patients[index];
        } else {
            return nullptr; 
        }
    }
    Doctor* getDoctorAtIndex(int index) {
        if (index >= 0 && index < patients.size()) {
            return &doctors[index];
        } else {
            return nullptr; 
        }
    }
    void addDoctor(const Doctor& doctor) {
        doctors.push_back(doctor);
    }

    void addPatient(const Patient& patient) {
        patients.push_back(patient);
    }

    void scheduleAppointment(const time_t& dateTime, Doctor* doctor, Patient* patient) {
        appointments.emplace_back(dateTime, doctor, patient);
    }

    void administerTreatment(const string& description, double cost, Patient* patient) {
        treatments.emplace_back(description, cost, patient);
    }

    void generateBill(Patient* patient) {
        Bill bill(patient);
        for (const auto& treatment : treatments) {
            if (treatment.patient == patient) {
                bill.addTreatment(treatment);
            }
        }
        bills.push_back(bill);
    }

    void displayDoctors() const {
        cout << "List of Doctors:" << endl;
        for (const auto& doctor : doctors) {
            doctor.displayDetails();
            cout << endl;
        }
    }

    void displayPatients() const {
        cout << "List of Patients:" << endl;
        for (const auto& patient : patients) {
            patient.displayDetails();
            cout << endl;
        }
    }

    void displayAppointments() const {
        cout << "List of Appointments:" << endl;
        for (const auto& appointment : appointments) {
            appointment.displayDetails();
            cout << endl;
        }
    }

    void displayTreatments() const {
        cout << "List of Treatments:" << endl;
        for (const auto& treatment : treatments) {
            treatment.displayDetails();
            cout << endl;
        }
    }

    void displayBills() const {
        cout << "List of Bills:" << endl;
        for (const auto& bill : bills) {
            bill.displayDetails();
            cout << endl;
        }
    }
};

int main() {
    Hospital hospital;

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Add Doctor\n";
        cout << "2. Add Patient\n";
        cout << "3. Schedule Appointment\n";
        cout << "4. Administer Treatment\n";
        cout << "5. Generate Bills\n";
        cout << "6. Display Doctors\n";
        cout << "7. Display Patients\n";
        cout << "8. Display Appointments\n";
        cout << "9. Display Treatments\n";
        cout << "10. Display Bills\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1: {
                Doctor doctor;
                cout << "\nEnter Doctor Details:\n";
                doctor.inputDetails();
                hospital.addDoctor(doctor);
                break;
            }
            case 2: {
                Patient patient;
                cout << "\nEnter Patient Details:\n";
                patient.inputDetails();
                hospital.addPatient(patient);
                break;
            }
            case 3: {
                int doctorIndex, patientIndex;
                cout << "\nEnter Doctor Index: ";
                cin >> doctorIndex;
                cout << "Enter Patient Index: ";
                cin >> patientIndex;
                time_t dateTime = time(0); 
                hospital.scheduleAppointment(dateTime, hospital.getDoctorAtIndex(doctorIndex), hospital.getPatientAtIndex(patientIndex));
                break;
            }
            case 4: {
                int patientIndex;
                string description;
                double cost;
                cout << "\nEnter Patient Index: ";
                cin >> patientIndex;
                cin.ignore(); 
                cout << "Enter Treatment Description: ";
                getline(cin, description);
                cout << "Enter Treatment Cost: ";
                cin >> cost;
                hospital.administerTreatment(description, cost, hospital.getPatientAtIndex(patientIndex));
                break;
            }
            case 5: {
                int patientIndex;
                cout << "\nEnter Patient Index: ";
                cin >> patientIndex;
                hospital.generateBill(hospital.getPatientAtIndex(patientIndex));
                break;
            }
            case 6:
                hospital.displayDoctors();
                break;
            case 7:
                hospital.displayPatients();
                break;
            case 8:
                hospital.displayAppointments();
                break;
            case 9:
                hospital.displayTreatments();
                break;
            case 10:
                hospital.displayBills();
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
