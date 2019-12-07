#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strcpy함수가 선언된 헤더파일  
#include "storage.h"

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/
typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;                       
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password




// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {              //특정 보관함 하나를 초기화 함  
//	int systemsize[0] = NULL; //storage를 초기화
	FILE fp; 
	
	deliverySystem[x][y].cnt = NULL;
	 
	 
	
	 
	
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {             //특정 보관함에 대해 비밀번호를 입력 받아서 맞는지 확인
	char passwd[PASSWD_LEN+1];
	char *validPasswd;
	scanf("%4s",&inputPasswd);
	validPasswd = deliverySystem[x][y].passwd;   
	
	if (inputPasswd == *validPasswd)       //내가 입력한 inputPasswd와 *validPasswd가 가리키는 변수가 같은 경우  
	{
		return 0;
	}

	else if (inputPasswd != *validPasswd) //내가 입력한 inputPasswd와 *validPasswd가 가리키는 변수가 다른 경우  
	{
		return -1;
	}
														//masterPasswd의 경우에는 열리도록 해야겠지...
														 
}






// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {		//현재 보관함들의 상태 및 설정 값들을 파일에 저장  
//main함수 보고 참고 
	int room;
	char passwd;
	char *context;


	FILE *fp;
	
	
	fp = fopen(filepath, "w");  //쓰기 모드로 열어서  
	fprintf(fp,"%d %d\n",systemSize[0],systemSize[1]); //우선 첫 행에 행과 열을 출력 
	fprintf(fp,"%d\n",masterPassword);                 //masterPasswd를 출력  
	fprintf(fp,"%d %d %d %d %s %s\n",systemSize[0],systemSize[1],room, passwd, *context);  //fprintf 사용해서 다시 파일 쓰기
	fclose(fp); //파일 포인터 닫기  
	 
	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {		//택배보관함 구조체 자료구조 생성


	 
	int i;

	  
	deliverySystem = (struct storage_t **)malloc(systemSize[0]*sizeof(storage_t*));         	  //system[0]에 row를 저장
	
	for(i=0; i<systemSize[0] ; i++)
	{
		deliverySystem = (struct storage_t *)malloc(systemSize[1]*sizeof(storage_t));			 //system[1]에 column을 저장 
	}
	
		
	if()
	{
		return 0;             //택배보관함 구조체 자료구조가 성공적으로 생성되었다면 return 0 
	}
	else if()
	{
		return -1;            //택배보관함 구조체 자료구조 생성에 실패했다면 return -1 
	}
	
	
}

//free the memory of the deliverySystem 
       //str_createSystem(cahr *filepath)함수의 경우에는 먼저 입력으로 들어온 filepath에 해당하는 파일을 
                 //입출력 함수를 이용해서  읽어들이고 그 안에서 deliverSystem의 크기에 해당하는
				     //  첫 행 정도(4,6)을 받아들인 다음에  그 크기만큼 storage_t  크기의 공간을 동적으로 할당받기  
void str_freeSystem(void) {		//택배보관함 자료구조 메모리 해제  
	
	
	free(filepath)
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {		
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {		
	if (x < 0 || x >= systemSize[0])
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) { //특정 보관함에 택배 넣기 
	int x,y;
	int nBuilding, nRoom; 
	char msg;
		//어느 위치에 넣을지 입력하기  
		
	if()
	{
		return 0;                   //만약 성공적으로 택배보관함에 택배가 넣어졌다면 return 0
	}
	else if()
	{
		return -1;                 //택배보관함에 택배가 성공적으로 들어가지 않았다면 return -1 
	}
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {		//특정 보관함에서 내 택배 꺼냄 
	
	
	
	
	
	//비밀번호 처리 과정  
	if(strcmp(dde) == 0)                 //비밀번호를 txt에서 긁어와서 내가 입력한 비밀번호랑 같은지 비교해서 같으면 0 반환   
	{	
		return 0;
	}
	
	else if(strcmp() == masterPasswd){
		return 0;
	}
	else    //txt에서 긁어온 비밀번호(진짜 비밀번호)와 내가 입력한 비밀번호가 같이 않으면 return -1 
	{
		return -1;
	}
	 
	//택배 꺼낼 때 내가 입력해놓았던 문구가 함께 출력됨 
	strcpy()
	while((c = fgetc(fp)) != EOF) //파일 끝이 아니면 반복해서 C에 글자 하나를 읽어서 저장.....? 
	                                          
	
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {		//내 택배가 있는 보관함 찾기  
	//building #, 즉 몇 동인지를 먼저 입력
	//room#,즉 몇 호인지를 입력해야함
	  //여기서 만약 내가 이상한 곳을 입력하면 "-----------> Failed to extract my package!"가 출력됨 :이건 모두 main에서짜여짐
	  
	
	   
	
	return cnt;
}
