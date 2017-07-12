#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Functions.h"

int connection()
{
	//Creating a connection to database
    conn = PQconnectdb("dbname=***** host=***** user=***** password=*****");
    
    //Check connection status
    if (PQstatus(conn) == CONNECTION_BAD) {
        printf("We were unable to connect to the database\n");
        return 1;
    }
    return 0;
}

void disconnect()
{
	//Close the connection and free the memory used by PGconn handler
    PQfinish(conn);
}