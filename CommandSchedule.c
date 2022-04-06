/* Program:    Double linked list
   Author:     Robert Drehmann
   Date:       October 8th, 2021
   File name:  CommandSchedule.c
   Compile:    gcc -o CommandSchedule CommandSchedule.c
   Run:        ./CommandSchedule

   This program is able to insert, delete, schedule,
   print, and run jobs from a doubly linked list using
   commaneds from the user.
*/
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>    // time()

// define the struct of nodes
struct JOB {
   char command[5][25];     // domain = { [a-zA-Z][a-zA-Z0-9_\.]* }
   long submissionTime;     // domain = { positive long integer }
   int startTime;           // domain = { positive integer }
   struct JOB *prev;
   struct JOB *next;
};
typedef struct JOB Job;

struct LIST {
   int numOfJobs;        // number of jobs in the list
   Job *firstJob;        // points to first job. NULL if list empty
   Job *lastJob;         // points to last job. NULL if list empty
};
typedef struct LIST List;

// declare function prototypes
//Job *createJob();
void printJob(Job *jobPtr);
List *createList();
//void appendJob(List *listPtr, Job *newPtr);
void printList(List *listPtr);
void insertOrdered(List *listPtr, Job *newPtr);
Job *deleteFirstJob(List *listPtr);
void *scheduler(void *ptr);
void *dispatcher(void *ptr);
int pthread_create(pthread_t *thread,
const pthread_attr_t *attr,
void *(*start_routine)(void*),
void *arg);

void *executer(void *ptr){
  int strcmp (const char* str1, const char* str2);
  Job *nextJob = (Job *)ptr;
  int child_pid;
  int status = 999;
  if ((child_pid=fork()) != 0) {

     waitpid(child_pid, &status, WEXITED);
  }else{

     int k;
     int i = 0;
     char *cmdString = nextJob->command[0];
     char *argv[6];
     while(strcmp(nextJob->command[i], "")!=0) {
        argv[i] = nextJob->command[i];
        i++;
      }
      argv[i] = NULL;
      k = execvp(cmdString, argv);
      printf("this should never be printed--%d--\n", i);
      exit(EXIT_SUCCESS);
  }
  printJob(nextJob);
  free(nextJob);
  return(NULL);
}

void *dispatcher(void *ptr){
  pthread_t thread_id2;
  List *listPtr = (List *)ptr;
  Job *nextJob;
  int true = 1;

  while (true==1) {
   if(listPtr->numOfJobs > 0){
    nextJob = listPtr->firstJob;
    //printf("2");

     int currentSysTime = time(NULL);

     //printf("%ld\n", nextJob->submissionTime);
     if (currentSysTime >= nextJob->submissionTime + nextJob->startTime) {
        //printf("%d\n", currentSysTime);
        nextJob = deleteFirstJob(listPtr);

        pthread_create(&thread_id2, NULL, executer, (void *)nextJob);

  }
}
}
return(NULL);
}
int main() {

  List *listPtr =createList();


      pthread_t thread_id;
       Job *jobPtr;

       char command[5][25];
       int numjobs;
       char symbol;
       int programStart = time(NULL);
       char input[50]; // domain = { [a-zA-Z][a-zA-Z0-9_\.]* }
       char programName[25]; // domain = { [a-zA-Z][a-zA-Z0-9_\.]* }
       int submissionTime;   // domain = { positive integer }
       int startTime;        // domain = { positive integer }
       int true = 1;
       int rc;

       listPtr = createList();


       while (true==1){

          jobPtr = (Job *) malloc(sizeof(Job));
          scanf("%[^\n]%*c",input);

          sscanf(input,"%c %d", &symbol, &numjobs);

          if(numjobs==5){
            sscanf(input,"%c %d %s %s %s %s %s %d",&symbol, &numjobs,
            jobPtr->command[0],
            jobPtr->command[1],
            jobPtr->command[2],
            jobPtr->command[3],
            jobPtr->command[4],
            &jobPtr->startTime);
            jobPtr->submissionTime= time(NULL);
          }else if(numjobs==4){
            sscanf(input,"%c %d %s %s %s %s %d",&symbol, &numjobs,
            jobPtr->command[0],
            jobPtr->command[1],
            jobPtr->command[2],
            jobPtr->command[3],
            &jobPtr->startTime);
            strcpy(jobPtr->command[4], "");
            jobPtr->submissionTime= time(NULL);
          }else if(numjobs==3){
            sscanf(input,"%c %d %s %s %s %d",&symbol, &numjobs,
            jobPtr->command[0],
            jobPtr->command[1],
            jobPtr->command[2],
            &jobPtr->startTime);
            strcpy(jobPtr->command[3], "");
            jobPtr->submissionTime= time(NULL);
          }else if(numjobs==2){
            sscanf(input,"%c %d %s %s %d",&symbol, &numjobs,
            jobPtr->command[0],
            jobPtr->command[1],
            &jobPtr->startTime);
            strcpy(jobPtr->command[2], "");
            jobPtr->submissionTime= time(NULL);
          }else if(numjobs==1){
            sscanf(input,"%c %d %s %d",&symbol, &numjobs,
            jobPtr->command[0],
            &jobPtr->startTime);
            strcpy(jobPtr->command[1], "");
            jobPtr->submissionTime= time(NULL);
          }

          if(symbol == '+'){
             insertOrdered(listPtr, jobPtr);
          }else if(symbol == '-'){
             printf("Job Deleted: \n");
             jobPtr = deleteFirstJob(listPtr);
             printJob(jobPtr);
             free(jobPtr);
          }else if(symbol == 'p'){
             printList(listPtr);
             free(jobPtr);
          }
           pthread_create(&thread_id, NULL, dispatcher, (void *)listPtr);
       }



}



