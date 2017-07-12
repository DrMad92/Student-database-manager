/* File: main.c
 * Author: Muhammet Madraimov 156213 MVEB22
 * Date: 07.03.2016
 * Updated: 14.03.2016
 * Description: Text-based user interface program which does:
 * 1. Adding, modifying, deleting student
 * 2. Adding, modifying, deleting course
 * 3. Giving grade to student on specific course. Calculating GPA
 * 4. Searching student based on GPA.
 * Program communicates with the given database and uses it for reading and storing data.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pgsql/libpq-fe.h>
#include "Functions.h"

int main(void) 
{
	int answer, check;
	 
	check = connection();//conncetion returns 1 if error, otherwise 0
	if(check == 1)
	{
		printf("Quiting!\n");
		exit(1);
	}
	
	do
	{
		printf("\nMAIN MENU\n\n1.Student\n2.Course\n3.Give grade\n4.Search\n5.Exit\n");
		printf("Select option: ");
		scanf("%d", &answer);
	
		switch(answer)
		{
			case 1: 
				printf("\nSTUDENT MENU\n\n1.Add student\n2.Modify student\n3.Delete student\n4.List\n5.Back\n");
				printf("Select option: ");
				scanf("%d", &answer);
				switch(answer)
				{
					case 1: add_student();break;
					case 2: modify_student();break;
					case 3: delete_student();break;
					case 4: list_student();break;
					case 5: break;
				}answer = -1; break;
			case 2:
				printf("\nCOURSE MENU\n\n1.Add course\n2.Modify course\n3.Delete course\n4.List course\n5.Back\n");
				printf("Select option: ");
				scanf("%d", &answer);
				switch(answer)
				{
					case 1: add_course(); break;
					case 2: modify_course();break;
					case 3: delete_course();break;
					case 4: list_course(); break;
					case 5: break;
				}answer = -1;break;
				
			case 3: give_grade();break;
			case 4: search_students();break;
			case 5: disconnect();
		}
	}while(answer != 5);
	return 0;
}
