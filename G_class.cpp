#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <sstream>
#include <sys/stat.h>  //library
#include <regex>
#include <iomanip>   //tabular data
#include <cstdlib>    //exit
#include <stdlib.h>
using namespace std;

bool isValidEmail(const string& email)
 {
     const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"); 
    return regex_match(email, pattern);

};

void clearScreen() 
{
    system("clear");  //windows=>cls
};
bool flag=1;


class User 
{
    public:
    string fullname;
    string email;
    string password;
    string role;
    
    void my_account(); 
    
};

void User::my_account() 
{
       
        cout << "\n******** USER PROFILE *******" << endl;
        
        cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
        
        cout << left << setw(15) << "Full Name >>>>" << setw(30) << fullname << endl;
        cout << left << setw(15) << "Email  >>>>" << setw(30) << email << endl;
        cout << left << setw(15) << "Role  >>>>" << setw(30) << role << endl;
        cout << "\n\n";

    };


class Assignment 
{
   public:
    int assignment_id;
    string title;
    string description;
    string dueDate;
    int courseId;
    Assignment(int id, string title, string description,string date,int courseId) {

        this->assignment_id = id;
        this->title = title;
        this->description = description;
        this->dueDate=date;
        this->courseId=courseId;
    };

    Assignment(){};

    void getDetails()
    {
        cout << "The title is: ";
        cout << this->title<<" "<<endl;
        cout << "The description is: ";
        cout << this->description << " "<<endl;
        
    }
};

class  Classroom;
class Teacher : public User {
public:
    vector<string> mycourses;
    Teacher(string fName, string eMail, string Password) {
        this->fullname = fName;
        this->email = eMail;
        this->password = Password;
        this->role = "teacher";
    }
    Teacher(){};

    void gradeAssignment(int submissionId, int assignmentId, int studentId, const string& newGrade);
    void teacherOptions(Classroom& c);
    void viewSubmissionsByCourseID(int courseId);

    void my_account() {
       
        cout << "\n******** TEACHER PROFILE *******" << endl;
        cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
        cout << left << setw(15) << "Full Name >>>>" << setw(30) << fullname << endl;
        cout << left << setw(15) << "Email  >>>>" << setw(30) << email << endl;
        cout << left << setw(15) << "Role  >>>>" << setw(30) << "Teacher" << endl;
        cout << "\n\n";

    };
    

};



void Teacher::gradeAssignment(int submissionId, int assignmentId, int studentId, const string& newGrade) 
{
    const string filename = "database/submissions.csv";
    vector<string> updatedSubmissions; 
    bool submissionFound = false; 

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open submissions file." << endl;
        return;
    }

    string line;

    while (getline(file, line)) {
        istringstream ss(line);
        int currentSubmissionId, submissionAssignmentId, submissionStudentId;
        string submissionDate, currentGrade, feedback, filePath;

       
        ss >> currentSubmissionId;
        ss.ignore(); 
        ss >> submissionAssignmentId;
        ss.ignore(); 
        ss >> submissionStudentId;
        ss.ignore(); 
        getline(ss, submissionDate, ',');
        getline(ss, currentGrade, ',');
        getline(ss, feedback, ',');
        getline(ss, filePath); 

        if (currentSubmissionId == submissionId) {
            currentGrade = newGrade; 
            submissionFound = true;
            cout << "Updated grade for Student ID " << studentId << " for Submission ID " << submissionId << " to: " << currentGrade << endl;
        }

        updatedSubmissions.push_back(to_string(currentSubmissionId) + "," + 
                                      to_string(submissionAssignmentId) + "," + 
                                      to_string(submissionStudentId) + "," + 
                                      submissionDate + "," + 
                                      currentGrade + "," + 
                                      feedback + "," + 
                                      filePath);
    }

    file.close(); 

   
    ofstream outfile(filename, ios::trunc); 
    if (!outfile.is_open()) {
        cout << "Error: Unable to open submissions file for writing." << endl;
        return;
    }
 for (int i = 0; i < updatedSubmissions.size(); ++i) {
    outfile << updatedSubmissions[i] << endl;
}
    outfile.close(); 

    if (!submissionFound) {
        cout << "No submission found with ID " << submissionId << "." << endl;
    }
}