Job *createJob(int submissionTime, int startTime, char programName[20]) {
   Job *jobPtr = (Job *) malloc(sizeof(Job));
   //scanf("%s %d %d", jobPtr->programName, &jobPtr->submissionTime, &jobPtr->startTime);

   return jobPtr;
}

List *createList() {
   List *listPtr = (List*) malloc(sizeof(List));
   listPtr->numOfJobs = 0;
   listPtr->firstJob = NULL;
   listPtr->lastJob = NULL;

   return listPtr;
}
//appeneds Job onto the List
void appendJob(List *listPtr, Job *newPtr) {
  Job *oldptr;
  oldptr = listPtr->lastJob;
   if (listPtr->numOfJobs == 0) {
      // empty list
      newPtr->prev = NULL;
      newPtr->next = NULL;

      listPtr->firstJob = newPtr;
      listPtr->lastJob = newPtr;
   }else {
      // at least one node
      newPtr->next = NULL;
      newPtr->prev = listPtr->lastJob;
      oldptr->next = newPtr;
      listPtr->lastJob = newPtr;
   }
   listPtr->numOfJobs++;
}

void printList(List *listPtr) {
   if (listPtr->numOfJobs == 0) {
      printf("Empty list!\n");
   }else{
      printf("\n# of Jobs %d\n", listPtr->numOfJobs);
      Job *ptr = listPtr->firstJob;
      for (int i = 0; i < listPtr->numOfJobs; i++) {
         printf("Job %d:\n", i+1);
         printJob(ptr);
         ptr = ptr->next;
      }
    /*  printf("\nReverse Order \n");
      ptr = listPtr->lastJob;
      for (int i = 0; i < listPtr->numOfJobs; i++) {
         printf("Job %d:\n", i+1);
         printJob(ptr);
         ptr = ptr->prev;
      } */

   }
}
void insertOrdered(List *listPtr, Job *newPtr){
   Job *oldptr;
   oldptr = listPtr->lastJob;
   Job *ptr = listPtr->firstJob;
   if(listPtr->numOfJobs == 0) {
      // empty list
      newPtr->prev = NULL;
      newPtr->next = NULL;
      listPtr->firstJob = newPtr;
      listPtr->lastJob = newPtr;
   }else if((oldptr->submissionTime+oldptr->startTime) >
   (newPtr->submissionTime+newPtr->startTime)){
      //insert before node
      for (int i = 0; i < listPtr->numOfJobs; i++) {
         if((ptr->submissionTime+ptr->startTime) >
         (newPtr->submissionTime+newPtr->startTime)){
            newPtr->next = ptr;
            newPtr->prev = ptr->prev;
            if(ptr == listPtr->firstJob){
              listPtr->firstJob = newPtr;
              ptr->prev = newPtr;
              i = listPtr->numOfJobs;
            }else{
              ptr->prev->next = newPtr;
              ptr->prev = newPtr;
              i = listPtr->numOfJobs;
            }
         }
         ptr = ptr->next;
      }
   }else{
      newPtr->next = NULL;
      newPtr->prev = listPtr->lastJob;
      listPtr->lastJob->next=newPtr;
      listPtr->lastJob = newPtr;
   }
   listPtr->numOfJobs++;
}

Job *deleteFirstJob(List *listPtr){

  if(listPtr->numOfJobs > 1){
      Job *firstJob = listPtr->firstJob;
      listPtr->firstJob->next->prev = NULL;
      listPtr->firstJob = listPtr->firstJob->next;
      listPtr->numOfJobs--;

      return firstJob;
  }else if(listPtr->numOfJobs == 1){
      Job *firstJob = listPtr->firstJob;
      listPtr->lastJob = NULL;
      listPtr->firstJob = NULL;
      listPtr->numOfJobs--;

      return firstJob;
  }else{

      return NULL;
  }

}
void printJob(Job *jobPtr) {
  if(jobPtr == NULL){
      printf("NULL\n");
  }else{
    printf("program name:");
      for (int i = 0; i < 5; i++) {

        printf(" %s", jobPtr->command[i]);
      }

      printf("\nsubmission time: %ld\n", jobPtr->submissionTime);
      printf("start time: %d\n", jobPtr->startTime);
  }

}
