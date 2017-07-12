#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Functions.h"

void add_student()
{
	PGresult *res;
	
	char fname[21];
	char lname[21];
	char birth[21], Month[4], Day[4];
	int month, day;
	char query[200] = "INSERT INTO opilane (tudengikood,eesnimi,perekonnanimi,synnikp) VALUES (";
	int latestID;
	char newID[7];
		
	printf("Enter student's first name: ");
	scanf("%s", fname);
	printf("Enter student's last name: ");
	scanf("%s", lname);
	printf("Enter student's birth year: ");
	scanf("%s", birth);
	do
	{
		printf("Enter student's birth month: ");
		scanf("%d", &month);
	}while(month <= 0 || month >12);//checking correctness of input
	do
	{
		printf("Enter student's birth day: ");
		scanf("%d", &day);
	}while(day<=0 || day>31);//checking correctness of input
	sprintf(Month, "-%d", month);//writing to string
	sprintf(Day, "-%d", day);//writing to string
	strcat(birth, Month);
	strcat(birth, Day);//latest version of 'date' is "yyyy-m-d" to correctly read by database
	
	
	res = PQexec(conn, "SELECT tudengikood FROM opilane ORDER BY tudengikood DESC;");
	
	//Check query SELECT result status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("We did not get any data!\n");
        PQclear(res);
        return;
    }
    //creating unique student code by incrementing latest code by 1
    latestID = atoi(PQgetvalue(res, 0, 0));
    sprintf(newID, "%d", latestID+1);
    
    //constructing query
	strcat(query, newID);
	strcat(query, ", '");
	strcat(query, fname);
	strcat(query, "', '");
	strcat(query, lname);
	strcat(query, "', '");
	strcat(query, birth);
	strcat(query, "');");
	
	PQclear(res);
	res = PQexec(conn, query);
	//Check query INSERT result status
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("We couldn't write data!\n");
        PQclear(res);
        return;
    }else 
    {
		printf("Operation success!\n");
		PQclear(res);
	}
	
	return;
}

void delete_student()
{
	PGresult *res;
    int row;
    int col;
    char search[7];
    char query[200] =  "DELETE FROM opilane WHERE tudengikood = ";
    char query2[200] = "SELECT * FROM opilane WHERE tudengikood = ";
    int check;
    char answer;
    
    list_student();// list all students
       
    printf("Enter student ID to delete: ");
    scanf("%s", search);
    strcat(query, search);
    strcat(query, ";");//query for deleting student
    strcat(query2, search);
    strcat(query2, ";");//query for searching student whether record exists
    
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
		for (row=0; row<check; row++) 
		{
			for (col=0; col<4; col++) 
			{
				printf("%-10s\t", PQgetvalue(res, row, col));
			}
		}
		do
		{
			printf("\nAre you sure?(Y/N)");//reask user about deletion
			scanf (" %c", &answer);
		}while(answer != 'N' && answer != 'n' && answer != 'Y' && answer != 'y');
		
		if (answer == 'N' || answer == 'n')
		{
			return;
		}
	}
    
    res = PQexec(conn, query);
    //Check query  DELETE result status
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Data is not deleted!\n");
        PQclear(res);
        return;
    }else 
    {
		printf("Operation success!\n");
	}
    
    PQclear(res);
    
    return;
}

void list_student()
{
	PGresult *res;
	int rec_count;
    int row;
    int col;
    
    //Execute query
    res = PQexec(conn, "SELECT * FROM opilane;");
    
    //Check query result status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("We did not get any data!\n");
        return;
    }
    
    //records count
    rec_count = PQntuples(res);
    
    printf("\nWe received %d records.\n", rec_count);

    printf("====================================================================\n");
    printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n", "Student code", "Name", "Surname", "Date of birth", "GPA");
    printf("--------------------------------------------------------------------\n");
    
    //Print received records
    for (row=0; row<rec_count; row++) {
        for (col=0; col<5; col++) {
            printf("%-10s\t", PQgetvalue(res, row, col));
        }
        printf("\n"); 
    }
    
    printf("=====================================================================\n");
    
    //Free PGresult handle 
    PQclear(res);
    return;
}

