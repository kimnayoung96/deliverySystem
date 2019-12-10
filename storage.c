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
	int i,j;
	 
	 
//	for(i = 0; i < deliverySystem[i][y] ; i++)
//	{	
//		for(j = 0; j < deliverySystem[x][j]; i++)
//		{
			deliverySystem[x][y].cnt = 0;
			
//		}
//	}
	
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {             //특정 보관함에 대해 비밀번호를 입력 받아서 맞는지 확인
	char userPasswd[PASSWD_LEN+1];   //사용자가 입력한 passwd를 userPasswd 로 선언하고  
	scanf("%4s",&userPasswd);        
	
	printf("-Input Password for (%d,%d) storage: ",x,y);
	
	if(strcmp(userPasswd,deliverySystem[x][y].passwd)==0||strcmp(userPasswd,masterPassword)==0) // userPasswd랑  passwd랑 같은지 비교하고, userPasswd랑 masterPasswd랑 같은지 
	                                                                                                 //비교해서 만약 같다면  retuen 0
	{
		return 0;
	}
	else
	{
		return -1;																		      //위의 경우가 아니라면 return -1 
	}
	
	
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

	int i, j;

	FILE *fp;
	
	
	fp = fopen(filepath, "w");  //쓰기 모드로 열어서  
	
	
	fprintf(fp,"%d %d\n",systemSize[0],systemSize[1]); //우선 첫 행에 행과 열을 출력 
	fprintf(fp,"%d\n",masterPassword);                 //masterPasswd를 출력  
	
	for(i = 0; i < systemSize[0] ; i++ )
	{
		for(j = 0; j< systemSize[1]; i++)
		{
			fprintf(fp,"%d %d %d %d %s %s\n",i,j,room, passwd, *context);  //fprintf 사용해서 다시 파일 쓰기
		}
 	}

	fclose(fp); //파일 포인터 닫기  
	 
	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {		//택배보관함 구조체 자료구조 생성

	FILE *fp;
	
	fp = fopen(filepath,"r");
	
	int x,y;//x:row y:column
	
	int i,j;
	char *context; 
	
	//context에 동적할당  
	context = (char*)malloc(100*sizeof(char));
	
	
	 //  첫 행 정도(4,6)을 받아들인 다음에  그 크기만큼 storage_t  크기의 공간을 동적으로 할당받기    
	deliverySystem = (storage_t **)malloc(systemSize[0]*sizeof(struct storage_t*));         	  //system[0]에 row를 저장
	
	for(i=0; i<systemSize[0] ; i++)
	{
		deliverySystem = ( storage_t *)malloc(systemSize[1]*sizeof(storage_t));			 //system[1]에 column을 저장 
	}
	
	//FILE fp;
	
	//fp = fopen(*filepath,"r");
	
	fscanf(fp, "%d %d",&systemSize[0],&systemSize[1]); 
	fcanf(fp,"%s", masterPassword);
	
	while(fgetc(FILE *fp) != EOF)/*파일이  끝날 때 까지 */
	{
		fscanf(fp,"%d %d ",&x,&y) ;
		fscanf(fp,"%d %d ",&deliverySystem[x][y].building,&deliverySystem[x][y].room);
		fscanf(fp,"%d", &deliverySystem[x][y].passwd);
		fscanf(fp,"%s",&deliverySystem[x][y].context);
		
		//위와 같이 비밀먼호, context받아주기 
		//while문 돌아갈때마다 
		storedCnt++; 
		
	}
	
	fclose(fp);	
}

	
	//context도 동적할당 
/*	for(i=0; i<systemSize[0] ; i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			deliverySystem[i][j].context       //이걸 동적할당  
		}
	} 
*/	
/*	if(fp == NULL){
		
		return;
	}
	
	deliverySystem -> 
	strcpy(deliverySystem -> systemSize[0],"")
	
		
	if()
	{
		return 0;             //택배보관함 구조체 자료구조가 성공적으로 생성되었다면 return 0 
	}
	else if()
	{
		return -1;            //택배보관함 구조체 자료구조 생성에 실패했다면 return -1 
	}
	
	*/


//free the memory of the deliverySystem 
       //str_createSystem(cahr *filepath)함수의 경우에는 먼저 입력으로 들어온 filepath에 해당하는 파일을 
                 //입출력 함수를 이용해서  읽어들이고 그 안에서 deliverSystem의 크기에 해당하는
				    
void str_freeSystem(void) {		//택배보관함 자료구조 메모리 해제  
	int i,j;
	//context 포인터에 대해서도 해제
	
	for(i = 0; systemSize[0]; i++)
	{
		for(j=0; systemSize[1]; j++)
		{
			free(deliverySystem[i][j].context);
		}
	}
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
	
	//char msg[MAX_MSG_SIZE+1];
	//char passwd[PASSWD_LEN+1];
	 
	//어디에 넣을 것인지 입력하고 나면 그 자리에 들어갈 수 있는지 먼저 확인				        //뭔가 들어있으면 1,비어있으면 0으로 생각  
	//if(deliverySystem[x][y] == NULL)               //내가 넣고자 하는 보관함이 비어있다면   
	if(deliverySystem[x][y].cnt == 0) // 
	{	
		deliverySystem[x][y].building=nBuilding;   //내가 입력한 building#가 txt파일에 쓰여지고 
		deliverySystem[x][y].room=nRoom;       //내가 입력한 room #가 txt파일에 쓰여지고  
	 	deliverySystem[x][y].context=msg;        //내가 입력한 message가 txt파일에 쓰여지고  
		deliverySystem[x][y].passwd[PASSWD_LEN+1]=passwd;	 //내가 입력한 passwd가 txt파일에 쓰여짐  
		deliverySystem[x][y].cnt++;      //성공적으로 택배가 넣어졌다면  
		
		return 0;                   //만약 성공적으로 택배보관함에 택배가 넣어졌다면 return 0
	
	}
	//else if(deliverySystem[x][y] != NULL)         //내가 넣고자 하는 보관함이 이미 차있다면   
	else 
	{
		return -1;                 //택배보관함에 택배가 성공적으로 들어가지 않았다면 return -1 
	}
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {		//특정 보관함에서 내 택배 꺼냄 
	
	
	
	//비밀번호 처리 과정과 택배 보낼 때 문구 뜨게하기    
	//if(inputPasswd(int x, int y) !=  0 )             //내가 입력한 passwd가 틀리면  
	if(inputPasswd != 0)
	{
		printf("------------>password is wrong!!\n");
		return -1;
		
	}
 	else                                            //내가 입력한 passwd가 맞으면  
 	{	
 		printStorageInside(x, y);            
		storedCnt--;                               //저장되어있던 보관함 하나가 비워지니까 -1해주기  
		return 0;
	}		

}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {		//내 택배가 있는 보관함 찾기  
	//building #, 즉 몇 동인지를 먼저 입력
	//room#,즉 몇 호인지를 입력해야함
	  //여기서 만약 내가 이상한 곳을 입력하면 "-----------> Failed to extract my package!"가 출력됨 :이건 모두 main에서짜여짐
	
	int i,j;
	int x,y;
	int cnt; 
	
	deliverySystem[x][y].building;      
	
	
	if(deliverySystem[x][y].building == nBuilding)       //내가 입력한 동과 같은 건물이면 
	{ 
		deliverySystem[x][y].room;	  
		printf("------------>Found package in (%d,%d)\n",systemSize[i],systemSize[j]);
		//deliverySystem[x][y]++;
	}
	
	else  // if(deliverySystem[x][y] != nBuilding)
	{
		deliverySystem[x][y].room;
	}
	
	
	return cnt;
}