void Teacher::viewSubmissionsByCourseID(int courseId) {
    ifstream file("database/submissions.csv");
    if (!file.is_open()) {
        cout << "No submissions found..." << endl;
        return;
    }

    string line;
    cout << "\nSubmissions for Course ID " << courseId << ":\n\n";
    cout << left << setw(15) << "Submission ID" 
              << setw(15) << "Assignment ID" 
              << setw(15) << "Student ID" 
              << setw(20) << "Submission Date" 
              << setw(15) << "Grade" 
              << setw(20) << "File Path" << endl;
    cout << string(100, '-') << endl;

    while (getline(file, line)) {
        istringstream ss(line);
        int id, assignmentID, studentID;
        string submissionDate, grade, feedback, filePath, courseIDStr;

        getline(ss, courseIDStr, ','); 
        if (stoi(courseIDStr) == courseId) { 
            ss >> id; 
            ss.ignore(); 
            ss >> assignmentID; 
            ss.ignore(); 
            ss >> studentID;
            ss.ignore(); 
            getline(ss, submissionDate, ',');
            getline(ss, grade, ',');
            getline(ss, feedback, ',');
            getline(ss, filePath); 
            cout << left << setw(15) << id 
                      << setw(15) << assignmentID 
                      << setw(15) << studentID 
                      << setw(20) << submissionDate 
                      << setw(15) << grade 
                      << setw(20) << filePath << endl;
        }
    }

    file.close();
}


class Submission {
public:
    int submissionID;
    int assignmentID;
    int studentID;
    string submissionDate;
    string grade;
    string feedback;
    string filePath;
  public:

    Submission(int id, int aID, int sID, const string& date, const string& g, const string& fb, const string& path)
        : submissionID(id), assignmentID(aID), studentID(sID), submissionDate(date), grade(g), feedback(fb), filePath(path) {}
};

void addSubmission(const Submission& submission) {
    ofstream outfile("database/submissions.csv", ios::app);
 
    outfile << submission.submissionID << ","
            << submission.assignmentID << ","
            << submission.studentID << ","
            << submission.submissionDate << ","
            << submission.grade << ","
            << submission.feedback << ","
            << submission.filePath << "\n";
           outfile.close();
}


class Student : public User 
{
public:
    int studentId;

    Student(){};
    Student(string fName, string eMail, string Password, int id) 
    {
        this->fullname = fName;
        this->email = eMail;
        this->password = Password;
        this->role = "student";
        this->studentId = id;
    }

    void my_account() 
    {
        cout << "\n ------------PROFILE ---------------------\n";
        cout << "Student Name: " << fullname << ", Email: " << email << ", ID: " << studentId << endl;
    }

    vector <string> viewCourses() 
    {
    clearScreen();
    ifstream infile("./database/enrollments.csv");
    string line;
    vector<string> courses;

 while (getline(infile, line)) {
    stringstream ss(line);
    string studentIdStr, courseIdStr, courseN;

    getline(ss, studentIdStr, ',');
    getline(ss, courseIdStr, ',');
    getline(ss, courseN, ',');

    int studentId = stoi(studentIdStr);
    int courseId = stoi(courseIdStr);

    if (studentId == this->studentId) {
       courses.push_back(courseN); 
    }
}
infile.close();
    return courses;
    };
          
     void joinCourse(Classroom &c);
    void studentOptions(Classroom& c);
    void displayAssignmentsForCourse(const string& courseName);
    void submitAssignment();
    int getNextSubmissionID();

};

int Student::getNextSubmissionID() {
    ifstream infile("database/submissions.csv");
    int id = 0;
    string line;

    while (getline(infile, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos) {
            int currentID = stoi(line.substr(0, pos)); // Get the submission ID
            id = max(id, currentID);
        }
    }
    infile.close();
    return id + 1; 
}

void Student::submitAssignment() {
    int assignmentID;
    string filePath, grade = "Not Graded", feedback = "No Feedback", submissionDate;

    cout << "Enter the assignment ID: ";
    cin >> assignmentID;

    cout << "Enter the local file path for the assignment: ";
    cin.ignore(); 
    getline(cin, filePath); 
    time_t now = time(0);
    submissionDate = ctime(&now); 

    Submission submission(getNextSubmissionID(), assignmentID, this->studentId, submissionDate, grade, feedback, filePath);

    addSubmission(submission);
    cout << "Assignment submitted successfully!" << endl;
}



