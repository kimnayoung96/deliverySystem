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
	//int i,j;
	 	int i;
	 	
	 	
	
	//Initialize to 0  
			deliverySystem[x][y].building = 0;
			deliverySystem[x][y].room = 0;
			deliverySystem[x][y].cnt = 0;
			
			
			for(i=0; i<(PASSWD_LEN+1);i++)
			{
				deliverySystem[x][y].passwd[i] = '0';   
			}
			
			deliverySystem[x][y].cnt = 0;
			
		
		
		
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {             //특정 보관함에 대해 비밀번호를 입력 받아서 맞는지 확인
	char userPasswd[PASSWD_LEN+1]; 
	
	printf("-Input Password for (%d,%d) storage: ",x,y);
	
	//사용자가 입력한 passwd를 userPasswd 로 선언하고  
	scanf("%4s",&userPasswd);       // Import only 4 characters from the password you entered
	
	
	
	if(strcmp(userPasswd,deliverySystem[x][y].passwd)==0||strcmp(userPasswd,masterPassword)==0) // compare userPasswd with  passwd and compare userPasswd with masterPasswd                                                                                              //비교해서 만약 같다면  retuen 0
	{
		return 0;   //if so return 0 
	}
	else
	{
		return -1;			//if not return -1
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
	
	
	fp = fopen(filepath, "w");  //open in write mode  
	
	
	fprintf(fp,"%d %d\n",systemSize[0],systemSize[1]); //우선 첫 행에 행과 열을 출력 
	fprintf(fp,"%s\n",masterPassword);                 //print masterPasswd  
	
	for(i = 0; i < systemSize[0] ; i++ )
	{
		for(j = 0; j< systemSize[1]; j++)
		{
			if(deliverySystem[i][j].cnt!=0)   
			{	//Re-write files using fprintf (building, room, passwd, context)				
				fprintf(fp,"%d %d %d %d %s %s\n",i,j,deliverySystem[i][j].building,deliverySystem[i][j].room, deliverySystem[i][j].passwd,deliverySystem[i][j].context);   
			}
			
		}
 	}

	fclose(fp); //close fp  
	
	return 0;
	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {		//택배보관함 구조체 자료구조 생성

	FILE *fp;
	
	fp = fopen(filepath,"r");		//open file in read mode 
	
	int x,y;		//x:row y:column
	
	int i,j;
	char *context,c;
	int cnt;
		
	
	fscanf(fp, "%d %d",&systemSize[0],&systemSize[1]); 		//Bring up the row and column 
	fscanf(fp,"%s", masterPassword);     					//Bring up the masterPassword  
	
	  
	
	//  첫 행 정도(4,6)을 받아들인 다음에  그 크기만큼 storage_t  크기의 공간을 동적으로 할당받기 
	deliverySystem = (storage_t **)malloc(systemSize[0]*sizeof( storage_t*));         	  //Dynamic Allocation to system[0](row)
	
	for(i=0; i<systemSize[0] ; i++)
	{
		deliverySystem[i]= ( storage_t *)malloc(systemSize[1]*sizeof( storage_t ));			 //Dynamic Allocation to system[1](column)  
			 
	}
	
	//Dynamic Allocation to context  
	context = (char*)malloc(100*sizeof(char));   
	
	for(i = 0; i<systemSize[0]; i++)
	{
		for(j=0; j<systemSize[1]; j++)
		{
			        
			initStorage(i,j);
		}
	}
	
	
	//To prevent the last value of the value read by fScanf from being read twice, check if the value of fscanf is the value of EOF
	while(EOF!=fscanf(fp,"%d %d ",&x,&y))     //If the return value of fscanf is not EOF  
	{	
		
		fscanf(fp,"%d %d ",&deliverySystem[x][y].building, &deliverySystem[x][y].room);  // receive building number and room number
		fscanf(fp,"%4s", deliverySystem[x][y].passwd); 									//receive passwd  
		
			deliverySystem[x][y].context = (char*)malloc(100*sizeof(char));
		
		fscanf(fp,"%s",deliverySystem[x][y].context);                                   //receive context 
	

		deliverySystem[x][y].cnt++;   
		
		 
		storedCnt++;    //when the while문  rotates, storedCnt becomes +1
		  
	
		
	}
	
	fclose(fp);	//close fp 
	
	return 0;
}

	

//free the memory of the deliverySystem 			    
void str_freeSystem(void) {		//택배보관함 자료구조 메모리 해제  
	int i,j;

	
	//Turn off memory for context 
	for(i = 0; i< systemSize[0]; i++)
	{
		for(j=0; j< systemSize[1]; j++)
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
	

	int i;
	 
	//After entering where to put it, first check if the package can be pushed to the storage 			        
	if(deliverySystem[x][y].cnt == 0)  //If the storage box I'm trying to put in is empty 
	{	
		
		deliverySystem[x][y].building=nBuilding;  //The building I entered is written to the txt file
		deliverySystem[x][y].room=nRoom;      //The room I entered is written to the txt file 
	 	deliverySystem[x][y].context=msg;       // The context I entered is written to the txt file
		
		
		//The passwd I entered is written to the txt file
		for(i=0 ; i<(PASSWD_LEN+1) ; i++)
		{						
				deliverySystem[x][y].passwd[i]=passwd[i];
		} 
	

	
		deliverySystem[x][y].cnt++;      //+1 because the parcel was successfully delivered  
		storedCnt++;
		return 0;                  //If the parcel is successfully placed in the storage, return 0
	
	}
  
	else 
	{
		return -1;                 //If the parcel is not successfully placed in the storage, return 0
	}
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {		//특정 보관함에서 내 택배 꺼냄 
	
	//비밀번호 처리 과정과 택배 보낼 때 문구 뜨게하기    
              
	if(inputPasswd(x,y) != 0)      //If the passwd I typed is wrong
	{
		printf("------------>password is wrong!!\n");          //사용자가 입력한 password가 틀렸다는 문구가 출력됨  
		return -1;
		
	}
 	else                                            //If the passwd I typed is right  
 	{	
 		printStorageInside(x, y);  
		initStorage(x,y);          					//exactStorage will empty one storage so initialize the components in struct(구조체)  
		storedCnt--;                               //Because one of the storage will be empty,StoredCnt -1  
		return 0;
	}		

}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {		//내 택배가 있는 보관함 찾기  
	
	
	int i,j;
	int x,y;
	int cnt; 
	
	deliverySystem[x][y].building;      
	
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			if(deliverySystem[i][j].building == nBuilding&&deliverySystem[i][j].room==nRoom)  //If the building and nBuilding I entered are the same and if the room and nroom I entered are the same
			{ 
					  
				printf("------------>Found package in (%d,%d)\n",i,j);         //택배 찾았다는 문구 출력  
				cnt++;
		
			}
		}
	}

	
	
	
	return cnt;
}

