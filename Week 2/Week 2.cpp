#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <stdexcept>

using namespace std;

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

        ofstream outFile("students.txt", ios::app);
        if (!outFile.is_open()) {
            throw StudentException("Unable to open file for writing");
        }

        outFile << student.rollNumber << " " << student.name << " " << student.age << endl;
        if (outFile.fail()) {
            throw StudentException("Error writing to file");
        }
        outFile.close();
        cout << "Student added successfully!" << endl;
    }
    catch (const StudentException& e) {
        cout << "Error: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Unexpected error: " << e.what() << endl;
    }
}