int getCourseIdFromName(string courseName) {
    ifstream infile("./database/courses.csv");
    string line;

    while (getline(infile, line)) {
        stringstream ss(line);
        string idStr, name;
        getline(ss, idStr, ',');
        getline(ss, name, ',');

        if (name == courseName) {
            return stoi(idStr); 
        }
    }
    return -1; 
}



void Student::displayAssignmentsForCourse(const string& courseName) {
    ifstream file("./database/assignments.csv"); 
    if (!file.is_open()) {
        cout << "No assignments yet..." << endl; 
        return; 
    }

    string line;
    cout << "Assignments for " << courseName << ":\n\n";
    cout << left << setw(30) << "Assignment Title" << "Due-Date" << endl;
    cout << string(50, '-') << endl;
    
    int id = getCourseIdFromName(courseName);
    cout << "Course ID: " << id << endl;

    while (getline(file, line)) {
        string assignmentId, courseId, title, description, dueDate;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);

        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos && pos4 != string::npos) {
            assignmentId = line.substr(0, pos1);
            courseId = line.substr(pos1 + 1, pos2 - pos1 - 1);
            title = line.substr(pos2 + 1, pos3 - pos2 - 1);
            description = line.substr(pos3 + 1, pos4 - pos3 - 1);
            dueDate = line.substr(pos4 + 1);

      
                if (!courseId.empty()) {
                    if (id == stoi(courseId)) {
                        cout <<left << setw(30) <<  "TITLE" << setw(30) << "ASSIGNMENT-ID" << setw(30) << "DESCRIPTION" << setw(30) <<  "DUE-DATE" << endl;
                        cout << left << setw(30) << title << setw(30) << assignmentId << setw(30)  << description << setw(30) << "   "  <<  dueDate << endl;
                    }
                } else {
                    cout << "Warning: courseId is empty in line: " << line << endl;
                }
        
        }
    }
    file.close();
}




class Course 
{
public:
    int courseId;
    string courseName;
    string teacherName;
    vector<Student*>students;

    Course(string name,string teacherName) 
    {
       this->courseId = rand() % 10000 + 5; 
        this->courseName = name;
        this->teacherName=teacherName;
    }
   
    void registerStudent(Student* s) 
    {

        students.push_back(s);
        cout << "Student " << s->fullname << " enrolled in " << courseName << endl;

    }

    void addAssignment(Assignment* a)
     {
        ofstream outfile("./database/assignments.csv", ios::app);
        outfile << a->assignment_id << "," << this->courseId <<"," << a->title << "," << a->description << "," << a->dueDate<< endl;
        cout << "Assignment " << a->title << " added to the course " << courseName << endl;
    }


    void courseOptions(Teacher* teacher);
};



class Announcement {
private:
    int announcementId;
    int courseId;
    string title;
    string path;

public:
   
    Announcement(int id, int cId, const string& t, const string& m)
        : announcementId(id), courseId(cId), title(t), path(m) {}

    void saveToCSV() {
        ofstream file("./database/announcements.csv", ios::app);
        if (file.is_open()) {
            file << announcementId << "," << courseId << "," << title << "," << path << "\n";
            file.close();
            cout << "Announcement saved successfully.\n";
        } else {
            cout << "Error: Unable to open file for saving announcements.\n";
        }
    }

    // friend ostream& operator<<(ostream& os, const Announcement& ann) {
    //     os << "-----------------------------------------\n";
    //     os << "Announcement ID: " << ann.announcementId << "\n";
    //     os << "Course ID: " << ann.courseId << "\n";

    //     os << "Title: " << ann.title << "\n";
    //     os << "Message: " << ann.path << "\n";
    //     os << "-------------------------------------------\n";
    //     return os;
    // }

    friend ostream& operator<<(ostream& os, const Announcement& ann) {
    os << "-----------------------------------------\n";
    os << "Announcement ID: " << ann.announcementId << "\n";
    os << "Course ID: " << ann.courseId << "\n";
    os << "Title: " << ann.title << "\n";

    // Attempt to open the file at the specified path
    std::ifstream file(ann.path);  // ann.path contains the file path
    if (file.is_open()) {
        // Read the file content
        std::stringstream buffer;
        buffer << file.rdbuf();   // Load entire file into buffer

        // Display the file content
        os << "Message: \n" << buffer.str() << "\n";
        
        // Close the file
        file.close();
    } else {
        // If the file could not be opened, display a message
        os << "Message: Could not open file at path " << ann.path << "\n";
    }

    os << "-------------------------------------------\n";
    return os;
}
   
