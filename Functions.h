#ifndef functions_h
#define functions_h

#include <pgsql/libpq-fe.h>
#include <stdio.h> 

PGconn *conn;//global connection variable

//function prototypes
int connection(void);
void disconnect(void);
void add_student(void);
void delete_student(void);
void list_student(void);
void modify_student(void);
void add_course(void);
void modify_course(void);
void list_course(void);
void delete_course(void);
void search_students(void);
void give_grade(void);
void update_gpa(char []);

#endif
