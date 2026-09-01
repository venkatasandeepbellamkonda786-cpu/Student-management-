/*
    Student Management System
    --------------------------
    A console-based C++ application that manages student records
    using file handling and menu-driven operations.

    Features:
      1. Add student
      2. Update student
      3. Delete student
      4. Display all students
      5. Search student by roll number
      0. Exit

    Data is stored persistently in "students.dat" (binary file).
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <limits>

using namespace std;

const char* FILENAME = "students.dat";

// ---------- Student Record ----------
struct Student {
    int rollNo;
    char name[50];
    char course[30];
    float marks;
};

// ---------- Utility: clear bad input ----------
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ---------- Utility: pause screen ----------
void pause_() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

// ---------- Check if a roll number already exists ----------
bool rollExists(int roll) {
    ifstream file(FILENAME, ios::binary);
    Student s;
    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo == roll) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// ---------- Add a new student ----------
void addStudent() {
    Student s;

    cout << "\n----- Add Student -----\n";
    cout << "Enter Roll Number: ";
    while (!(cin >> s.rollNo)) {
        cout << "Invalid input. Enter a numeric Roll Number: ";
        clearInputBuffer();
    }

    if (rollExists(s.rollNo)) {
        cout << "A student with Roll Number " << s.rollNo << " already exists!\n";
        clearInputBuffer();
        return;
    }

    clearInputBuffer();

    cout << "Enter Name: ";
    cin.getline(s.name, 50);

    cout << "Enter Course: ";
    cin.getline(s.course, 30);

    cout << "Enter Marks: ";
    while (!(cin >> s.marks)) {
        cout << "Invalid input. Enter numeric Marks: ";
        clearInputBuffer();
    }
    clearInputBuffer();

    ofstream file(FILENAME, ios::binary | ios::app);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }
    file.write(reinterpret_cast<char*>(&s), sizeof(Student));
    file.close();

    cout << "Student added successfully!\n";
}

// ---------- Display all students ----------
void displayAll() {
    ifstream file(FILENAME, ios::binary);
    if (!file) {
        cout << "\nNo records found.\n";
        return;
    }

    Student s;
    bool found = false;

    cout << "\n----- All Student Records -----\n";
    cout << left << setw(10) << "Roll No" << setw(20) << "Name"
         << setw(20) << "Course" << setw(10) << "Marks" << "\n";
    cout << string(60, '-') << "\n";

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        cout << left << setw(10) << s.rollNo << setw(20) << s.name
             << setw(20) << s.course << setw(10) << s.marks << "\n";
        found = true;
    }
    file.close();

    if (!found) {
        cout << "No records found.\n";
    }
}

// ---------- Search student by roll number ----------
void searchStudent() {
    int roll;
    cout << "\n----- Search Student -----\n";
    cout << "Enter Roll Number to search: ";
    while (!(cin >> roll)) {
        cout << "Invalid input. Enter a numeric Roll Number: ";
        clearInputBuffer();
    }
    clearInputBuffer();

    ifstream file(FILENAME, ios::binary);
    if (!file) {
        cout << "No records found.\n";
        return;
    }

    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo == roll) {
            cout << "\nRecord Found:\n";
            cout << "Roll No : " << s.rollNo << "\n";
            cout << "Name    : " << s.name << "\n";
            cout << "Course  : " << s.course << "\n";
            cout << "Marks   : " << s.marks << "\n";
            found = true;
            break;
        }
    }
    file.close();

    if (!found) {
        cout << "Student with Roll Number " << roll << " not found.\n";
    }
}

// ---------- Update a student's record ----------
void updateStudent() {
    int roll;
    cout << "\n----- Update Student -----\n";
    cout << "Enter Roll Number to update: ";
    while (!(cin >> roll)) {
        cout << "Invalid input. Enter a numeric Roll Number: ";
        clearInputBuffer();
    }
    clearInputBuffer();

    fstream file(FILENAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "No records found.\n";
        return;
    }

    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo == roll) {
            found = true;

            cout << "Current Name: " << s.name << "\nEnter New Name: ";
            cin.getline(s.name, 50);

            cout << "Current Course: " << s.course << "\nEnter New Course: ";
            cin.getline(s.course, 30);

            cout << "Current Marks: " << s.marks << "\nEnter New Marks: ";
            while (!(cin >> s.marks)) {
                cout << "Invalid input. Enter numeric Marks: ";
                clearInputBuffer();
            }
            clearInputBuffer();

            // Move file pointer back to overwrite this record
            streampos pos = file.tellp();
            pos -= static_cast<streamoff>(sizeof(Student));
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&s), sizeof(Student));
            break;
        }
    }
    file.close();

    if (found) {
        cout << "Student record updated successfully!\n";
    } else {
        cout << "Student with Roll Number " << roll << " not found.\n";
    }
}

// ---------- Delete a student's record ----------
void deleteStudent() {
    int roll;
    cout << "\n----- Delete Student -----\n";
    cout << "Enter Roll Number to delete: ";
    while (!(cin >> roll)) {
        cout << "Invalid input. Enter a numeric Roll Number: ";
        clearInputBuffer();
    }
    clearInputBuffer();

    ifstream file(FILENAME, ios::binary);
    if (!file) {
        cout << "No records found.\n";
        return;
    }

    ofstream temp("temp.dat", ios::binary);
    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo == roll) {
            found = true;   // skip writing this record -> effectively deletes it
            continue;
        }
        temp.write(reinterpret_cast<char*>(&s), sizeof(Student));
    }
    file.close();
    temp.close();

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found) {
        cout << "Student record deleted successfully!\n";
    } else {
        cout << "Student with Roll Number " << roll << " not found.\n";
    }
}

// ---------- Menu ----------
void showMenu() {
    cout << "\n==========================================\n";
    cout << "      STUDENT MANAGEMENT SYSTEM\n";
    cout << "==========================================\n";
    cout << "1. Add Student\n";
    cout << "2. Update Student\n";
    cout << "3. Delete Student\n";
    cout << "4. Display All Students\n";
    cout << "5. Search Student by Roll Number\n";
    cout << "0. Exit\n";
    cout << "==========================================\n";
    cout << "Enter your choice: ";
}

// ---------- Main ----------
int main() {
    int choice;

    do {
        showMenu();
        while (!(cin >> choice)) {
            cout << "Invalid input. Enter a numeric choice: ";
            clearInputBuffer();
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addStudent();     break;
            case 2: updateStudent();  break;
            case 3: deleteStudent();  break;
            case 4: displayAll();     break;
            case 5: searchStudent();  break;
            case 0: cout << "\nExiting... Thank you!\n"; break;
            default: cout << "\nInvalid choice. Please try again.\n";
        }

        if (choice != 0) pause_();

    } while (choice != 0);

    return 0;
}