    static void displayAnnouncementsForCourse(int courseId) {
        ifstream file("./database/announcements.csv");
        if (!file.is_open()) {
            cout << "No announcements available...\n";
            return;
        }

        string line;
        bool found = false;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, cid, title, path;

            getline(ss, id, ',');
            getline(ss, cid, ',');
            getline(ss, title, ',');
            getline(ss, path, ',');

            int x = stoi(cid);
            if (x == courseId) {
                Announcement ann(stoi(id), x, title, path);
                cout << ann; 
                found = true;
            }
        }

        if (!found) {
            cout << "No announcements found for course: " << courseId << "\n";
        }

        file.close();
    }
};


class Classroom 
  {
    public:
    string classroomName;
    vector<Course*> courses;
     void load_courses_data(const string& filename)
      {
      ifstream infile(filename);
      string line;
        while (getline(infile, line))
         {
            stringstream ss(line);
            string idStr, name, teacherName;
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, teacherName, ',');
            int id = stoi(idStr);
            Course* newCourse = new Course(name, teacherName);
            newCourse->courseId = id; 
            courses.push_back(newCourse);

        }
    }

   vector<Course*> get_course_from_teacher(string teacherName) {

         vector<Course*> teacherCourses;
        for (int i=0;i<courses.size();i++) {
            if (courses[i]->teacherName == teacherName)
            {
                teacherCourses.push_back(courses[i]);
            }
        }
        return teacherCourses;
}

Classroom(string name) : classroomName(name) {};

     Classroom(){};

    void addCourse(Course* course) 
    {
        courses.push_back(course);
        cout << "Course " << course->courseName << " added to " << classroomName << endl;
    }

    void displayCourses() 
    {
    cout << "Courses in " << classroomName << ":\n";
    cout << left << setw(30) << "COURSE NAME" << setw(30) << "COURSE ID" <<endl; // Header
    cout << string(40, '-') << endl; // Divider

    for (int i = 0; i < courses.size(); ++i) { 
        cout << left << setw(30) << courses[i]->courseName << setw(30) << courses[i]->courseId<<endl; // Display each course
    }
}

Course* getCoursesById(int courseId) 
{
    for (int i = 0; i < courses.size(); ++i) {
        if (courses[i]->courseId == courseId) {
            return courses[i]; 
        }
    }
    return nullptr; 
};
bool validateCourse(string courseName)
{
    bool flag=1;
    for(int i=0;i<courses.size();i++)
    {
        if(courses[i]->courseName==courseName)
        {
            flag=0;

        }
    }
    return flag;
}

};


void Student::joinCourse(Classroom& classroom)
{
      int courseId;
     cout << "Enter the course you want to join: ";
      cin>>courseId;
    Course* selectedCourse = classroom.getCoursesById(courseId);
    if (selectedCourse) 
    {
        selectedCourse->registerStudent(this);
        cout << "You have successfully joined the course: " << selectedCourse->courseName << endl;
        ofstream outfile("database/enrollments.csv ", ios::app);
        outfile << this->studentId << "," << selectedCourse->courseId << "," << selectedCourse->courseName << "\n";
        outfile.close();   
}
};


Student* authenticateStudent(const string& email, const string& password, const string& role) 
{
    try
    {
        string filename = "./database/" + role + "s.csv";
        ifstream infile(filename);
        if (!infile)
        {
            throw string("File can't be opened");
        }

        string line;
        while (getline(infile, line)) 
        {
            stringstream ss(line);
            string fullname, userEmail, userPassword;
            int studentId = -1;

            getline(ss, fullname, ',');
            getline(ss, userEmail, ',');
            getline(ss, userPassword, ',');

            if (role == "student") 
            {
                ss >> studentId; 
            }
            if (userEmail == email && userPassword == password) 
            {
                 if (role == "student") 
                {
                    return new Student(fullname, userEmail, userPassword, studentId);
                }
            }
        }
        return nullptr;
    }
    catch (const string& e) 
    {
        cout << e << endl;
        return nullptr;
    }
}



