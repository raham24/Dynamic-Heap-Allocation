/*  CSC 16 Lab 9 (memory leaks lab) part III

In this version of the "students" program, instead of having a fixed array
of 8 courses in each student object, there's now a "linked list" of courses:
each course object has a pointer to another course.  

Your task is the same as before: understand the program and get rid of
the memory leaks while preserving the existing code and not introducing
any new errors.
*/

#include<iostream>
#include<string>
#include<cstdlib>
using namespace std;

struct course {
  string name;
  int credits;
  bool required;
  course* next_course; // creates "linked list" of courses
  course(string n, bool r=1) {  // constructor (with optional arg r)
    name = n;  required = r; credits = 3;
    next_course = nullptr;
  }//constructor

  // is this course linked to another course?
  bool has_next_course() { return next_course!=nullptr; }

  void link_course(course* c) {
    this->next_course = c;
  }
};//struct course


struct student {
  string name;
  int id;
  string major;
  course* courses; // courses student is taking (points to first course)
  student(string n, int id, string m) {
    name = n;  major = m;  this->id=id;
    courses = nullptr; // initially no courses
  }//constructor

  void add_course(course* c) {
    c->next_course=nullptr; // make sure this is the only class added
    if (courses == nullptr) { courses=c; return; } //first course added
    course *cptr = courses; // course pointer
    while (cptr->has_next_course()) { cptr = cptr->next_course; }
    // at this point, cptr is pointing to last course in list of courses
    cptr->next_course = c; // adds course to end of list
  }//add_course

  int num_credits() { // returns total number of credits student is taking
    int total = 0;
    course* cptr = courses;
    while (cptr!=nullptr) {
      total += cptr->credits;
      cptr = cptr->next_course;
    }
    return total;
  }

  void drop_course(string class_name) { // drop a class by name
    if (courses==nullptr) return; // nothing to drop
    if (courses->name == class_name) { // drop first course:
      courses = courses->next_course;
      return;
    }
    // else, course to drop is somewhere in middle of list
    course* current = courses;
    course* next = courses->next_course;
    while (next!=nullptr && next->name!=class_name) {
      current = next;  // advance both pointers forward
      next = next->next_course;
    }
    // at this point, next is either pointing to course to drop, or is nullptr
    if (next!=nullptr) {  // set next_couse to skip course to be deleted
      current->next_course = next->next_course;
      //cout << name << " dropped class: " << class_name << endl;

    }
  }//drop_course


  ~student() {   // destructor should delete all courses taken

    // while (courses){
    //   if (courses->has_next_course() != nullptr){
    //     delete temp;
    //   }
    // }

    course* current = courses;
    while (current) {
      course* temp = current;
      if (current->has_next_course()){
        current = current->next_course;
        delete temp;
      }
    }
    // what to call delete on?
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

