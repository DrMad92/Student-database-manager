#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Functions.h"

void give_grade()
{
	PGresult *res, *res2;
	char searchstudent[7];
	char searchcourse[7];
	char query[200] = "SELECT kkh FROM opilane WHERE tudengikood = ";
	char query2[200] = "SELECT * FROM opilane WHERE tudengikood = ";
	char query3[200] = "SELECT * FROM aine WHERE id_aine = ";
	char query6[200] = "SELECT hinne FROM hinne WHERE tudengikood = ";
	char query5[200] = "INSERT INTO hinne (id_hinne,id_aine,tudengikood,hinne) VALUES (";
	char query7[200];
	int check;
	char Grade[2];
	int grade;
	int latestID, courseID;
	char newID[7];
    int row, row2;
    int col;
    int rec_count, rec_count2;
	
	list_student();//list all students
	printf("Select student (input ID): ");
	scanf("%s", searchstudent);
	
	strcat(query2, searchstudent);
	strcat(query2, ";");//query for searching student by studentcode
	strcat(query6, searchstudent);
	strcat(query6, " AND id_aine = ");//part of query to find grades of student
	
	res = PQexec(conn, query2);
    //Check query SELECT result status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("We did not get any data!\n");
        PQclear(res);
        return;
    }
    check = PQntuples(res);
    if(check == 0)//if 0 student record doesnt exist
    {
		printf("Student not found!");
		PQclear(res);
		return;
	}
	else//else list student
	{
		printf("\n");
		for (row=0; row<check; row++) 
		{
			for (col=0; col<4; col++) 
			{
				printf("%-10s\t", PQgetvalue(res, row, col));
			}
		}
		PQclear(res);
		
		//Execute query for listing all courses
		res = PQexec(conn, "SELECT * FROM aine;");
		
		//Check query result status
		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			printf("\nWe did not get any data!\n");
			return;
		}
		
		//records count
		rec_count = PQntuples(res);

		printf("\n============================================================\n");
		printf("%s|%s|%s|\t%s\t\t|\t%s\n", "ID", "EAP", "Course code", "Name", "Grades");
		printf("------------------------------------------------------------\n");
		
		//Print received records
		for (row=0; row<rec_count; row++) 
		{	//listing courses
			printf("%-2s", PQgetvalue(res, row, 0));
			printf("%3s", PQgetvalue(res, row, 1));
			printf("%10s\t  ", PQgetvalue(res, row, 2));
			printf("%-24s\t", PQgetvalue(res, row, 3));
			
			strcpy(query7, query6);
			strcat(query7, PQgetvalue(res, row, 0));
			strcat(query7, ";");
			//Execute query to list grades of specific courses of a student
			res2 = PQexec(conn, query7);
			//Check query result status
			if (PQresultStatus(res2) != PGRES_TUPLES_OK) {
				printf("\nWe did not get any data!\n");
				return;
			}
		
			//records count
			rec_count2 = PQntuples(res2);
			if(rec_count2 == 0)//if 0 student doent have grade on particular course
			{
				printf("%10s","No grade");
			}
			else//else list all grades on particular course
			{
				for(row2=0; row2<rec_count2; row2++)
				{
					printf("%3s", PQgetvalue(res2, row2, 0) );
				}
			}
			query7[0] = '\0';//terminating query string to reconstruct for different course
			printf("\n");
			PQclear(res2);
		}
		
		printf("============================================================\n");
		
		strcat(query, searchstudent);
		strcat(query, ";");
		//Execute query to show current GPA of a student
		res2 = PQexec(conn, query);
		//Check query result status
		if (PQresultStatus(res2) != PGRES_TUPLES_OK) {
			printf("\nWe did not get any data!\n");
			return;
		}
		printf("\t\t\t\t\t\tGPA - %s", PQgetvalue(res2, 0, 0));
		//Free PGresult handle 
		PQclear(res);
		PQclear(res2);
		printf("\nSelect course (input ID): ");
		scanf("%s", searchcourse);
		
		strcat(query3, searchcourse);
		strcat(query3, ";");//query to add grade on particular course
		
		res = PQexec(conn, query3);
		//Check query SELECT result status
		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			printf("We did not get any data!\n");
			PQclear(res);
			return;
		}
		check = PQntuples(res);
		if(check == 0)//if wrong ID course then return to main
		{
			printf("Course not found!");
			PQclear(res);
			return;
		}
		else
		{
			do
			{
				printf("Enter grade: ");
				scanf("%d", &grade);
			}while(grade < 0 || grade > 5);//checking input
			
			sprintf(Grade, "%d", grade);
			
			PQclear(res);
			res = PQexec(conn, "SELECT id_hinne FROM hinne ORDER BY id_hinne DESC;");
			//Check query SELECT result status
			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				printf("We did not get any data!\n");
				PQclear(res);
				return;
			}
			//id for grade
			latestID = atoi(PQgetvalue(res, 0, 0));
			sprintf(newID, "%d", latestID+1);
			//constructing query
			strcat(query5, newID);				
			strcat(query5, ", ");				
			strcat(query5, searchcourse);				
			strcat(query5, ", ");				
			strcat(query5, searchstudent);				
			strcat(query5, ", ");
			strcat(query5, Grade);
			strcat(query5, ");");
			
			PQclear(res);
			//query for adding grade
			res = PQexec(conn, query5);
			//Check query INSERT result status
			if (PQresultStatus(res) != PGRES_COMMAND_OK) 
			{
				printf("We couldn't write data!\n");
				PQclear(res);
				return;
			}else 
			{
				printf("Operation success!\n");
				PQclear(res);
				update_gpa(searchstudent);//after successful adding grade, updating GPA
				return;
			}
							
		}
	}
}
