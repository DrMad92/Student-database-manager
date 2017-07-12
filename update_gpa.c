#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Functions.h"

//taking students code as argument from give_grade function
void update_gpa(char searchstudent[7])
{
	PGresult *res;
	char query[200] = "SELECT * FROM hinne WHERE tudengikood = ";
	char query2[200] = "UPDATE opilane SET kkh = ";
	char query3[200] = "SELECT eap FROM aine WHERE id_aine = ";
	char query4[200];
	int rec_count, hinne[20], row, id_aine[20], i=0, Eap[20], eap=0, product=0;
	float gpa;
	char GPA[5];
	char ID_aine[4];
	
	strcat(query, searchstudent);
	strcat(query, ";");
	res = PQexec(conn, query);
	//Check query SELECT result status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("We did not get any data!\n");
        PQclear(res);
        return;
    }
	rec_count = PQntuples(res);//counting total grades of a student
	
	for(row=0; row<rec_count; row++)
	{
		id_aine[row] = atoi(PQgetvalue(res, row, 1));//array of course IDs
		hinne[row] = atoi(PQgetvalue(res, row, 3));//array of grades respective to id_aine array
	}
	
	
	for(i=0; i<rec_count;i++)//loop which gets EAP for each id_ainne value
	{
		PQclear(res);
		ID_aine[0] = '\0';
		query4[0] = '\0';
		sprintf(ID_aine, "%d", id_aine[i]);
		strcpy(query4, query3); 
		strcat(query4, ID_aine); 
		strcat(query4, ";"); 
		
		res = PQexec(conn, query4);
		//Check query SELECT result status
		if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			printf("We did not get any data!\n");
			PQclear(res);
			return;
		}
		Eap[i] = atoi(PQgetvalue(res, 0, 0));//array of EAP
	}
	
	for(i=0;i<rec_count;i++)//loop calculating total EAP and total EAP*grade
	{
		eap = eap + Eap[i];
		product = product + Eap[i]*hinne[i];
	}
	
	
	gpa = (float)product/eap;
	sprintf(GPA, "%.2f", gpa);
	
	PQclear(res);
	strcat(query2, GPA);
	strcat(query2, " WHERE tudengikood = ");
	strcat(query2, searchstudent);
	strcat(query2, ";");
	
	//Updating GPA
	res = PQexec(conn, query2);
	printf("Updated GPA - %.2f", gpa);
	return;
}
