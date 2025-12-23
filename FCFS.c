#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_NAME 100
#define HASH_SIZE 1000
#define STATE_READY 0
#define STATE_RUNNING 1
#define STATE_WAITING 2
#define STATE_TERMINATED 3
#define STATE_KILLED 4

struct process {
  int processId;
  char processName[MAX_NAME];

  int totalBurstTime;
  int remainingBurstTime;

  int ioStartTime;
  int ioDuration;
  int ioRemainingTime;

  int arrivalTime;
  int completionTime;

  int killAt;
  int state;

  struct process *next;
};

struct queue {
  struct process *front;
  struct process *rear;
};

struct process *hashMap[HASH_SIZE];

struct queue *createQueue() {
  struct queue *queue = malloc(sizeof(struct queue));
  queue->front = queue->rear = NULL;
  return queue;
}
void enqueue(struct queue *queue, struct process *process) {
  process->next = NULL;
  if (!queue->rear) {
    queue->front = queue->rear = process;
    return;
  }
  queue->rear->next = process;
  queue->rear = process;
}

struct process *dequeue(struct queue *queue) {
  if (!queue->front)
    return NULL;
  struct process *temp = queue->front;
  queue->front = queue->front->next;
  if (!queue->front)
    queue->rear = NULL;
  return temp;
}

int isEmpty(struct queue *queue) { 
    return queue->front == NULL; 
}

void addToHash(struct process *process) { 
    hashMap[process->processId] = process; 
}

struct process *getFromHash(int processId) {
  if (processId < 0 || processId >= HASH_SIZE)
    return NULL;
  return hashMap[processId];
}

void removeFromQueue(struct queue *queue, struct process *process) {
  if (!queue->front)
    return;

  struct process *curr = queue->front;
  struct process *prev = NULL;

  while (curr) {
    if (curr == process) {
      if (prev == NULL) {
        queue->front = curr->next;
      } else {
        prev->next = curr->next;
      }
      if (curr == queue->rear)
        queue->rear = prev;
      return;
    }
    prev = curr;
    curr = curr->next;
  }
} 

void createProcess(char *name, int pid, int burst, int ioStart, int ioDuration, struct queue *readyQueue) {
  struct process *p = malloc(sizeof(struct process));

  strcpy(p->processName, name);
  p->processId = pid;
  p->totalBurstTime = burst;
  p->remainingBurstTime = burst;

  p->ioStartTime = ioStart;
  p->ioDuration = ioDuration;
  p->ioRemainingTime = ioDuration;

  p->arrivalTime = 0;
  p->completionTime = -1;

  p->killAt = -1;
  p->state = STATE_READY;

  p->next = NULL;

  addToHash(p);
  enqueue(readyQueue, p);
}

void scheduleKill(int pid, int time) {
  struct process *p = getFromHash(pid);
  if (!p) {
    printf("PID %d does not exist!\n", pid);
    return;
  }
  p->killAt = time;
  printf("Scheduled KILL %d at time %d\n", pid, time);
}

void runSimulation(struct queue *readyQueue, struct queue *waitingQueue, struct queue *terminatedQueue) {
  printf("\n--- STARTING SIMULATION ---\n");
  int time = 0;
  struct process *running = NULL;

  while (!isEmpty(readyQueue) || !isEmpty(waitingQueue) || running != NULL) {

    for (int pid = 0; pid < HASH_SIZE; pid++) {
      struct process *p = hashMap[pid];
      if (!p)
        continue;

      if (p->killAt == time && p->state != STATE_TERMINATED) {
        printf("KILLED Process %d at time %d\n", p->processId, time);

        if (running == p)
          running = NULL;

        removeFromQueue(readyQueue, p);
        removeFromQueue(waitingQueue, p);

        p->state = STATE_KILLED;
        p->completionTime = time;
        enqueue(terminatedQueue, p);
      }
    }

    int count = 0;
    struct process *tempList[500];

    while (!isEmpty(waitingQueue)) {
      tempList[count++] = dequeue(waitingQueue);
    }

    for (int i = 0; i < count; i++) {
      struct process *p = tempList[i];

      if (p->state == STATE_KILLED)
        continue;

      p->ioRemainingTime--;

      if (p->ioRemainingTime == 0) {
        p->state = STATE_READY;
        enqueue(readyQueue, p);
        p->ioRemainingTime = p->ioDuration;
      } else {
        enqueue(waitingQueue, p);
      }
    }

    if (!running) {
      running = dequeue(readyQueue);
      if (running)
        running->state = STATE_RUNNING;
    }

    if (running) {
      running->remainingBurstTime--;

      int executed = running->totalBurstTime - running->remainingBurstTime;

      if (running->ioDuration > 0 && executed == running->ioStartTime) {

        running->state = STATE_WAITING;
        enqueue(waitingQueue, running);
        running = NULL;
      }

      else if (running->remainingBurstTime == 0) {
        running->state = STATE_TERMINATED;
        running->completionTime = time + 1;
        enqueue(terminatedQueue, running);
        running = NULL;
      }
    }

    time++;
    Sleep(1000);
  }

  printf("\n--- SIMULATION COMPLETED ---\n");
  printf("%-5s %-10s %-5s %-5s %-12s %-10s\n", "PID", "Name", "CPU", "IO",
         "Turnaround", "Waiting");

  while (!isEmpty(terminatedQueue)) {
    struct process *p = dequeue(terminatedQueue);

    if (p->state == STATE_KILLED) {
      printf("%-5d %-10s %-5d %-5d %-12s %-10s\n", p->processId, p->processName,
             p->totalBurstTime, p->ioDuration, "-", "-");
      continue;
    }

    int turnaround = p->completionTime - p->arrivalTime;
    int waiting = turnaround - p->totalBurstTime - p->ioDuration;

    printf("%-5d %-10s %-5d %-5d %-12d %-10d\n", p->processId, p->processName,
           p->totalBurstTime, p->ioDuration, turnaround, waiting);
  }
}

int main() {
  char input[200], name[100];
  int processId, cpu, ioS, ioD, killTime;
  struct queue *readyQueue, *waitingQueue, *terminatedQueue;

  readyQueue = createQueue();
  waitingQueue = createQueue();
  terminatedQueue = createQueue();

  printf("<name> <pid> <cpu> <io_start> <io_duration>\n");
  printf("KILL <pid> <time>\n");
  printf("RUN to begin\n");

  while (1) {
    printf("> ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "RUN") == 0) {
      runSimulation(readyQueue, waitingQueue, terminatedQueue);
      break;
    }

    if (sscanf(input, "KILL %d %d", &processId, &killTime) == 2) {
      scheduleKill(processId, killTime);
      continue;
    }

    if (sscanf(input, "%s %d %d %d %d", name, &processId, &cpu, &ioS, &ioD) == 5) {
      createProcess(name, processId, cpu, ioS, ioD, readyQueue);
      continue;
    }

    printf("Invalid input!\n");
  }

  return 0;
}