Teacher* authenticateTeacher(const string& email, const string& password, const string& role) 
{  
    try
    {
        string filename = "./database/" + role + "s.csv";
        ifstream infile(filename);
        if (!infile)
        {
            throw string("File can't be opened");
        }

        string line;
        while (getline(infile, line)) 
        {
            stringstream ss(line);
            string fullname, userEmail, userPassword;
            int studentId = -1;

            getline(ss, fullname, ',');
            getline(ss, userEmail, ',');
            getline(ss, userPassword, ',');

        
            if (userEmail == email && userPassword == password) 
            {
                if (role == "teacher") 
                {        return new Teacher(fullname, userEmail, userPassword); } 
            }
        }
        return nullptr;
    }
    catch (const string& e) 
    {
        cout << e << endl;
        return nullptr;
    }
}


bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(const string& date) {
   
    if (date.length() != 10 || date[2] != '-' || date[5] != '-') {
        return false; 
    }

    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    
    if (month < 1 || month > 12) {
        return false;
    }
    int daysInMonth;
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            daysInMonth = 31;
            break;
        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;
        case 2:
            daysInMonth = isLeapYear(year) ? 29 : 28;
            break;
        default:
            return false;
    }

    if (day < 1 || day > daysInMonth) {
        return false; 
    }
    return true;
};



void registerUser(const string& fullname, const string& email, const string& password, const string& role) 
{
     clearScreen();
    string filename = "./database/" + role + "s.csv";
    ofstream outfile(filename, ios::app);

    if (role == "student") 
    {
        int newStudentId = rand() % 10000 + 1; 
        outfile << fullname << "," << email << "," << password << "," << newStudentId << endl;
        cout << "Registered new student: " << fullname << " with ID: " << newStudentId << endl;
    } 
    else if (role == "teacher") 
    {
        outfile << fullname << "," << email << "," << password << endl;
        cout << "Registered new teacher: " << fullname << endl;
    }

    outfile.close();
}




void Course::courseOptions(Teacher *teacher)
{
     while (true) 
      {
                int subOption;
                cout << "1. Add Assignment\n2. View Submissions\n3. Grade Assignment\n4. Announce\n5. Go Back" << endl;
                cout << "Choose an option: " ;
                cin >> subOption;

                if (subOption == 5) break; 

                if (subOption == 1) 
                {
                    string title, description;
                    string date;
                    cout << "Enter Assignment Title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter Assignment Description: ";
                    getline(cin, description);
                    cout << "Enter the due date to submit (DD-MM-YYYY): ";
                    getline(cin,date);
                    try {
                        if (isValidDate(date)) {
                            cout << "The date is valid." << endl;
                        }
                    } catch (const invalid_argument& e) {
                        cout << "Error: " << e.what() << endl;
                    } 

                    int id=rand() % 10000 + 5;
                    Assignment* newAssignment = new Assignment( id, title, description,date,this->courseId);
                    this->addAssignment(newAssignment);
                    cout << "*****----- Assignment added successfully! -----******" << endl;

                }

                else if (subOption == 2) 
                {
                   teacher->viewSubmissionsByCourseID(this->courseId);
                    
                } 
                else if (subOption == 3) 
                {
                    int assignmentId;
             cout << "Enter Assignment ID to grade: ";
             cin >> assignmentId;

    ifstream file("database/submissions.csv");
    if (!file.is_open()) {
        cout << "No submissions found..." << endl;
        return;
    }

    string line;
    cout << "\nSubmissions for Assignment ID " << assignmentId << ":\n\n";
    cout << left << setw(15) << "Submission ID" 
         << setw(15) << "Student ID" 
         << setw(20) << "Submission Date" 
         << setw(15) << "Grade" 
         << setw(20) << "File Path" << endl;
    cout << string(100, '-') << endl;

    while (getline(file, line)) {
        istringstream ss(line);
        int submissionID, studentID, submissionAssignmentID;
        string submissionDate, grade, feedback, filePath;
        ss >> submissionID; 
        ss.ignore(); 
        ss >> submissionAssignmentID; 
        ss.ignore(); 
        ss >> studentID; 
        ss.ignore();
        getline(ss, submissionDate, ',');
        getline(ss, grade, ',');
        getline(ss, feedback, ',');
        getline(ss, filePath); 
        if (submissionAssignmentID == assignmentId) {
            cout << left << setw(15) << submissionID 
                 << setw(15) << studentID 
                 << setw(20) << submissionDate 
                 << setw(15) << grade 
                 << setw(20) << filePath << endl;
            cout << "Enter Grade for Submission ID " << submissionID << ": ";
            string newGrade;
            cin >> newGrade;
            teacher->gradeAssignment(submissionID, assignmentId, studentID, newGrade);
        }
    }

    file.close();
                }
                else if (subOption == 4)
                {
                    int id = rand() % 1000 + 5; 
                    string title;
                    string path;
                    cout << "Enter the title ..." <<endl;
                    cin>>title;
                    cout << "Input the file path to upload notes..." << endl;
                    cin>>path;
                    Announcement *announce=new Announcement(id,courseId,title,path);
                    announce->saveToCSV();

                }
                else 
                {
                    cout << "Invalid option! Please choose a valid option." << endl;
                }
            } 
        }
      
