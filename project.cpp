#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

#ifdef _WIN32
#define PAUSE system("pause")
#else
#define PAUSE system("read -p 'Press any key to continue...' var")
#endif

void create_user();
bool login();
void main_menu();
void gpa(float *, char *, float [], int);
void create_record();
void display_record();
void display_specific();
void delete_record();
void save_records();
void load_records();

const int MAX_STUDENTS = 100;
const int MAX_PASSWORD_LENGTH = 20;

struct Student {
    int id;
    char name[50];
    float marks[5];
    float gpa;
    char grade;
};

Student students[MAX_STUDENTS];
int student_count = 0;

char username[] = "admin";
char password[MAX_PASSWORD_LENGTH] = "admin123";

int main() {
    create_user();
    if (login()) {
        load_records();
        main_menu();
    }
    save_records();
    return 0;
}

void create_user() {
    ifstream user_name_file_check("user_name.txt");
    ifstream pass_file_check("pass.txt");

    if (!user_name_file_check.is_open() || !pass_file_check.is_open()) {
        ofstream user_name_file("user_name.txt");
        user_name_file << username;
        user_name_file.close();

        ofstream pass_file("pass.txt");
        pass_file << password;
        pass_file.close();

        cout << "Default credentials are stored in user_name.txt and pass.txt.\n";
    } else {
        cout << "Credential files already exist. Skipping creation.\n";
    }
    user_name_file_check.close();
    pass_file_check.close();
}

bool login() {
    char input_username[50];
    char input_password[MAX_PASSWORD_LENGTH];

    cout << "Enter username: ";
    cin >> input_username;

    cout << "Enter password: ";
    cin >> input_password;

    ifstream file("user_name.txt");
    file >> username;
    file.close();
    file.open("pass.txt");
    file >> password;
    file.close();

    if (strcmp(input_username, username) == 0 && strcmp(input_password, password) == 0) {
        cout << "\nLogin successful.\n";
        return true;
    }
    cout << "Wanna try again? (Y/n): ";
    char x;
    cin >> x;
    if (x == 'y' || x == 'Y') {
        return login();
    }
    cout << "\nInvalid login.\n";
    return false;
}

void main_menu() {
    int choice;
    do {
        cout << "\n----- Main Menu -----\n";
        cout << "1. Create Record\n";
        cout << "2. Display All Records\n";
        cout << "3. Display Specific Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            create_record();
            break;
        case 2:
            display_record();
            break;
        case 3:
            display_specific();
            break;
        case 4:
            delete_record();
            break;
        case 5:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 5);
}

void gpa(float *gpa, char *grade, float marks[], int num_subjects) {
    float total = 0;
    for (int i = 0; i < num_subjects; i++)
        total += marks[i];

    float percentage = total / (num_subjects * 100.0);
    *gpa = percentage * 5;

    if (*gpa >= 4.5) *grade = 'A';
    else if (*gpa >= 4.0) *grade = 'B';
    else if (*gpa >= 3.5) *grade = 'C';
    else if (*gpa >= 3.0) *grade = 'D';
    else if (*gpa >= 2.5) *grade = 'E';
    else *grade = 'F';
}

void create_record() {
    if (student_count >= MAX_STUDENTS) {
        cout << "Maximum number of students reached.\n";
        return;
    }

    Student s;
    bool unique = false;
    do {
        cout << "Enter Student ID: ";
        cin >> s.id;
        unique = true;
        for (int i = 0; i < student_count; i++) {
            if (students[i].id == s.id) {
                cout << "ID already exists. Enter a unique ID.\n";
                unique = false;
                break;
            }
        }
    } while (!unique);

    cout << "Enter Student Name: ";
    cin.
ignore();
    cin.getline(s.name, 50);

    for (int i = 0; i < 5; i++) {
        do {
            cout << "Enter marks for subject " << (i + 1) << ": ";
            cin >> s.marks[i];
            if (s.marks[i] < 0 || s.marks[i] > 100)
                cout << "Invalid marks! Must be between 0 and 100.\n";
        } while (s.marks[i] < 0 || s.marks[i] > 100);
    }

    gpa(&s.gpa, &s.grade, s.marks, 5);
    students[student_count++] = s;

    cout << "Record created successfully!\n";
    PAUSE;
}

void display_record() {
    if (student_count == 0) {
        cout << "No records to display.\n";
    } else {
        cout << "\n--- Student Records ---\n";
        for (int i = 0; i < student_count; i++) {
            cout << "ID: " << students[i].id
                 << " | Name: " << students[i].name
                 << " | GPA: " << students[i].gpa
                 << " | Grade: " << students[i].grade << endl;
        }
    }
    PAUSE;
}

void display_specific() {
    int id;
    cout << "Enter Student ID to display: ";
    cin >> id;

    for (int i = 0; i < student_count; i++) {
        if (students[i].id == id) {
            cout << "\nID: " << students[i].id
                 << "\nName: " << students[i].name
                 << "\nGPA: " << students[i].gpa
                 << "\nGrade: " << students[i].grade << endl;
            PAUSE;
            return;
        }
    }
    cout << "Record not found.\n";
    PAUSE;
}

void delete_record() {
    int id;
    cout << "Enter Student ID to delete: ";
    cin >> id;

    for (int i = 0; i < student_count; i++) {
        if (students[i].id == id) {
            for (int j = i; j < student_count - 1; j++) {
                students[j] = students[j + 1];
            }
            student_count--;
            cout << "Record deleted successfully.\n";
            PAUSE;
            return;
        }
    }
    cout << "Record not found.\n";
    PAUSE;
}

void save_records() {
    ofstream file("students.dat", ios::binary);
    if (!file) {
        cerr << "Error opening file for saving.\n";
        return;
    }

    file.write((char *)&student_count, sizeof(student_count));
    file.write((char *)students, sizeof(Student) * student_count);
    file.close();

    cout << "Records saved successfully.\n";
}

void load_records() {
    ifstream file("students.dat", ios::binary);
    if (!file.is_open()) return;

    int count;
    file.read((char *)&count, sizeof(count));

    if (count < 0 || count > MAX_STUDENTS) {
        cout << "Corrupted data file. Ignoring.\n";
        file.close();
        return;
    }

    file.read((char *)students, sizeof(Student) * count);

    if (!file.good()) {
        cout << "Error reading file. Some data may be missing.\n";
        file.close();
        return;
    }

    student_count = count;
    file.close();
}