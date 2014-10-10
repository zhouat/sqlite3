#include<stdio.h>
#include<string.h>
#include"sqlite3.h"

#define DEBUG 1
#define ERROR -1
#define SUCCESS 0

sqlite3* db = NULL;

int handle_db(sqlite3 *db, const char* sql);

int open_db()
{
	if(db != NULL)
	{
		if(DEBUG)
		  printf("db already open!\n");
		return SUCCESS;
	}

	int user_info = sqlite3_open("info.db",&db);
        if(user_info){
                fprintf(stderr, "open db failed!  %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
		return ERROR;
        }else{  
                if(DEBUG)
                   printf("open db success!\n");
		return SUCCESS;
	}
}

int close_db()
{
	if(db != NULL)
	{
	  sqlite3_close(db);
	  db = NULL;
	  return SUCCESS;
	}else{
	  if(DEBUG)
	    printf("close db failed!\n");
	  return ERROR;		
	}
		
}

void init_db()
{	
	if(open_db()==SUCCESS){

	char* sql = "CREATE TABLE IF NOT EXISTS info(\
			id INTEGER PRIMARY KEY,\
                    	name VARCHAR(50),\
                    	passwd VARCHAR(50)\
        	    )";

	handle_db(db,sql);
	close_db(db);
	}	
}

void insert_db(const char* u_name, const char* u_passwd)
{
	if(open_db()==SUCCESS){
	
	char sql[1024];
	sprintf(sql, "INSERT INTO info(name, passwd) VALUES ('%s', '%s')", u_name, u_passwd);
	
        handle_db(db,sql);
        close_db(db);	
	}
}

void select_db(const char* u_name, const char* u_passwd)
{
	if(DEBUG)
	  printf("[%s]  [%s]\n", u_name, u_passwd);

	if(open_db()==SUCCESS){

        char sql[1024];
	sprintf(sql, "SELECT * from info WHERE name = '%s' AND passwd= '%s' ", u_name, u_passwd);
	int row,column;
	char **result;
	char *errorMsg; 
	sqlite3_get_table(db, sql, &result, &row, &column, &errorMsg);

	if(DEBUG)
	   printf("row: [%d] \n",row);

       	handle_db(db,sql);
        close_db(db);
        }else{
		if(DEBUG)
		  printf("open failed![%s]\n",sqlite3_errmsg(db));
	}
}

int handle_db(sqlite3 *db, const char* sql)
{
	char* error;
	int num =sqlite3_exec(db, sql, 0, 0, &error);		
	if(error){
		printf("operate failed! [%s]\n", sql);
		return ERROR;
	}else{
		if(DEBUG)
		   printf("operate success! [%s]\n", sql);
		return num;
	}
}
	
void main()
{
	init_db();
	insert_db("zat","123");
	select_db("zat","123");	
}
