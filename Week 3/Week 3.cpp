#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <stdexcept>

using namespace std;
class StudentException : public runtime_error {
public:
    StudentException(const string& message) : runtime_error(message) {}
};

class Student {
public:
    int rollNumber;
    string name;
    int age;
    void getDetails() {
        try {
            cout << "Enter Roll Number: ";
            if (!(cin >> rollNumber)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw StudentException("Invalid roll number format");
            }
            if (rollNumber <= 0) {
                throw StudentException("Roll number must be positive");
            }
            cout << "Enter Name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);
            if (name.empty()) {
                throw StudentException("Name cannot be empty");
            }
            cout << "Enter Age: ";
            if (!(cin >> age)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw StudentException("Invalid age format");
            }
            if (age <= 0 || age > 120) {
                throw StudentException("Age must be between 1 and 120");
            }
        }
        catch (const StudentException& e) {
            throw;
        }
    }
    void displayDetails() const {
        cout << "Roll Number: " << rollNumber << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
    }
};

void loadStudents(vector<Student>& students) {
    ifstream inFile("students.data");
    if (inFile.is_open()) {
        Student student;
        while (inFile >> student.rollNumber) {
            inFile.ignore();
            getline(inFile, student.name, '\t');
            inFile >> student.age;
            students.push_back(student);
        }
        inFile.close();
    }
}
void saveStudentToFile(const Student& student) {
    ofstream outFile("students.data", ios::app);
    if (!outFile.is_open()) {
        throw StudentException("Unable to open file for writing");
    }
    outFile << student.rollNumber << "\t" << student.name << "\t" << student.age << endl;
    if (outFile.fail()) {
        throw StudentException("Error writing to file");
    }
    outFile.close();
}
void addStudent(vector<Student>& students) {
    try {
        Student student;
        student.getDetails();
        for (const auto& existingStudent : students) {
            if (existingStudent.rollNumber == student.rollNumber) {
                throw StudentException("Student with this roll number already exists");
            }
        }
        students.push_back(student);
        saveStudentToFile(student);
        cout << "Student added successfully!" << endl;
    }
    catch (const StudentException& e) {
        cout << "Error: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Unexpected error: " << e.what() << endl;
    }
}
void displayAllStudents(const vector<Student>& students) {
    try {
        if (students.empty()) {
            throw StudentException("No students to display");
        }
        for (const auto& student : students) {
            student.displayDetails();
            cout << "-----------------------" << endl;
        }
    }
    catch (const StudentException& e) {
        cout << "Error: " << e.what() << endl;
    }
}
void searchStudent(const vector<Student>& students) {
    try {
        int rollNumber;
        cout << "Enter Roll Number to search: ";
        if (!(cin >> rollNumber)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw StudentException("Invalid roll number format");
        }
        bool found = false;
        for (const auto& student : students) {
            if (student.rollNumber == rollNumber) {
                student.displayDetails();
                found = true;
                break;
            }
        }
        if (!found) {
            throw StudentException("Student not found");
        }
    }
    catch (const StudentException& e) {
        cout << "Error: " << e.what() << endl;
    }
}
int main() {
    vector<Student> students;
    loadStudents(students);
    int choice;
    do {
        cout << "\nStudent Management System\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addStudent(students);
            break;
        case 2:
            displayAllStudents(students);
            break;
        case 3:
            searchStudent(students);
            break;
        case 4:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 4);
    return 0;
}
