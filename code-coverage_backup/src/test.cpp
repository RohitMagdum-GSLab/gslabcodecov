#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	{
		FILE* dbFile = NULL;
		char* readline = NULL;
		size_t len = 0;
		dbFile = fopen("/var/tmp/databaseFile.db", "r+");
		while(getline(&readline, &len, dbFile) != -1){
			if(strstr(readline, ":^*^0")){
			fseek(dbFile, -3, SEEK_CUR);
			fwrite("1",strlen("1"), 1, dbFile);
			}
		}
	}
   printf("Hello World.\n");
   return 0;
}