void Teacher::teacherOptions( Classroom& classroom) 
{
     if(flag)
    {
         classroom.load_courses_data("./database/courses.csv");
         flag=0;
    }
    vector<Course*>teacher_courses=classroom.get_course_from_teacher(this->fullname);

    cout << "************ Welcome educator " + this->fullname + " ***************\n";
    cout << "\n-------SAY US WHAT DO YOU WANT TO DO TODAY? -----------\n\n";
    while (true) 
    {
        cout << "\n1. Create a New Course📋" << endl;
        cout << "2. See my courses" << endl;
        cout << "3. Select one of your courses" << endl;
        cout << "4. View profile🧾" << endl;
        cout << "5. Log out🚫 " << endl;
        cout << "Choose an option: ";
        
        int option;
        cin >> option;

        if (option == 5) break;

        if (option == 4)
        {
            this->my_account();
        }
        if (option == 1) 
        {
            string courseName;
            cout << "Enter Course Name: "; 
            cin.ignore(); 
            getline(cin, courseName);
            Course* newCourse = new Course(courseName,this->fullname);
            try
            {
                if(classroom.validateCourse(courseName))
                {
                      
                     classroom.addCourse(newCourse);

                     this->mycourses.push_back(newCourse->courseName);

                } else {
                    throw string("\n\n--------COURSE ALREADY EXISTS----------\n\n");
                }
            }
            catch(const string str)
            {
                cout << str << endl;
                break;
            }
            
           

            cout << "********* The course " << newCourse->courseName << " is created and ID is " << newCourse->courseId << " ***********"<< endl;

            ofstream outfile("./database/courses.csv", ios::app);
            outfile << newCourse->courseId << "," << newCourse->courseName << "," << this->fullname << endl;
            outfile.close();
            teacher_courses.push_back(newCourse);

        } 
        else if (option == 2) 
        {
            cout << "********* YOUR COURSES *********\n";
         
         cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;

          for(int i=0;i<teacher_courses.size();i++)
            {
                cout << i+1 << ". "<<left << setw(15) << "Course Name : " << setw(30) << teacher_courses[i]->courseName << endl; 
            } 
              cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
               cout << "\n\n";
        } 

        else if (option == 3) 
        {
             int cNum;
            cout << "********* YOUR COURSES **********\n\n" << endl;
             cout << "SELECT ANY ONE OF THEM ?" << endl;
             cout << " Course-ID ------------------------------ COURSE-NAME\n\n";
              for(int i=0;i<teacher_courses.size();i++)
            {
                cout << teacher_courses[i]->courseId << " >> "<< teacher_courses[i]->courseName << endl;
            } 
            cout << "Please enter the course ID to select: ";
            cin>>cNum;

            if(cNum<1 )
            {
                cout << "Invalid number. please select an appropriate course...\n\n";
                cout << "-----------------------------------------------------------------\n";
                continue;
            }

            Course* selectedCourse=classroom.getCoursesById(cNum);
            selectedCourse->courseOptions(this);
        } }
};



