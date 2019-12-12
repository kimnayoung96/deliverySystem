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
	//구조체 안에 들어있는 것 모두 0으로 초기화 시켜주기  
	
			deliverySystem[x][y].building = 0;
			deliverySystem[x][y].room = 0;
			deliverySystem[x][y].cnt = 0;
			
			
			for(i=0; i<(PASSWD_LEN+1);i++)
			{
				deliverySystem[x][y].passwd[i] = '0'; // 5글자를 받아와도 conext는 char로 가져오기 때문에 크기를 5로 설정해도 4개만 가져오고 마지막은 NULL로!  
			}
			//strcpy(deliverySystem[x][y].context,"electronic");
			//strcpy(deliverySystem[x][y].context,"initialize");
			deliverySystem[x][y].cnt = 0;
		
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {             //특정 보관함에 대해 비밀번호를 입력 받아서 맞는지 확인
	char userPasswd[PASSWD_LEN+1]; 
	printf("-Input Password for (%d,%d) storage: ",x,y);
	  //사용자가 입력한 passwd를 userPasswd 로 선언하고  
	scanf("%4s",&userPasswd);       //사용자가 입력한 passwd에서 4글자만 가져오기  
	
	
	
	if(strcmp(userPasswd,deliverySystem[x][y].passwd)==0||strcmp(userPasswd,masterPassword)==0) // userPasswd랑  passwd랑 같은지 비교하고, userPasswd랑 masterPasswd랑 같은지                                                                                               //비교해서 만약 같다면  retuen 0
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
	fprintf(fp,"%s\n",masterPassword);                 //masterPasswd를 출력  
	
	for(i = 0; i < systemSize[0] ; i++ )
	{
		for(j = 0; j< systemSize[1]; j++)
		{
			if(deliverySystem[i][j].cnt!=0)   
			{	//fprint 사용해서 다시 파일 쓰기 (building, room, passwd, context)				
				fprintf(fp,"%d %d %d %d %s %s\n",i,j,deliverySystem[i][j].building,deliverySystem[i][j].room, deliverySystem[i][j].passwd,deliverySystem[i][j].context);   
			}
			
		}
 	}

	fclose(fp); //파일 포인터 닫기  
	
	return 0;
	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {		//택배보관함 구조체 자료구조 생성

	FILE *fp;
	
	fp = fopen(filepath,"r");		//파일을 읽기모드로 연다  
	
	int x,y;//x:row y:column
	
	int i,j;
	char *context,c;
	int cnt;
		
	
	fscanf(fp, "%d %d",&systemSize[0],&systemSize[1]); //첫번째 행과 열을 받아온다. 
	fscanf(fp,"%s", masterPassword); //masterPassword를 받아온다.  
	
	//cnt 메모리 할당
	//cnt = (int*)malloc(deliverySystem[x][y].cnt*sizeof(int));

	
	//////////////////////////////////////////
	//context에 동적할당  
	//context = (char*)malloc(100*sizeof(char));   
	
	//  첫 행 정도(4,6)을 받아들인 다음에  그 크기만큼 storage_t  크기의 공간을 동적으로 할당받기 
	deliverySystem = (storage_t **)malloc(systemSize[0]*sizeof( storage_t*));         	  //system[0](row)에 동적할당  
	
	for(i=0; i<systemSize[0] ; i++)
	{
		deliverySystem[i]= ( storage_t *)malloc(systemSize[1]*sizeof( storage_t ));			 //system[1](column)에 동적할당  
	//	deliverySystem[i] = ( storage_t *)malloc(systemSize[1]*sizeof( storage_t ));			 
	}
	//context에 동적할당  
	context = (char*)malloc(100*sizeof(char));   
	
	for(i = 0; i<systemSize[0]; i++)
	{
		for(j=0; j<systemSize[1]; j++)
		{
			//deliverySystem[i][j].cnt = 0;         
			initStorage(i,j);
		}
	}
	
	//FILE fp;
	
	//fp = fopen(*filepath,"r");
	
//	fscanf(fp, "%d %d",&systemSize[0],&systemSize[1]); 
//	fscanf(fp,"%s", masterPassword);
	
//	while(EOF!=fscanf(fp, "%s", &c)
   //(파일이 끝날 때까지)fgetc를 이용해서 파일로부터 한 문자씩 입력받는다.  
//	while()
	while(EOF!=fscanf(fp,"%d %d ",&x,&y))
	{	
//		if(feof(fp)!=0)
//		{
//			break;
//		}
			//printf("fgetc = %c",fgetc(fp));
		//fscanf(fp,"%d %d ",&x,&y) ;     //행과 열을 나타낼 x,y를 받아주고  ;
		//printf("x = %d y = %d\n",x,y);
		fscanf(fp,"%d %d ",&deliverySystem[x][y].building, &deliverySystem[x][y].room);  // 건물(동)과 room번호를 받아주기  
		//printf("building= %d room = %d\n",deliverySystem[x][y].building,deliverySystem[x][y].room);
		fscanf(fp,"%4s", deliverySystem[x][y].passwd); //passwd를 받아주기  
		//printf("deliverySystem[x][y].passwd= %s\n",deliverySystem[x][y].passwd);
			
			deliverySystem[x][y].context = (char*)malloc(100*sizeof(char));
		
		fscanf(fp,"%s",deliverySystem[x][y].context); //context를 받아주기 
		//printf("context = %s \n",deliverySystem[x][y].context) ;
		
		//	deliverySystem[x][y].cnt = (int*)malloc(sizeof(int));

			  
		deliverySystem[x][y].cnt++;   
		//printf("context= %s \n",deliverySystem[x][y].context);
		
		//위와 같이 비밀먼호, context받아주기 
		//while문 돌아갈때마다 
		storedCnt++;     //위에 처럼 행,열, 건물(동), room번호, passwd,context 모두 입력받으면 저장된 package가 하나 늘어남  
	//	printf("fgetc = %c",fgetc(fp));
		
	}
	
	fclose(fp);	//파일 포인터 닫기  
	
	return 0;
}

	

//free the memory of the deliverySystem 
       //str_createSystem(cahr *filepath)함수의 경우에는 먼저 입력으로 들어온 filepath에 해당하는 파일을 
                 //입출력 함수를 이용해서  읽어들이고 그 안에서 deliverSystem의 크기에 해당하는
				    
void str_freeSystem(void) {		//택배보관함 자료구조 메모리 해제  
	int i,j;

	
	//context포인터에 대해서 메모리 해제  
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
	
	//char msg[MAX_MSG_SIZE+1];
	//char passwd[PASSWD_LEN+1];
	
	int i;
	 
	//어디에 넣을 것인지 입력하고 나면 그 자리에 들어갈 수 있는지 먼저 확인				        //뭔가 들어있으면 1,비어있으면 0으로 생각  
	//if(deliverySystem[x][y] == NULL)                
	if(deliverySystem[x][y].cnt == 0)  //내가 넣고자 하는 보관함이 비어있다면 
	{	
		
		deliverySystem[x][y].building=nBuilding;   //내가 입력한 building #가 txt파일에 쓰여지고 
		deliverySystem[x][y].room=nRoom;       //내가 입력한 room #가 txt파일에 쓰여지고  
	 	deliverySystem[x][y].context=msg;        //내가 입력한 message가 txt파일에 쓰여지고  
		//!!!!!deliverySystem[x][y].passwd[PASSWD_LEN+1]=passwd;	 //내가 입력한 passwd가 txt파일에 쓰여짐
		
		//deliverySystem[i].passwd;
		for(i=0 ; i<(PASSWD_LEN+1) ; i++) {
			//strcpy(deliverySystem[x][y].passwd, passwd[i]);
			deliverySystem[x][y].passwd[i]=passwd[i];
		} 
	

	
		deliverySystem[x][y].cnt++;      //성공적으로 택배가 넣어졌으니까 +1해주기  
		
		return 0;                   //만약 성공적으로 택배보관함에 택배가 넣어졌다면 return 0
	
	}
  
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
	//if(inputPasswd(int x, int y) !=  0 )               
	if(inputPasswd(x,y) != 0)      //내가 입력한 passwd가 틀리면
	{
		printf("------------>password is wrong!!\n");          //사용자가 입력한 password가 틀렸다는 문구가 출력됨  
		return -1;
		
	}
 	else                                            //내가 입력한 passwd가 맞으면  
 	{	
 		printStorageInside(x, y);  
		initStorage(x,y);          					//extractStorage를 해주면 storage 하나가 비워주기까 initStorage해주기!!  
		storedCnt--;                               //저장되어있던 보관함 하나가 비워지니까 -1해주기  
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
			if(deliverySystem[i][j].building == nBuilding&&deliverySystem[i][j].room==nRoom)       //내가 입력한 동과 같은 건물인지와 내가 입력한 room number와 같은 방이라면   
			{ 
				//deliverySystem[x][y].room;	  
				printf("------------>Found package in (%d,%d)\n",i,j);         //택배 찾았다는 문구 출력  
				cnt++;
		//deliverySystem[x][y]++;
			}
		}
	}

	
	/*else  // if(deliverySystem[x][y] != nBuilding)
	{
		deliverySystem[x][y].room;
	}*/
	
	
	return cnt;
}

