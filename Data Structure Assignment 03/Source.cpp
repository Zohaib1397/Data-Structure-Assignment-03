#include<iostream>
#include<string>
using namespace std;
class Student {
public:
	string name;
	string regNo;
	float cGPA;
	string department;
	string program;
	Student* next;
	Student* previous;
	//Insertion Operator Overloading to get the values from user
	friend istream& operator>>(istream& in, Student& obj) {
		cout << "Enter Student Name: ";
		getline(in, obj.name);
		cout << "Enter Reg.No.: ";
		getline(in, obj.regNo);
		cout << "Enter CGPA: ";
		in >> obj.cGPA;
		in.ignore();
		cout << "Enter Department Name: ";
		getline(in, obj.department);
		cout << "Enter Program: ";
		getline(in, obj.program);
		return in;
	}
	//Extertion Operator Overloading to console out Student Details
	friend ostream& operator<<(ostream& out, Student& obj) {
		out << "\nName: " << obj.name << "\n";
		out << "RegNo: " << obj.regNo << "\n";
		out << "CGPA: " << obj.cGPA << "\n";
		out << "Department: " << obj.department << "\n";
		out << "Program: " << obj.program << "\n";
		return out;
	}
};
class LinkedList {
	Student* head;
public:
	LinkedList() :head(NULL) {}
	bool isHeadNULL() {
		return head == NULL ? true : false;
	}
	//Main function passing head NULL to this function 
	void insertIfNULL(Student* obj) {
		if (head == NULL) head = obj;
	}
	void insertStudent(string name, Student* obj) {
		//No need to check for (head == NULL) because the case is handled in main function
		int countSameStudent = 0;
		Student* current = head;
		Student* temporaryStore = head;
		for (current; current != NULL; current = current->next) {
			if (current->name == name) {
				countSameStudent++;
				temporaryStore = current;
			}
		}
		//Check if the Same RegNo already exists
		if (checkRegNo(obj->regNo)) {
			cout << "\033[1m\033[31mError!!\033[0m regNo already exists Cannot enter Student\n";
			return;
		}
		/*Now if the student name appears more than once the user will get a warning
		indicating that the user Already exists more than once.
		He/She will be asked to enter RegNo. instead*/
		if (countSameStudent > 1) {
			printf("\033[1m\033[31mAttention!!\033[0m the Student Named: %s already exists %i times\n", name.c_str(), countSameStudent);
			printf("Please Enter Student RegNo. instead: ");
			getline(cin, name);
			//Reediting count to zero in case if student does not exists
			countSameStudent = 0;
			for (temporaryStore = head; temporaryStore != NULL; temporaryStore = temporaryStore->next) {
				if (temporaryStore->regNo == name) {
					countSameStudent++;
					break;
				}
			}
		}
		if (countSameStudent != 0) {
			//if the name/reg is the last node
			if (temporaryStore->next == NULL) {
				temporaryStore->next = obj;
				obj->previous = temporaryStore;
			}
			else {
				obj->previous = temporaryStore;
				obj->next = temporaryStore->next;
				temporaryStore->next->previous = obj;
				temporaryStore->next = obj;
			}
		}
		else cout << "Student Named: \033[1m\033[31m" << name << "\033[0m Not Found!!\n";
	}
	bool checkRegNo(string regNo) {
		Student* current = head;
		for (current; current != NULL; current = current->next) if (current->regNo == regNo) return true;
		return false;
	}
	void deleteStudent(string regNo) {
		if (head == NULL) return;
		Student* current = head;
		for (current; current != NULL; current = current->next) if (current->regNo == regNo) break;
		if (current == head) cout << "\033[1m\033[31mError!!\033[0m Cannot delete before Head Node\n";
		else if (current == head->next) {
			Student* temp = head;
			head = head->next;
			current->previous = NULL;
			delete temp;
		}
		else {
			Student* temp = current->previous;
			current->previous->previous->next = current;
			current->previous = current->previous->previous;
			delete temp;
		}
	}
	Student* topper() {
		//First of All find the Maximum CGPA from list
		float max = head->cGPA;
		for (Student* current = head; current != NULL; current = current->next) if (current->cGPA > max) max = current->cGPA;
		/*Creating a second head and using it as a list of toppers
		having equal and maximum CGPA*/
		Student* second_head = NULL;
		for (Student* current = head; current != NULL; current = current->next) {
			//if cGPA is equal to Max then create head or insert at end
			if (current->cGPA == max) {
				//Copying the data from current to a temporary variable
				Student* temp = new Student;
				temp->name = current->name;
				temp->regNo = current->regNo;
				temp->cGPA = current->cGPA;
				temp->department = current->department;
				temp->program = current->program;
				temp->next = temp->previous = NULL;
				//same as insertion method 
				//if head is NULL then create a head node
				if (second_head == NULL) second_head = temp;
				//and if there is already a student having max number then insert second student into the list
				else {
					Student* end = second_head;
					for (end; end->next != NULL; end = end->next) {}
					end->next = temp;
					temp->previous = end;
				}
			}
		}
		//finally returning the second head consisting of the list of toppers 
		return second_head;
	}
	void showName_Reg() {
		cout << "\nStudents Name and Reg.No. in the List\n";
		for (Student* current = head; current != NULL; current = current->next) cout << "\033[1m\033[32m   " << current->name << " " << current->regNo << "   \033[0m|";
		cout << "\n";
	}
	void display() {
		for (Student* current = head; current != NULL; current = current->next)	cout << *current;
	}
	void display(Student* obj) {
		for (Student* current = obj; current != NULL; current = current->next) cout << *current;
	}
};
int main() {
	bool isRunning = true;
	Student* student;
	LinkedList* obj = new LinkedList;
	string name;
	do {
		bool user_choice;
		do {
			user_choice = true;
			cout << "\nPress [I] to Insert Student\n";
			cout << "Press [R] to Delete With RegNo\n";
			cout << "Press [T] to Get Topper\n";
			cout << "Press [D] to Display\n";
			cout << "Press [X] to Exit\n";
			cout << "\nChoose your option: ";
			char choice;
			cin >> choice;
			cin.ignore();
			choice = tolower(choice);
			switch (choice) {
			case 'i':
				if (obj->isHeadNULL()) {
					cout << "\033[1m\033[32mThis is first node\033[0m So just enter Student Details\n";
					student = new Student;
					student->next = student->previous = NULL;
					cin >> *student;
					obj->insertIfNULL(student);
				}
				else {
					obj->showName_Reg();
					cout << "Enter Student Name After Which you want to insert New Student: ";
					getline(cin, name);
					student = new Student;
					student->next = student->previous = NULL;
					cin >> *student;
					obj->insertStudent(name, student);
				}
				break;
			case 'r':
				if (obj->isHeadNULL()) cout << "List is Empty\n";
				else {
					obj->showName_Reg();
					cout << "Enter RegNo of the Student: ";
					//treating name as regNo here
					getline(cin, name);
					//deleting student before the 
					obj->deleteStudent(name);
				}
				break;
			case 'd':obj->display();
				break;
			case 't':
				student = obj->topper();
				cout << "\nTopper(s) in the list\n";
				obj->display(student);
				break;
			case 'x':isRunning = false;
				break;
			default:
				cout << "Invalid Choice\n";
				user_choice = false;
			}
		} while (user_choice == false);
	} while (isRunning);
}