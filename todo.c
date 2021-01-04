#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUF_LEN 256
#define MAX 256

int line(FILE *fptodo,char data[]);
void help();
void date();
void ls(FILE *fptodo, char data[]);
void add(FILE *fptodo, char* task);
void del(FILE *fptodo,int num,char data[]);
void done(FILE *fptodo, FILE *fpdone,int num,char data[]);
void report(FILE *fptodo, FILE *fpdone,char data[]);

int main(int argc, char *argv[])
{
    FILE *fptodo, *fpdone;
    int argi;
    char data[MAX];
    int i;
    
    if(argc<=1){
	    printf("You did not feed any arguments.\n");
	    help();
	    exit(1);
    }
    else if (strcmp(argv[1],"help") == 0){
	    help();
	    exit(1);
    }
    else if(strcmp(argv[1],"ls")==0 ){
	    i = line(fptodo, data);
	    if(i == 0)
		    printf("there is no task in todo.");
	    else
	    	ls(fptodo, data);/*listing left tasks */
    }
    else if(strcmp(argv[1],"add")==0){
	    if(strcmp(argv[2],"") == 0)
		    printf("write task to add in todo.\n");
	    strcpy(data,argv[2]);
	    add(fptodo, data);
    }
    else if(strcmp(argv[1],"del")==0 ){
	    argi = atoi(argv[2]);
	    i = line(fptodo,data);
	    if(argi <= i){
	    	del(fptodo, argi,data);
		printf("Deleted todo #%d\n",argi);
	    }
	    else
		     printf("Error : todo #%d does not exist. Nothing deleted\n",argi);
    }
    else if(strcmp(argv[1],"done")==0 ){
	    argi = atoi(argv[2]);
	    i = line(fptodo,data);
	    if(argi <= i){
	    	done(fptodo, fpdone, argi, data);
		printf("Marked todo #%d as done.\n",argi);
	    }
	    else
		    printf("Error : todo #%d does not exist.\n", argi);
    }
    else if(strcmp(argv[1],"report")==0 )
	    report(fptodo, fpdone, data);
    else{
	    printf("\nyou feed wrong arguments, Use \"$./todo help\" to see Usage.\n");
	    exit(1);
    }

    return 0;
}

void help(){
	printf("\nUsage :- ");
	printf("\n$ ./todo add \"todo item\"  # Add a new todo ");
	printf("\n$ ./todo ls               # Show remaining todos");
	printf("\n$ ./todo del NUMBER       # Delete a todo");
	printf("\n$ ./todo done NUMBER      # Complete a todo");
	printf("\n$ ./todo help             # Show usage");
	printf("\n$ ./todo report           # Statistics\n");
	exit(1);
}

void ls(FILE *fptodo, char data[]){
	/*list all of the remaining  taks*/
	fptodo = fopen("todo.txt", "r+");
	int i =0;

	while(fgets(data,MAX,fptodo) != NULL ){
		i++;
		printf("[%d] %s",i,data);
	}
	fclose(fptodo);

}

void add(FILE *fptodo, char data[]){
	fptodo = fopen("todo.txt", "a+");
	
	fputs(data,fptodo);
	fputs("\n", fptodo);

	printf("Added todo : \"%s\" \n",data);
	fclose(fptodo);
}

void del(FILE *fptodo,int num,char data[]){
	FILE *fpt;
	fptodo = fopen("todo.txt", "r");
	fpt = fopen("temp.txt", "w");
	int i =0;
	
	while (!feof(fptodo)){
		strcpy(data, "\0");
		fgets(data,MAX,fptodo);
		if(!feof(fptodo)){
			i++;
			if(i != num){
				fprintf(fpt, "%s", data);
			}
		}
	}
	fclose(fptodo);
	fclose(fpt);
	remove("todo.txt");
	rename("temp.txt","todo.txt");
	
}

int line(FILE *fptodo,char data[]){
	int c=0;
	fptodo = fopen("todo.txt", "r");
	while(fgets(data,MAX,fptodo) != NULL)
		c++;
	return c;
}
	
void done(FILE *fptodo, FILE *fpdone,int num,char data[]){
	int ctr=0;
	
	fptodo = fopen("todo.txt", "r");
	fpdone = fopen("done.txt", "a+");

	while(!feof(fptodo))
	{
	 	strcpy(data, "\0");
		fgets(data, MAX, fptodo);
		if(!feof(fptodo)){
			ctr++;
			if(ctr == num)
				fprintf(fpdone,"x %s",data);
			}
	}
	fclose(fptodo);
	fclose(fpdone);
	del(fptodo,num,data);
}

void report(FILE *fptodo, FILE *fpdone,char data[]){
	fptodo = fopen("todo.txt", "r");
	fpdone = fopen("done.txt", "r");
	int tnum=0;
	int dnum=0;

	while(fgets(data,MAX,fptodo) != NULL )
		tnum++;
	fclose(fptodo);
	while(fgets(data,MAX,fpdone) != NULL)
		dnum++;
	fclose(fpdone);
	date();	
	printf(" Pending : %d Completed : %d\n", tnum, dnum);
	exit(1);
}

void date(){
	char buf[BUF_LEN] = {0};

	time_t rawtime = time(NULL);

	if(rawtime == -1)
		puts("date function not working.");

	struct tm *ptm = localtime(&rawtime);

	if(ptm == NULL)
		puts("the localtime() function not working.");

	strftime(buf, BUF_LEN, "%Y/%m/%d", ptm);
	fputs(buf, stdout);
}
