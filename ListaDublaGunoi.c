
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct {
	int year;
	char* title;
	int hasOscar;
}Movie;

typedef struct Node {
	Movie* data;
	struct Node* next;
	struct  Node* prev;
}Node;

typedef struct {
	Node* head;
	Node* tail;
}List;

typedef void(*InsertListFn)(List*, Movie*);

void printMovie(const Movie* movie) {
	printf("[%d]%s - Oscar:%s \n",
		movie->year, movie->title, movie->hasOscar ? "Yes" : "No");
}

int loadMovies(const char* filename, List* list, InsertListFn insertList) {
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		printf("Error: could not open file '%s'", filename);
		return -1;
	}

	char line[256];
	int count = 0;

	while (fgets(line, sizeof(line), f) != NULL) {
		Movie* movie = malloc(sizeof(Movie));
		char* token = strtok(line, ",");
		movie->year = atoi(token);

		token = strtok(NULL, ",");
		movie->title = malloc(strlen(token) + 1);
		strcpy(movie->title, token);

		token = strtok(NULL, ",");
		movie->hasOscar = atoi(token);

		insertList(list, movie);
		count++;
	}
	fclose(f);
	return count;
}

void addToBegging(List* list, Movie* movie) {
	Node* newNode = malloc(sizeof(Node));
	newNode->data = movie;
	newNode->prev = NULL;
	newNode->next = list->head;
	
	if (list->head) {
		list->head->prev = newNode;
	}
	else {
		list->tail = newNode;
	}
	list->head = newNode;
}

void addToEnd(List* list, Movie* movie) {
	Node* newNode = malloc(sizeof(Node));
	newNode->data = movie;
	newNode->next = NULL;
	newNode->prev = list->tail;
	if (list->tail) {
		list->tail->next = newNode;
	}
	else {
		list->head = newNode;
	}
	list->tail = newNode;
}

void parseListBtoE(List list) {
	if (list.head) {
		while (list.head) {
			printMovie(list.head->data);
			list.head = list.head->next;
		}
	}
	else {
		printf("List is empty\n");
	}
}

void parseListEtoB(List list) {
	if (list.tail) {
		while (list.tail) {
			printMovie(list.tail->data);
			list.tail = list.tail->prev;
		}
	}
	else {
		printf("List is empty\n");
	}
}



int countByCondition(List list, int (*conditionFn)(Movie*)) {
    int count = 0;
    Node* aux = list.head;

    while (aux) {
        if (conditionFn(aux->data)) {
            count++;
        }
        aux = aux->next;
    }

    return count;
}

int countByCondition(List list, int (*condition)(Movie*)) {
	int count = 0;
	Node* aux = list.head;

	while (aux) {
		if (condition(aux->data)) {
			count++;
		}
		aux = aux->next;
	}
	return count;
}

int isFrom2025(Movie* m) {
	return m->year  == 2025;
}

int isBarbie(Movie* m) {
	return strcmp(m->title, "Barbie") == 0;
}

int isBarbiefrom2023(Movie* m) {
	return (strcmp(m->title, "Barbie") == 0 &m->year==2023);
	
}

void freeList(List* list) {
	Node* current = list->head;
	while (current) {
		Node* next = current->next;
		free(current->data->title);
		free(current->data);
		free(current);
		current = next;
	}
	list->head = NULL;
	list->tail = NULL;
}
int main() {
	List list = { NULL,NULL };

	int count = loadMovies("movie.csv", &list, addToEnd);
	printf("Cate filme sunt: %d\n", count);
	parseListBtoE(list);

	printf("\nCate filme sunt: %d\n", count);
	parseListEtoB(list);
	printf("==========================\n");
	printf("Filme dinn 2025: %d\n", countByCondition(list,isFrom2025));
	printf("Filme cu den Barbie: %d\n", countByCondition(list, isBarbie));
	printf("Filme cu den Barbie from 2023: %d\n", countByCondition(list, isBarbiefrom2023));
	printf("\n==========================\n");
	freeList(&list);
	parseListEtoB(list);
	return 0;
	
}