void Student::studentOptions(Classroom& classroom) 
{
    if(flag)
    {
         classroom.load_courses_data("./database/courses.csv");
         flag=0;
    }
    clearScreen();

    cout << "********* HI "+ this->fullname +" ************\n";

      vector<string>enrolledCourses=this->viewCourses();
    int option;
    while (true) 
    {
       
        cout << "\n---- Student Options ---- " << endl;
        cout << "1. Show Enrolled Courses📝" << endl; 
        cout << "2. See Assignments📑" << endl;       
        cout << "3. Submit Assignment" << endl;   
        cout << "4. Join a Course " << endl;     
        cout << "5. Show my account details🧾 " << endl; 
        cout << "6. Show the announcements " << endl;
        cout << "7. Go to main menu📃 " << endl;

        cout << "Choose an option:📋 ";  

        cin >> option;
        if (option == 7)
        {
            break;

        } else
        if(option == 5)
        {
            clearScreen();
            this->my_account();
        }
         else
        if (option == 1) 
        {
            clearScreen();
            cout << "------ENROLLED COURSES--------\n"; 
          for(int i=0;i<enrolledCourses.size();i++)
          {
            cout <<"        " <<  i+1 << " << " <<  enrolledCourses[i] << endl;
          }
        } 

        else if (option == 2) 
{          string courseName;
           cout << "Enter the course name to see assignments: ";
           cin.ignore();             
           getline(cin, courseName); 
            this->displayAssignmentsForCourse(courseName);
}
        else if (option == 3) 
        {
            this->submitAssignment();
  
  
} else if (option == 4)
{
    classroom.displayCourses();
    this->joinCourse(classroom);
} else if (option == 6)
{
    int id;
     classroom.displayCourses();
    cout << "Enter the course-ID to see the announcements: ";
    cin>>id;
    Announcement::displayAnnouncementsForCourse(id);

}
};
};

void displayMainMenu() 
{
    cout << "********* 🏛️ Welcome to our Google Classroom 🏛️ **********";
    cout << "\n------ Main Menu ------" << endl;
    cout << "1. Administrator Login 📝" << endl;
    cout << "2. Student Login 📖" << endl;
    cout << "3. Register 📃" << endl;
    cout << "4. Exit 🚫" << endl;
    cout << "5. Acknowledgement📋" <<endl;
    cout << "Choose an option: ";
};

void handleTeacherLogin(Classroom& classroom) 
{

     clearScreen();
     cout << "************** LOGIN AS A TEACHER ****************\n";
    string email, password;
    cout << "Enter teacher email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;
    Teacher* teacher = authenticateTeacher(email, password, "teacher");

    if (teacher!= nullptr) 
    {
        teacher->teacherOptions(classroom);
    } else 
    {
        cout << "Invalid teacher credentials." << endl;
    }
};

void handleStudentLogin(Classroom& classroom) 
{
     clearScreen();
      cout << "************** LOGIN AS A STUDENT ****************\n";
    string email, password;
    cout << "Enter student email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;

    Student* student = authenticateStudent(email, password, "student");
    
    if (student != nullptr) 
    {
        student->studentOptions(classroom);
    } 
    else
    {
        cout << "Invalid student credentials." << endl;
    }
}

void handleRegistration(Classroom& classroom) 
{
    string fullname, email, password, role;
    cout << "Enter full name: ";
    cin.ignore();
    getline(cin, fullname);
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;
    cout << "Register as (teacher/student): ";
    cin >> role;

try           // ...... Exception handling .....
{
    if (isValidEmail(email)) 
    {
        registerUser(fullname, email, password, role);
        cout << "Registration successful." << endl;

        if (role == "teacher")
         {
            handleTeacherLogin(classroom); 

        } else if (role == "student") {
            handleStudentLogin(classroom); 
        }
    } else {
        throw "--------------Invalid email format--------------";
        }
}
catch(string s)
{
    cout << s << endl;
}  
}




int main() {
     clearScreen();
    Classroom classroom("CS101");
   
    int choice;

    while (true) {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                handleTeacherLogin(classroom);
                break;
            case 2:
                handleStudentLogin(classroom);
                break;
            case 3:
                handleRegistration(classroom);
                break;
            case 4:
                cout << "Exiting....." << endl;
                return 0;
             case 5:
             cout<< "\n----- We the team who made this model of 'Google Classroom' would like to convey special thanks to Mrs. Vrinda Yadav mam, Faculty of Introduction to Object Oriented Programming, for teaching us all the necessary oops concepts to make this project ---------\n\n";
             break;
            default:
                cout << "Invalid option. Please try again." << endl;
        
        }}};