void search_students()
{
	PGresult *res;
	int rec_count;
    int row;
    int col;
    char search[7];
    int answer;
    char query[200] = "SELECT * FROM opilane WHERE kkh ";
    
    printf("\n1.List average bigger than\n2.List average less than\nSelect option: ");
    scanf("%d", &answer);
    switch (answer)
    {
		case 1:
			printf("Input average: ");
			scanf("%s", search);
			
			strcat(query, "> ");
			strcat(query, search);
			strcat(query, ";");
			
			res = PQexec(conn, query);
    
			//Check query result status
			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				printf("We did not get any data!\n");
				return;
			}
			
			//records count
			rec_count = PQntuples(res);
			
			printf("We received %d records.\n", rec_count);

			printf("====================================================================\n");
			printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n", "Student code", "Name", "Surname", "Date of birth", "GPA");
			printf("--------------------------------------------------------------------\n");
			
			//Print received records
			for (row=0; row<rec_count; row++) {
				for (col=0; col<5; col++) {
					printf("%-10s\t", PQgetvalue(res, row, col));
				}
				printf("\n"); 
			}
			
			printf("=====================================================================\n");
			
			//Free PGresult handle 
			PQclear(res);
			return;
		case 2:
			printf("Input average: ");
			scanf("%s", search);
			
			strcat(query, "< ");
			strcat(query, search);
			strcat(query, ";");
			
			res = PQexec(conn, query);
    
			//Check query result status
			if (PQresultStatus(res) != PGRES_TUPLES_OK) {
				printf("We did not get any data!\n");
				return;
			}
			
			//records count
			rec_count = PQntuples(res);
			
			printf("We received %d records.\n", rec_count);

			printf("====================================================================\n");
			printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n", "Student code", "Name", "Surname", "Date of birth", "GPA");
			printf("--------------------------------------------------------------------\n");
			
			//Print received records
			for (row=0; row<rec_count; row++) {
				for (col=0; col<5; col++) {
					printf("%-10s\t", PQgetvalue(res, row, col));
				}
				printf("\n"); 
			}
			
			printf("=====================================================================\n");
			
			//Free PGresult handle 
			PQclear(res);
			return;
	}
}

void modify_student(void)
{
	PGresult *res;
	char search[7];
	char query[200] = "UPDATE opilane SET ";
	char query2[200] = "SELECT * FROM opilane WHERE tudengikood = ";
	int answer, check;
	char fname[21], lname[21], birth[21], Month[4], Day[4];
	int month, day;
	int row, col;
	
	list_student();//list all students
	
	printf("Enter student ID to modify: ");
	scanf("%s", search);
	
	strcat(query2, search);
	strcat(query2, ";");
	
	res = PQexec(conn, query2);
    //Check query SELECT result status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("We did not get any data!\n");
        PQclear(res);
        return;
    }
    check = PQntuples(res);
    if(check == 0)
    {
		printf("Student not found!");
		PQclear(res);
		return;
	}
	else
	{
		for (row=0; row<check; row++) 
		{
			for (col=0; col<4; col++) 
			{
				printf("%-10s\t", PQgetvalue(res, row, col));
			}
		}
		printf("\n\nWhat do you want to change?\n");
		printf("1.First name\n2.Last name\n3.Date of birth\nSelect option: ");
		scanf("%d", &answer);
		
		//menu to modify different values, each one creates different query
		switch(answer)
		{
			case 1:
				printf("Enter new first name: ");
				scanf("%s", fname);
				strcat(query, "eesnimi = '");
				strcat(query, fname);
				strcat(query, "' WHERE tudengikood = ");
				strcat(query, search);
				strcat(query, ";");
				res = PQexec(conn, query);
				//Check query UPDATE result status
				if (PQresultStatus(res) != PGRES_COMMAND_OK) {
					printf("We couldn't write data!\n");
					PQclear(res);
					return;
				}else 
				{
					printf("Operation success!\n");
					PQclear(res);
					return;
				}
			case 2:
				printf("Enter new last name: ");
				scanf("%s", lname);
				
				strcat(query, "perekonnanimi = '");
				strcat(query, lname);
				strcat(query, "' WHERE tudengikood = ");
				strcat(query, search);
				strcat(query, ";");
				
				res = PQexec(conn, query);
				//Check query UPDATE result status
				if (PQresultStatus(res) != PGRES_COMMAND_OK) {
					printf("We couldn't write data!\n");
					PQclear(res);
					return;
				}else 
				{
					printf("Operation success!\n");
					PQclear(res);
					return;
				}
			case 3:
				printf("Enter new birth year: ");
				scanf("%s", birth);
				do
				{
					printf("Enter new birth month: ");
					scanf("%d", &month);
				}while(month <= 0 || month >12);
				do
				{
					printf("Enter new birth day: ");
					scanf("%d", &day);
				}while(day<=0 || day>31);
				sprintf(Month, "-%d", month);
				sprintf(Day, "-%d", day);
				strcat(birth, Month);
				strcat(birth, Day);
				
				strcat(query, "synnikp = '");
				strcat(query, birth);
				strcat(query, "' WHERE tudengikood = ");
				strcat(query, search);
				strcat(query, ";");
				
				res = PQexec(conn, query);
				//Check query UPDATE result status
				if (PQresultStatus(res) != PGRES_COMMAND_OK) {
					printf("We couldn't write data!\n");
					PQclear(res);
					return;
				}else 
				{
					printf("Operation success!\n");
					PQclear(res);
					return;
				}
		}
	}
}