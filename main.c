#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
// #include <sys/wait.h>
#include <signal.h>
#include <errno.h>
// #include "linked_list.h"


// add new node to linklist
struct Node* head = NULL;
struct Node {
  int PID;
  char ProcessName[50];
  struct Node * next;
};

void addNew(int PID,char *ProcessName);

void addNew(int PID,char *ProcessName) {
   struct Node *link = (struct Node*) malloc(sizeof(struct Node));
   link->PID = PID;
   strcpy(link->ProcessName,ProcessName);
   link->next = head;
   head = link;
}

// delete node from linklist 
struct DelN{
  int a;
  int PID;
  char ** cmd[100];
  struct Node *next;
};

void delN(int EPID);

void delN(int EPID){
  struct Node *q=head;
  struct Node *nextNode=head->next;  
  int D=0;      // use for count if this pid in the linklist, if 0 than this pid is not in the linklit
  // if the first pid is what we need
  if (q->PID==EPID){
    if(nextNode!=NULL){
      q=head->next;
      free(head);
      D=1;
    }
    else{
      head=NULL;
      free(head);
      D=1;
    }    
  }
  // if the first pid is not we need 
  else{     
    while(nextNode!=NULL){
      if (nextNode->PID==EPID){
        q->next=nextNode->next;
        free(nextNode);
        D=1;
        break;
      }
      else{
        q=nextNode;
        nextNode=nextNode->next;
      }
    }
    // if we didnot fin the pid.
    if(D==0){
      printf("wrong pid! \n");
    }
    
  } 
}

void func_BG(char **cmd){
  pid_t fpid; // fork return pid
  char cmd2[60];
  fpid = fork();
  strcpy(cmd2,cmd[1]);

  // wrong situation
  if (fpid<0){
    printf("error in fork! \n");   
  }
  // child process
  else if (fpid == 0) {
    char *temp=cmd2+2;
    char *argv1_execvp[] = {temp,NULL};
    int check=0;
    char name1[50];
    char name2[50];
    strcpy(name1,"./");
    strcpy(name2,cmd[1]);
    strcat(name1,name2);
    check=execvp(name1, argv1_execvp);
    if (check==-1){
      char *argv2_execvp[] = {temp,NULL};
      execvp("./test", argv2_execvp);
      printf("Pman: > ");
    }
  }
  // parent process
  else if (fpid>0){
    printf("Pman: > ");
    addNew(fpid,cmd[1]);
  } 
}

void func_BGlist(char **cmd){
	//Your code here;
  int CountBgWork=0 ;     //use for count how many total work
  char cwd[500];
  struct Node *certen = head;
  getcwd(cwd,sizeof(cwd));

  // if linklist is empty
  if (certen == NULL){
    printf("Total background jobs: 0\n");
    printf("Pman: > ");
  }
  else{
    while(certen != NULL) {   
      if(certen->ProcessName!=NULL){
        printf("%d: %s %s \n",certen->PID,certen->ProcessName,cwd);
        certen = certen->next;
        CountBgWork+=1;
      }     
    }
    printf("Total background jobs: %d\n",CountBgWork);
    printf("Pman: > ");
  }
}

void func_BGkill(char * str_pid){
	//Your code here
  int Ipid=atoi(str_pid);
  int retVal; 
  retVal = kill(Ipid,SIGTERM);
  if(retVal==0){
    delN(Ipid);
    printf("Pman: > ");
  }
}

void func_BGstop(char * str_pid){
	//Your code here
  struct Node *nextNode=head;
  int D=0;
  int Ipid=atoi(str_pid);
  int retVal; 
  while(nextNode!=NULL){
      if (nextNode->PID==Ipid){
        retVal = kill(Ipid,SIGSTOP);
        if(retVal==0){
          printf("Pman: > ");
        }
        D=1;
      }
      nextNode=nextNode->next;
    }
  if(D==0){
    printf("wrong pid\n");
    printf("Pman: > ");
  }
}

void func_BGstart(char * str_pid){
	//Your code here
  struct Node *nextNode=head;
  int D=0;
  int Ipid=atoi(str_pid);
  int retVal;
  while(nextNode!=NULL){
      if (nextNode->PID==Ipid){
          retVal = kill(Ipid,SIGCONT);
          if(retVal==0){
            printf("Pman: > ");
          }
        D=1;
      }
      nextNode=nextNode->next;
    }
  if(D==0){
    printf("wrong pid\n");
    printf("Pman: > ");
  }
}

void func_pstat(char * str_pid){
	//Your code here
  char STAT[300];
  char STATUS[300];
  char BUFFER1[300];
  char BUFFER2[300];
  int Ipid=atoi(str_pid);
  struct Node *nextNode=head;
  int D=0;
  while(nextNode!=NULL){
      if (nextNode->PID==Ipid){  
        D=1;
      }
      nextNode=nextNode->next;
    }
  if(D==0){
    printf("wrong pid\n");
    printf("Pman: > ");
  }

  if(D!=0){
    sprintf(STAT,"/proc/%d/stat",Ipid);
    FILE * file1;
    file1=fopen(STAT,"r");
    fgets(BUFFER1,300,(FILE*)file1);  
    const char * split =" ";
    char *p;
    p=strtok(BUFFER1,split);
    int c=0;
    while(p!=NULL) {
      if (c==1){
        printf ("COMM: %s \n",p);
      }
      if (c==2){
        printf ("STATE: %s \n",p);
      }
      if (c==13){
        printf ("UTIME: %s \n",p);
      }
      if (c==14){
        printf ("STIME: %s \n",p);
      }
      if (c==23){
        printf ("RSS: %s \n",p);
      }
      p = strtok(NULL,split);
      c+=1;
    }
    sprintf(STATUS,"/proc/%d/status",Ipid);
    FILE * file2;
    file2=fopen(STATUS,"r");
    while(fgets(BUFFER2,5000,(FILE*)file2)!=NULL){
      if(strstr(BUFFER2,"voluntary_ctxt_switches:")!=NULL){
        printf("%s",BUFFER2);
      }
    }
    printf("Pman: > ");
  }
}
int main(){
    char user_input_str[50];
    printf("Pman: > ");
    while (true) {
      fgets(user_input_str, 50, stdin);
      printf("User input: %s \n", user_input_str);
      char * ptr = strtok(user_input_str, " \n");
      if(ptr == NULL){
        printf("Pman: > ");
        continue;
      }
      char * lst[50];
      int index = 0;
      lst[index] = ptr;
      index++;
      while(ptr != NULL){
        ptr = strtok(NULL, " \n");
        lst[index]=ptr;
        index++;
      }
      if (strcmp("bg",lst[0]) == 0){
        func_BG(lst);
      } else if (strcmp("bglist",lst[0]) == 0) {
        func_BGlist(lst);
      } else if (strcmp("bgkill",lst[0]) == 0) {
        func_BGkill(lst[1]);
      } else if (strcmp("bgstop",lst[0]) == 0) {
        func_BGstop(lst[1]);
      } else if (strcmp("bgstart",lst[0]) == 0) {
        func_BGstart(lst[1]);
      } else if (strcmp("pstat",lst[0]) == 0) {
        func_pstat(lst[1]);
      } else if (strcmp("q",lst[0]) == 0) {
        printf("Bye Bye \n");
        exit(0);
      } else {
        printf("Invalid input\n");
        printf("Pman: > ");
      }
    }
  return 0;
}