#include "types.h"
#include "stat.h"
#include "user.h"

#define MSGSIZE 8
#define NUM_PROC 8

char *itos(int a){

	int temp = a, size = 0;
	while( temp != 0){
		temp = temp /10;
		size++;
	}

	char *p = malloc(size*sizeof(char));

	int i = 0;
	while( a != 0){
		p[i] = a % 10 + '0';
		a = a /10;
		i++;
	}
	char *ans = malloc(size*sizeof(char));

	for(int j = i - 1 ; j > -1 ; j--){
		ans[j] = p[i - j - 1];
	}
	return ans;
}

int stoi(char *a){
	int sum = 0, size = 3;
	for(int j = 0 ; j < size ; j++){
		sum = sum*10 + a[j] -'0';
	}
	return sum;
}

int main(int argc, char *argv[])
{
	if(argc< 2){
		printf(1,"Need type and input filename\n");
		exit();
	}
	char *filename;
	filename=argv[2];
	int type = atoi(argv[1]);
	//printf(1,"Type is %d and filename is %s\n",type, filename);

	int tot_sum = 0;

	int size=1000;
	short arr[size];
	char c;
	int fd = open(filename, 0);
	for(int i=0; i< size; i++){
		read(fd, &c, 1);
		//printf("%c \n",c);
		arr[i]= c-'0';
		read(fd, &c, 1);

	}
  	close(fd);
  	// this is to supress warning
  	//printf(1,"first elem %d\n", arr[0]);



  	//----FILL THE CODE HERE for unicast sum
  	//int myid = 0, from = 0;
  	int ppid = getpid();

  	for(int i = 0 ; i < NUM_PROC; i++){
  		int cid = fork();
  		if(cid < 0){
  			printf(1,"Forking %i process failed", i);
  		} else if (cid == 0){
  			int start = ((i*1000)/NUM_PROC);
  			int end = start + 125;
  			int sum = 0;
  			for(int j = start; j < end; j++){
  				sum = add(sum, arr[j]);
  			}
  			//printf(1,"PID: %d\nstart: %d\nend: %d\nsum: %d\n", getpid(), start, end, sum);
  			char *msg = itos(sum);
  			send(getpid(),ppid,msg);
  			exit();
  		}
  	}

  	for(int k = 0 ; k < NUM_PROC ; k++)
  		wait();

	tot_sum = 0;
	//int po;
  	for(int i = 0 ; i < NUM_PROC; i++){
  		char *msg_child = (char *)malloc(MSGSIZE);
		//msg_child = "500";

		int stat=-1;
		while(stat==-1){
			//printf(1,"heyy");
			stat = recv(msg_child);
			//int size = sizeof(msg_child)/sizeof(char);
  			/*printf(1,"stoi:::");
  			for(int k = 0 ; k < size; k++){

  				printf(1, "%c", msg_child[k]);
  			}*/

		}

		tot_sum = tot_sum + stoi(msg_child);
		//printf(1,"%d\n", po);
		free(msg_child);
  	}

  		/*

  	int pid1 = fork();

  	if(pid1==0){
	// This is child
		int sum = 0;
		for(int i = 0 ; i < size ; i++){
			sum = add(sum, arr[i]);
		}
		char *msg = itos(sum);
		send(0,ppid,msg);

		exit();
	}else{
		wait();
		// This is parent
		char *msg_child = (char *)malloc(MSGSIZE);
		//msg_child = "500";

		int stat=-1;
		while(stat==-1){
			stat = recv(&myid ,&from, msg_child);

		}

		tot_sum = stoi(msg_child);
		free(msg_child);
	}*/
  	//------------------



  	if(type==0){ //unicast sum
		printf(1,"Sum of array for file %s is %d\n", filename,tot_sum);
	}
	exit();
}
