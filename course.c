#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Functions.h"

void add_course()
{
	PGresult *res;
	
	char coursename[31], teachername[31], coursecode[11], eap[3];
	char query[200] = "INSERT INTO aine (id_aine,eap,ainekood,nimi,oppejoud) VALUES (";
	int latestID;
	char newID[7];
	int i;
		
	printf("Enter course name: ");
	scanf(" %[^\n]s", coursename);//scanning(including whitespace) until new line character
	printf("Enter course code: ");
	scanf(" %s", coursecode);
	printf("Enter teacher's name: ");
	scanf(" %[^\n]s", teachername);//scanning(including whitespace) until new line character
	printf("Enter credit points: ");
	scanf(" %s", eap);
	
	
	res = PQexec(conn, "SELECT id_aine FROM aine ORDER BY id_aine DESC;");
	
	//Check query SELECT result status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("We did not get any data!\n");
        PQclear(res);
        return;
    }
    //to create unique course id, function increments latest id number by 1
    latestID = atoi(PQgetvalue(res, 0, 0));
    sprintf(newID, "%d", latestID+1);
    
    //constructing query
	strcat(query, newID);
	strcat(query, ", ");
	strcat(query, eap);
	strcat(query, ", '");
	strcat(query, coursecode);
	strcat(query, "', '");
	strcat(query, coursename);
	strcat(query, "', '");
	strcat(query, teachername);
	strcat(query, "');");
	
	PQclear(res);//clearing res before next use
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

void list_course()
{
	PGresult *res;
	int rec_count;
    int row;
    
    //Execute query
    res = PQexec(conn, "SELECT * FROM aine;");
    
    //Check query result status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("\nWe did not get any data!\n");
        return;
    }
    
    //records count
    rec_count = PQntuples(res);
    
    printf("\nWe received %d records.\n", rec_count);

    printf("============================================================\n");
    printf("%s|%s|%s|\t%s\t\t|\t%s\n", "ID", "EAP", "Course code", "Name", "Teacher");
    printf("------------------------------------------------------------\n");
    
    //Print received records
    for (row=0; row<rec_count; row++) 
    {
		printf("%-2s", PQgetvalue(res, row, 0));
		printf("%3s", PQgetvalue(res, row, 1));
		printf("%10s\t  ", PQgetvalue(res, row, 2));
		printf("%-25s\t", PQgetvalue(res, row, 3));
		printf("%s", PQgetvalue(res, row, 4));
		printf("\n");
    }
    
    printf("============================================================\n");
    
    //Free PGresult handle 
    PQclear(res);
    return;
}

void modify_course(void)
{
	PGresult *res;
	char search[7];
	char query[200] = "UPDATE aine SET ";
	char query2[200] = "SELECT * FROM aine WHERE id_aine = ";
	int answer, check;
	char coursename[31], teachername[31], coursecode[11], eap[3];
	int row, col;
	
	list_course();//list all course
	
	printf("Enter course ID to modify: ");
	scanf("%s", search);
	//constructing query
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
    if(check == 0)//if 0 course record not found
    {
		printf("Course not found!");
		PQclear(res);
		return;
	}
	else//else list course
	{
		for (row=0; row<check; row++) 
		{
			printf("%-2s", PQgetvalue(res, row, 0));
			printf("%3s", PQgetvalue(res, row, 1));
			printf("%10s\t  ", PQgetvalue(res, row, 2));
			printf("%-25s\t", PQgetvalue(res, row, 3));
			printf("%s", PQgetvalue(res, row, 4));
		}
		printf("\n\nWhat do you want to change?\n");
		printf("1.Course code\n2.Course name\n3.EAP\n4.Teacher name\nSelect option: ");
		scanf("%d", &answer);
		
		//menu to modify different values, each one creates different query
		switch(answer)
		{
			case 1:
				printf("Enter new course code: ");
				scanf("%s", coursecode);
				strcat(query, "ainekood = '");
				strcat(query, coursecode);
				strcat(query, "' WHERE id_aine = ");
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
				printf("Enter new course name: ");
				scanf(" %[^\n]s", coursename);
				
				strcat(query, "nimi = '");
				strcat(query, coursename);
				strcat(query, "' WHERE id_aine = ");
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
				printf("Enter new EAP: ");
				scanf("%s", eap);
				
				strcat(query, "eap = ");
				strcat(query, eap);
				strcat(query, " WHERE id_aine = ");
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
			case 4:
				printf("Enter new teacher's name: ");
				scanf(" %[^\n]s", teachername);
				
				strcat(query, "oppejoud = '");
				strcat(query, teachername);
				strcat(query, "' WHERE id_aine = ");
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

void delete_course()
{
	PGresult *res;
    int row;
    int col;
    char search[7];
    char query[200] =  "DELETE FROM aine WHERE id_aine = ";
    char query2[200] = "SELECT * FROM aine WHERE id_aine = ";
    int check;
    char answer;
    
    list_course();//list all courses
       
    printf("Enter course ID to delete: ");
    scanf("%s", search);
    strcat(query, search);
    strcat(query, ";");//query for deleting course based on course ID
    strcat(query2, search);
    strcat(query2, ";");//query for searching whether course exists
    
    res = PQexec(conn, query2);
    //Check query SELECT result status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("We did not get any data!\n");
        PQclear(res);
        return;
    }
    check = PQntuples(res);
    if(check == 0)//if 0 then course is not found so return to main
    {
		printf("Course not found!");
		PQclear(res);
		return;
	}
	else//else list found course
	{
		for (row=0; row<check; row++) 
		{
			printf("%-2s", PQgetvalue(res, row, 0));
			printf("%3s", PQgetvalue(res, row, 1));
			printf("%10s\t  ", PQgetvalue(res, row, 2));
			printf("%-25s\t", PQgetvalue(res, row, 3));
			printf("%s", PQgetvalue(res, row, 4));
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

