#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// --- Class Definition ---
class Student {
private:
    int id;
    string name;
    double gpa;

public:
    Student(int i, string n, double g) : id(i), name(n), gpa(g) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    double getGpa() const { return gpa; }

    // Display student details in a formatted row
    void display() const {
        cout << left << setw(10) << id 
             << setw(20) << name 
             << setw(5) << fixed << setprecision(2) << gpa << endl;
    }

    // Format for file storage
    string toFileData() const {
        return to_string(id) + "," + name + "," + to_string(gpa);
    }
};

// --- Manager Class to handle Logic ---
class ManagementSystem {
private:
    vector<Student> students;
    const string filename = "students.txt";

public:
    ManagementSystem() {
        loadFromFile();
    }

    void addStudent() {
        int id;
        string name;
        double gpa;

        cout << "\nEnter ID: ";
        while(!(cin >> id)) {
            cout << "Invalid input. Enter a numeric ID: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        cin.ignore(); // Clear buffer
        cout << "Enter Name: ";
        getline(cin, name);
        
        cout << "Enter GPA: ";
        cin >> gpa;

        students.emplace_back(id, name, gpa);
        saveToFile();
        cout << "Student added successfully!\n";
    }

    void showAll() {
        if (students.empty()) {
            cout << "\nNo records found.\n";
            return;
        }

        cout << "\n--- Student Records ---\n";
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(5) << "GPA" << endl;
        cout << string(35, '-') << endl;
        
        for (const auto& s : students) {
            s.display();
        }
    }

    void findStudent() {
        int id;
        cout << "\nEnter ID to search: ";
        cin >> id;

        auto it = find_if(students.begin(), students.end(), [id](const Student& s) {
            return s.getId() == id;
        });

        if (it != students.end()) {
            cout << "\nRecord Found:\n";
            it->display();
        } else {
            cout << "\nStudent with ID " << id << " not found.\n";
        }
    }

    void saveToFile() {
        ofstream outFile(filename);
        for (const auto& s : students) {
            outFile << s.toFileData() << endl;
        }
        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile(filename);
        if (!inFile) return;

        string line;
        while (getline(inFile, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find_last_of(',');
            
            int id = stoi(line.substr(0, pos1));
            string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            double gpa = stod(line.substr(pos2 + 1));
            
            students.emplace_back(id, name, gpa);
        }
        inFile.close();
    }
};

// --- Main Menu ---
int main() {
    ManagementSystem sms;
    int choice;

    do {
        cout << "\n=== University Management System ===";
        cout << "\n1. Add Student";
        cout << "\n2. View All Students";
        cout << "\n3. Search by ID";
        cout << "\n4. Exit";
        cout << "\nSelect option: ";
        cin >> choice;

        switch (choice) {
            case 1: sms.addStudent(); break;
            case 2: sms.showAll(); break;
            case 3: sms.findStudent(); break;
            case 4: cout << "Exiting program. Goodbye!\n"; break;
            default: cout << "Invalid choice, try again.\n";
        }
    } while (choice != 4);

    return 0;
}
