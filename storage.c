#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strcpy�Լ��� ����� �������  
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
static void initStorage(int x, int y) {              //Ư�� ������ �ϳ��� �ʱ�ȭ ��  
//	int systemsize[0] = NULL; //storage�� �ʱ�ȭ
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
static int inputPasswd(int x, int y) {             //Ư�� �����Կ� ���� ��й�ȣ�� �Է� �޾Ƽ� �´��� Ȯ��
	char userPasswd[PASSWD_LEN+1];   //����ڰ� �Է��� passwd�� userPasswd �� �����ϰ�  
	scanf("%4s",&userPasswd);        
	
	printf("-Input Password for (%d,%d) storage: ",x,y);
	
	if(strcmp(userPasswd,deliverySystem[x][y].passwd)==0||strcmp(userPasswd,masterPassword)==0) // userPasswd��  passwd�� ������ ���ϰ�, userPasswd�� masterPasswd�� ������ 
	                                                                                                 //���ؼ� ���� ���ٸ�  retuen 0
	{
		return 0;
	}
	else
	{
		return -1;																		      //���� ��찡 �ƴ϶�� return -1 
	}
	
	
}






// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {		//���� �����Ե��� ���� �� ���� ������ ���Ͽ� ����  
//main�Լ� ���� ���� 
	int room;
	char passwd;
	char *context;

	int i, j;

	FILE *fp;
	
	
	fp = fopen(filepath, "w");  //���� ���� ���  
	
	
	fprintf(fp,"%d %d\n",systemSize[0],systemSize[1]); //�켱 ù �࿡ ��� ���� ��� 
	fprintf(fp,"%d\n",masterPassword);                 //masterPasswd�� ���  
	
	for(i = 0; i < systemSize[0] ; i++ )
	{
		for(j = 0; j< systemSize[1]; i++)
		{
			fprintf(fp,"%d %d %d %d %s %s\n",i,j,room, passwd, *context);  //fprintf ����ؼ� �ٽ� ���� ����
		}
 	}

	fclose(fp); //���� ������ �ݱ�  
	 
	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {		//�ù躸���� ����ü �ڷᱸ�� ����

	FILE *fp;
	
	fp = fopen(filepath,"r");
	
	int x,y;//x:row y:column
	
	int i,j;
	char *context; 
	
	//context�� �����Ҵ�  
	context = (char*)malloc(100*sizeof(char));
	
	
	 //  ù �� ����(4,6)�� �޾Ƶ��� ������  �� ũ�⸸ŭ storage_t  ũ���� ������ �������� �Ҵ�ޱ�    
	deliverySystem = (storage_t **)malloc(systemSize[0]*sizeof(struct storage_t*));         	  //system[0]�� row�� ����
	
	for(i=0; i<systemSize[0] ; i++)
	{
		deliverySystem = ( storage_t *)malloc(systemSize[1]*sizeof(storage_t));			 //system[1]�� column�� ���� 
	}
	
	//FILE fp;
	
	//fp = fopen(*filepath,"r");
	
	fscanf(fp, "%d %d",&systemSize[0],&systemSize[1]); 
	fcanf(fp,"%s", masterPassword);
	
	while(fgetc(FILE *fp) != EOF)/*������  ���� �� ���� */
	{
		fscanf(fp,"%d %d ",&x,&y) ;
		fscanf(fp,"%d %d ",&deliverySystem[x][y].building,&deliverySystem[x][y].room);
		fscanf(fp,"%d", &deliverySystem[x][y].passwd);
		fscanf(fp,"%s",&deliverySystem[x][y].context);
		
		//���� ���� ��и�ȣ, context�޾��ֱ� 
		//while�� ���ư������� 
		storedCnt++; 
		
	}
	
	fclose(fp);	
}

	
	//context�� �����Ҵ� 
/*	for(i=0; i<systemSize[0] ; i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			deliverySystem[i][j].context       //�̰� �����Ҵ�  
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
		return 0;             //�ù躸���� ����ü �ڷᱸ���� ���������� �����Ǿ��ٸ� return 0 
	}
	else if()
	{
		return -1;            //�ù躸���� ����ü �ڷᱸ�� ������ �����ߴٸ� return -1 
	}
	
	*/


//free the memory of the deliverySystem 
       //str_createSystem(cahr *filepath)�Լ��� ��쿡�� ���� �Է����� ���� filepath�� �ش��ϴ� ������ 
                 //����� �Լ��� �̿��ؼ�  �о���̰� �� �ȿ��� deliverSystem�� ũ�⿡ �ش��ϴ�
				    
void str_freeSystem(void) {		//�ù躸���� �ڷᱸ�� �޸� ����  
	int i,j;
	//context �����Ϳ� ���ؼ��� ����
	
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
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) { //Ư�� �����Կ� �ù� �ֱ� 
	
	//char msg[MAX_MSG_SIZE+1];
	//char passwd[PASSWD_LEN+1];
	 
	//��� ���� ������ �Է��ϰ� ���� �� �ڸ��� �� �� �ִ��� ���� Ȯ��				        //���� ��������� 1,��������� 0���� ����  
	//if(deliverySystem[x][y] == NULL)               //���� �ְ��� �ϴ� �������� ����ִٸ�   
	if(deliverySystem[x][y].cnt == 0) // 
	{	
		deliverySystem[x][y].building=nBuilding;   //���� �Է��� building#�� txt���Ͽ� �������� 
		deliverySystem[x][y].room=nRoom;       //���� �Է��� room #�� txt���Ͽ� ��������  
	 	deliverySystem[x][y].context=msg;        //���� �Է��� message�� txt���Ͽ� ��������  
		deliverySystem[x][y].passwd[PASSWD_LEN+1]=passwd;	 //���� �Է��� passwd�� txt���Ͽ� ������  
		deliverySystem[x][y].cnt++;      //���������� �ù谡 �־����ٸ�  
		
		return 0;                   //���� ���������� �ù躸���Կ� �ù谡 �־����ٸ� return 0
	
	}
	//else if(deliverySystem[x][y] != NULL)         //���� �ְ��� �ϴ� �������� �̹� ���ִٸ�   
	else 
	{
		return -1;                 //�ù躸���Կ� �ù谡 ���������� ���� �ʾҴٸ� return -1 
	}
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {		//Ư�� �����Կ��� �� �ù� ���� 
	
	
	
	//��й�ȣ ó�� ������ �ù� ���� �� ���� �߰��ϱ�    
	//if(inputPasswd(int x, int y) !=  0 )             //���� �Է��� passwd�� Ʋ����  
	if(inputPasswd != 0)
	{
		printf("------------>password is wrong!!\n");
		return -1;
		
	}
 	else                                            //���� �Է��� passwd�� ������  
 	{	
 		printStorageInside(x, y);            
		storedCnt--;                               //����Ǿ��ִ� ������ �ϳ��� ������ϱ� -1���ֱ�  
		return 0;
	}		

}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {		//�� �ù谡 �ִ� ������ ã��  
	//building #, �� �� �������� ���� �Է�
	//room#,�� �� ȣ������ �Է��ؾ���
	  //���⼭ ���� ���� �̻��� ���� �Է��ϸ� "-----------> Failed to extract my package!"�� ��µ� :�̰� ��� main����¥����
	
	int i,j;
	int x,y;
	int cnt; 
	
	deliverySystem[x][y].building;      
	
	
	if(deliverySystem[x][y].building == nBuilding)       //���� �Է��� ���� ���� �ǹ��̸� 
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

