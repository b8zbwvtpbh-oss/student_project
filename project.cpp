#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#ifdef _WIN32
#define PAUSE system("pause")
#else
#define PAUSE system("read -p 'Press any key to continue...' var")
#endif
using namespace std;

void create_user();
bool login();
void main_menu();
void gpa(float *, char *, float []);
void create_record();
void display_record();
void display_specific();
void delete_record();
void save_records();
void load_records();

const int MAX_STUDENTS = 100;
const int MAX_PASSWORD_LENGTH = 20;
const int NUM_SUBJECTS=5;
struct Student {
    int id;
    char name[50];
    float marks[NUM_SUBJECTS];
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

void gpa( float* gpa, char* grade, float marks[]) {
    
    float totalMarks = 0;
    
    for (int i = 0; i < NUM_SUBJECTS; i++){
        totalMarks += marks[i];
    }
    
    float percentage = totalMarks / (NUM_SUBJECTS * 100.0);
    *gpa = percentage * 5;
    
    if (*gpa >= 4.5)
     *grade = 'A';
    else if (*gpa >= 4.0)
     *grade = 'B';
    else if (*gpa >= 3.5)
     *grade = 'C';
    else if (*gpa >= 3.0)
     *grade = 'D';
    else if (*gpa >= 2.5)
     *grade = 'E';
    else 
     *grade = 'F';
}

void create_record() {
    if (student_count >= MAX_STUDENTS) {
        cerr << "Error: Maximum number of students reached!\n";
        PAUSE;
        return;
    }

    int id;
    bool recordID;

    do {
        recordID = false;

        cout << "Enter student ID: ";
        cin >> id;

        for (int i = 0; i < student_count; i++) {
            if (students[i].id == id) {
                recordID = true;
                cerr << "Error: ID already exists. Enter a unique ID!\n";
                break;
            }
        }
    } while (recordID);

    students[student_count].id = id;

    cout << "Enter student name: ";
    cin.ignore();
    cin.getline(students[student_count].name, 50);

    cout << "Enter marks for "<<NUM_SUBJECTS<<" subjects (0 - 100):\n";
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        do {
            cout << "Subject " << i + 1 << ": ";
            cin >> students[student_count].marks[i];

            if (students[student_count].marks[i] < 0 || students[student_count].marks[i] > 100) {
                cerr << "Error: Invalid input, enter a number between 0 and 100." << endl;
            }
        } while (students[student_count].marks[i] < 0 || students[student_count].marks[i] > 100);
    }

    gpa(&students[student_count].gpa, &students[student_count].grade, students[student_count].marks);

    cout << "Record added successfully!" << endl;
    cout << "GPA: " << students[student_count].gpa << endl;
    cout << "Grade: " << students[student_count].grade << endl;

    student_count++;
    PAUSE;
}

void display_record()
{
    if (student_count == 0)
    {
        cerr << "\nError: No student records available to display!\n";
    }
    else
    {
        cout << "\n^All Student Records^\n";
        
        for (int i = 0; i < student_count; i++)
        {
            cout << "----------------------------------------"<<endl;
            cout << "ID: " << students[i].id << endl;
            cout << "Name: " << students[i].name << endl;
            cout << "GPA: " << fixed << setprecision(2) << students[i].gpa << endl;
            cout << "Grade: " << students[i].grade << endl;
        }
        cout << "----------------------------------------"<<endl;
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
    cerr << "Error: Record not found!\n";
    PAUSE;
}

void delete_record() 
{
    if (student_count == 0)
    {
        cerr << "Error : No student records to delete! \n";
        PAUSE;
        return;
    }
    
    int Id_To_Delete;
    
    cout << "Enter the student ID to delete : ";
    cin >> Id_To_Delete;
    
    int index = -1;
    
    for (int i = 0 ; i < student_count ; i++) 
    {
        if (students[i].id == Id_To_Delete)
        {
            index = i;
            break;
        }
    }
    
    if (index == -1) 
    {
        cerr << "Error : Student ID not found! \n";
        PAUSE;
        return;
    }
    
    for (int i = index ; i < student_count - 1 ; i++) 
    {
        students[i] = students[i + 1];
    }

    student_count--;
    
    cout << "Student record deleted successfully .\n";
    PAUSE;
}

void save_records()
{
    ofstream stu_data("students.dat", ios::binary);
    if (!stu_data) {
        cerr << "Error: Could not open file for saving!\n";
        return;
    }

    stu_data.write(reinterpret_cast<char*>(&student_count), sizeof(student_count));
    stu_data.write(reinterpret_cast<char*>(students), sizeof(Student) * student_count);

    if (stu_data.good())
        cout << "Records saved successfully.\n";
    else
        cerr << "Error: Failed while writing to file!\n";

    stu_data.close();
}

void load_records()
{
    ifstream file("students.dat", ios::binary);
    
    if (!file)
    {
        cerr << "Error: Could not open file for loading! ";
        return;
    }

    int count_from_file = 0;
    
    file.read(reinterpret_cast<char*>(&count_from_file), sizeof(count_from_file));
    
    if (count_from_file < 0 || count_from_file > MAX_STUDENTS)
    {
        cerr << "Error: Invalid student count. File may be corrupted!\n";
        file.close();
        return;
    }
     file.read(reinterpret_cast<char*>(students), sizeof(Student) * count_from_file);
     
      if (!file.good())
    {
        cerr << "Error: Failed reading student records! \n";
        file.close();
        return;
    }
    
     student_count = count_from_file;
     
      file.close();
      cout<<"\nRecords loaded successfully.\n";
}

