#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int compareIntegers(const void *firstElement, const void *secondElement) {
  return (*(int *)firstElement - *(int *)secondElement);
}

void printArray(int *numbers, int arraySize) {
  for (int i = 0; i < arraySize; i++) {
    printf("%d\t", numbers[i]);
  }
  printf("\n");
}

void executeChildProcess(int *sharedMemory, int arraySize) {
  qsort(sharedMemory, arraySize, sizeof(int), compareIntegers);
  if (shmdt(sharedMemory) == -1) {
    perror("shmdt child failed");
  }
  exit(0);
}

void executeParentProcess(int *sharedMemory, int arraySize) {
  wait(NULL);
  printf("After sorting:\n");
  printArray(sharedMemory, arraySize);

  if (shmdt(sharedMemory) == -1) {
    perror("shmdt parent failed");
  }
}

int main() {
  key_t key;
  int sharedMemoryId;
  int *sharedMemory;
  int arraySize;

  if (scanf("%d", &arraySize) != 1)
    return 1;

  key = ftok("data", 77);
  if (key == -1) {
    perror("ftok failed");
  }

  sharedMemoryId = shmget(key, sizeof(int) * arraySize, 0666 | IPC_CREAT);
  if (sharedMemoryId == -1) {
    perror("shmget failed");
    return 1;
  }

  sharedMemory = (int *)shmat(sharedMemoryId, NULL, 0);
  if (sharedMemory == (void *)-1) {
    perror("shmat failed");
    return 1;
  }

  for (int i = 0; i < arraySize; i++) {
    scanf("%d", &sharedMemory[i]);
  }

  printf("Before sorting:\n");
  printArray(sharedMemory, arraySize);

  int processId = fork();

  if (processId < 0) {
    perror("Fork failed");
    return 1;
  } else if (processId == 0) {
    executeChildProcess(sharedMemory, arraySize);
  } else {
    executeParentProcess(sharedMemory, arraySize);
    shmctl(sharedMemoryId, IPC_RMID, NULL);
  }
  return 0;
}
