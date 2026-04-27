#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	int number;
	char* driver;
	char* team;
	float engine_hp;
} F1Car;

//================ NODURI =================

// similar cu Node din lista simpla
typedef struct NodeStack {
	F1Car* data;
	struct NodeStack* next;
} NodeStack;

typedef struct NodeQueue {
	F1Car* data;
	struct NodeQueue* next;
} NodeQueue;

//================ CREARE OBIECT =================

// similar cu ce faci in loadCars (alocare + copiere)
F1Car* createCar(int number, char* driver, char* team, float hp) {
	F1Car* car = (F1Car*)malloc(sizeof(F1Car));
	car->number = number;

	car->driver = (char*)malloc(strlen(driver) + 1);
	strcpy(car->driver, driver);

	car->team = (char*)malloc(strlen(team) + 1);
	strcpy(car->team, team);

	car->engine_hp = hp;

	return car;
}

//================ STACK =================

// similar cu addToBeginning -> push
void push(NodeStack** stack, F1Car* car) {

	NodeStack* newNode = (NodeStack*)malloc(sizeof(NodeStack));
	newNode->data = car;

	// IDENTIC ca la lista: inserare la inceput
	newNode->next = *stack;
	*stack = newNode;
}

// similar cu deleteFromBeginning -> pop
int pop(NodeStack** stack, F1Car** car) {

	if (*stack == NULL) {
		return -1;
	}

	NodeStack* temp = *stack;
	*car = temp->data;

	*stack = (*stack)->next;

	free(temp);
	return 0;
}

// similar cu parseList
void printStack(NodeStack* stack) {
	while (stack) {
		printf("Car #%d - %s from %s - HP: %.1f\n",
			stack->data->number,
			stack->data->driver,
			stack->data->team,
			stack->data->engine_hp);

		stack = stack->next;
	}
}

//================ QUEUE =================

// similar cu addToEnd (dar avem si ultim)
void enqueue(NodeQueue** first, NodeQueue** last, F1Car* car) {

	NodeQueue* newNode = (NodeQueue*)malloc(sizeof(NodeQueue));
	newNode->data = car;
	newNode->next = NULL;

	// similar cu lista goala
	if (*first == NULL) {
		*first = *last = newNode;
	}
	else {
		(*last)->next = newNode;
		*last = newNode;
	}
}

// similar cu deleteFromBeginning
int dequeue(NodeQueue** first, NodeQueue** last, F1Car** car) {

	if (*first == NULL) {
		return -1;
	}

	NodeQueue* temp = *first;
	*car = temp->data;

	*first = (*first)->next;

	if (*first == NULL) {
		*last = NULL;
	}

	free(temp);
	return 0;
}

// similar cu parseList
void printQueue(NodeQueue* first) {
	while (first) {
		printf("Car #%d - %s from %s - HP: %.1f\n",
			first->data->number,
			first->data->driver,
			first->data->team,
			first->data->engine_hp);

		first = first->next;
	}
}

//================ CITIRE CSV =================

// IDENTIC concept cu loadCars din lista simpla
int loadStackFromCSV(const char* filename, NodeStack** stack) {

	FILE* f = fopen(filename, "r");
	if (!f) {
		printf("Eroare fisier\n");
		return -1;
	}

	char line[256];
	int count = 0;

	while (fgets(line, sizeof(line), f)) {

		char* token = strtok(line, ",");
		int number = atoi(token);

		token = strtok(NULL, ",");
		char driver[100];
		strcpy(driver, token);

		token = strtok(NULL, ",");
		char team[100];
		strcpy(team, token);

		token = strtok(NULL, ",");
		float hp = atof(token);

		F1Car* car = createCar(number, driver, team, hp);

		// IMPORTANT: ca la lista -> alegem functia de inserare
		push(stack, car);

		count++;
	}

	fclose(f);
	return count;
}

//================ FREE =================

// similar cu freeList
void freeCar(F1Car* car) {
	free(car->driver);
	free(car->team);
	free(car);
}

void freeStack(NodeStack** stack) {
	F1Car* car;
	while (pop(stack, &car) == 0) {
		freeCar(car);
	}
}

void freeQueue(NodeQueue** first, NodeQueue** last) {
	F1Car* car;
	while (dequeue(first, last, &car) == 0) {
		freeCar(car);
	}
}

//================ CONVERSIE =================

// EXTRA (ca la profesorul 2)
void stackToQueue(NodeStack** stack, NodeQueue** first, NodeQueue** last) {

	F1Car* car;

	// similar cu parcurgere + mutare
	while (pop(stack, &car) == 0) {
		enqueue(first, last, car);
	}
}

//================ MAIN =================

int main() {

	NodeStack* stack = NULL;
	NodeQueue* first = NULL, * last = NULL;

	int count = loadStackFromCSV("cars.csv", &stack);

	printf("\n--- STACK ---\n");
	printStack(stack);

	// conversie
	stackToQueue(&stack, &first, &last);

	printf("\n--- QUEUE ---\n");
	printQueue(first);

	freeQueue(&first, &last);

	return 0;
}