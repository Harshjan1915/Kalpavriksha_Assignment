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
  int pid;
  char name[MAX_NAME];

  int totalBurst;
  int remainingBurst;

  int ioStart;
  int ioDuration;
  int ioRemaining;

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

struct queue *readyQ, *waitQ, *termQ;

struct process *hashMap[HASH_SIZE];

struct queue *createQueue() {
  struct queue *q = malloc(sizeof(struct queue));
  q->front = q->rear = NULL;
  return q;
}

void enqueue(struct queue *q, struct process *p) {
  p->next = NULL;
  if (!q->rear) {
    q->front = q->rear = p;
    return;
  }
  q->rear->next = p;
  q->rear = p;
}

struct process *dequeue(struct queue *q) {
  if (!q->front)
    return NULL;
  struct process *temp = q->front;
  q->front = q->front->next;
  if (!q->front)
    q->rear = NULL;
  return temp;
}

int isEmpty(struct queue *q) { 
    return q->front == NULL; 
}

void addToHash(struct process *p) { 
    hashMap[p->pid] = p; 
}

struct process *getFromHash(int pid) {
  if (pid < 0 || pid >= HASH_SIZE)
    return NULL;
  return hashMap[pid];
}

void removeFromQueue(struct queue *q, struct process *p) {
  if (!q->front)
    return;

  struct process *curr = q->front;
  struct process *prev = NULL;

  while (curr) {
    if (curr == p) {
      if (prev == NULL) {
        q->front = curr->next;
      } else {
        prev->next = curr->next;
      }
      if (curr == q->rear)
        q->rear = prev;
      return;
    }
    prev = curr;
    curr = curr->next;
  }
}

void createProcess(char *name, int pid, int burst, int ioStart, int ioDur) {
  struct process *p = malloc(sizeof(struct process));

  strcpy(p->name, name);
  p->pid = pid;
  p->totalBurst = burst;
  p->remainingBurst = burst;

  p->ioStart = ioStart;
  p->ioDuration = ioDur;
  p->ioRemaining = ioDur;

  p->arrivalTime = 0;
  p->completionTime = -1;

  p->killAt = -1;
  p->state = STATE_READY;

  p->next = NULL;

  addToHash(p);
  enqueue(readyQ, p);
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

void runSimulation() {
  printf("\n--- STARTING SIMULATION ---\n");
  int time = 0;
  struct process *running = NULL;

  while (!isEmpty(readyQ) || !isEmpty(waitQ) || running != NULL) {

    for (int pid = 0; pid < HASH_SIZE; pid++) {
      struct process *p = hashMap[pid];
      if (!p)
        continue;

      if (p->killAt == time && p->state != STATE_TERMINATED) {
        printf("KILLED Process %d at time %d\n", p->pid, time);

        if (running == p)
          running = NULL;

        removeFromQueue(readyQ, p);
        removeFromQueue(waitQ, p);

        p->state = STATE_KILLED;
        p->completionTime = time;
        enqueue(termQ, p);
      }
    }

    int count = 0;
    struct process *tempList[500];

    while (!isEmpty(waitQ)) {
      tempList[count++] = dequeue(waitQ);
    }

    for (int i = 0; i < count; i++) {
      struct process *p = tempList[i];

      if (p->state == STATE_KILLED)
        continue;

      p->ioRemaining--;

      if (p->ioRemaining == 0) {
        p->state = STATE_READY;
        enqueue(readyQ, p);
        p->ioRemaining = p->ioDuration;
      } else {
        enqueue(waitQ, p);
      }
    }

    if (!running) {
      running = dequeue(readyQ);
      if (running)
        running->state = STATE_RUNNING;
    }

    if (running) {
      running->remainingBurst--;

      int executed = running->totalBurst - running->remainingBurst;

      if (running->ioDuration > 0 && executed == running->ioStart) {

        running->state = STATE_WAITING;
        enqueue(waitQ, running);
        running = NULL;
      }

      else if (running->remainingBurst == 0) {
        running->state = STATE_TERMINATED;
        running->completionTime = time + 1;
        enqueue(termQ, running);
        running = NULL;
      }
    }

    time++;
    Sleep(1000);
  }

  printf("\n--- SIMULATION COMPLETED ---\n");
  printf("%-5s %-10s %-5s %-5s %-12s %-10s\n", "PID", "Name", "CPU", "IO",
         "Turnaround", "Waiting");

  while (!isEmpty(termQ)) {
    struct process *p = dequeue(termQ);

    if (p->state == STATE_KILLED) {
      printf("%-5d %-10s %-5d %-5d %-12s %-10s\n", p->pid, p->name,
             p->totalBurst, p->ioDuration, "-", "-");
      continue;
    }

    int turnaround = p->completionTime - p->arrivalTime;
    int waiting = turnaround - p->totalBurst - p->ioDuration;

    printf("%-5d %-10s %-5d %-5d %-12d %-10d\n", p->pid, p->name, p->totalBurst,
           p->ioDuration, turnaround, waiting);
  }
}

int main() {
  char input[200], name[100];
  int pid, cpu, ioS, ioD, killT;

  readyQ = createQueue();
  waitQ = createQueue();
  termQ = createQueue();

  printf("<name> <pid> <cpu> <io_start> <io_duration>\n");
  printf("KILL <pid> <time>\n");
  printf("RUN to begin\n");

  while (1) {
    printf("> ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "RUN") == 0) {
      runSimulation();
      break;
    }

    if (sscanf(input, "KILL %d %d", &pid, &killT) == 2) {
      scheduleKill(pid, killT);
      continue;
    }

    if (sscanf(input, "%s %d %d %d %d", name, &pid, &cpu, &ioS, &ioD) == 5) {
      createProcess(name, pid, cpu, ioS, ioD);
      continue;
    }

    printf("Invalid input!\n");
  }

  return 0;
}
