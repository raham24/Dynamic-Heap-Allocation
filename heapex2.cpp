/*  CSC 16 Lab 9 (memory leaks lab) part II

  The following program will compile and run without errors ... until your
computer runs out of memory.  Once again there are plenty of places where
memory is leaked.  Study the program first to understand how it works (how
it uses heap-allocated memory).  Then fix the leaks.

You will need to:

1. finish writing the "destructor" of the class student so that when a 
student is deleted, all the courses that student's taking are also deleted.

2. call delete where appropriate in the program: once again, your deletes
cannot introduce new memory errors into the program.  You will need to
delete more than just the two students at the end of function f, but also
examine closely the function 'drop_course' to see if there are any other
places where leaks can occur.  The most common cause of leaks is reassigning
a pointer away from allocated memory without deleting it first.

As before, you need to make sure that all the code in the program 
is still being executed.  You may only add new code.

Use the task manager or similar program to confirm that there're no more leaks.
*/

#include<iostream>
#include<string>
#include<cstdlib>
using namespace std;

struct course {
  string name;
  int credits;
  bool required;
  course(string n, bool r=1) {  // constructor (with optional arg r)
    name = n;  required = r; credits = 3;
  }//constructor
};//struct course


struct student {
  string name;
  int id;
  string major;
  course* courses[8]; // array of up to 8 pointers to courses
  student(string n, int id, string m) {
    name = n;  major = m;  this->id=id;
    for(int i=0;i<8;i++) courses[i]=nullptr; // no courses yet
  }//constructor

  bool add_course(course* c) {  // returns true if course was added
    int i = 0;
    while (i<8) {
      if (courses[i]==nullptr) {courses[i]=c;  return 1;}
      i++;
    }
    return 0;
  }//add_course

  int num_credits() { // returns total number of credits student is taking
    int total = 0;
    for(int i=0;i<9;i++) {
      if (courses[i]!=nullptr) total += courses[i]->credits;
    }
    return total;
  }

  // drop a class by name, returns true if drop was successful
  bool drop_course(string class_name) { 
    for(int i=0;i<9;i++) {
      if (courses[i]!=nullptr && courses[i]->name==class_name) {
	//cout << name << " is dropping "<<class_name<<endl;
	courses[i] = nullptr;
	return 1;
      }
    }//for i
    return 0; // did not find course to drop
  }//drop_course


  ~student() {   // destructor should delete all courses taken
    int i = 0;
    while (i<8){
      if (courses[i] != nullptr){
        delete courses[i];
        courses[i]=nullptr;
      }
    }
    // what to call delete on?  try and see
    
  }//destructor

}; // struct student


void f(int cx) {
  student *student0 = new student("Stu Dious",cx, "computer science");
  student *student1 = new student("Nev Erstudy",cx+1,"basketry");
  student0->add_course(new course("phys11"));
  student0->add_course(new course("csc16"));
  student0->add_course(new course("csc24"));
  student0->add_course(new course("baskets101",0)); // not required for cs
  student0->add_course(new course("math71"));
  student1->add_course(new course("baskets101"));
  student1->add_course(new course("baskets102"));  
  student1->add_course(new course("csc15",0));
  student0->drop_course("baskets101");
  student0->drop_course("phys11");  
  student1->drop_course("csc15");
  delete(student0);
  delete(student1); // these deletes won't be enough
}// function f

int main()
{ 
  int cx = 0;  // ID counter
  while (1) {
    f(cx); cx+=2;
  }
  return 0;  
}// main

