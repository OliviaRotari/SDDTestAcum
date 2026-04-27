#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<malloc.h>
#include<math.h>


typedef struct {
	int year;
	char* title;
	int hasOscar;
}Movie;

typedef struct Node {
	Movie* data;
	struct Node* next;
	struct Node* prev;
}Node;

typedef struct {
	Node* head;
	Node* tail;
}List;

typedef void (*InsertListFn)(List*, Movie*);

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


void printMovie(const Movie* movie) {
	printf("[%d] %s-Oscar:%s\n",
		movie->year, movie->title, movie->hasOscar ? "Yes" : "No");
}

void addToBeggining(List* list, Movie* movie) {
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
			list.tail = list.tail ->prev;
		}
	}
	else {
		printf("List is empty\n");
	}
}
//
//void parseList(List* list) {
//	if (list->head) {
//		Node* current = list->head;
//		while (current) {
//			printMovie(current->data);
//			current = current->next;
//		}
//	}
//	else {
//		printf("List is empty\n");
//	}
//}

void freeNode(Node* node) {
	free(node->data->title);
	free(node->data);
	free(node);
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


int hasOscarCondition(Movie* m) {
	return m->hasOscar == 1;
}

int isNewMovie(Movie* m) {
	return m->year > 2023;
}

int isMovieAfter2010(Movie* m) {
	return m->year > 2010;
}

int hasNoOscar(Movie* m) {
	return m->hasOscar == 0;
}

int countMoviesByCondition(List list, int (*conditionFn)(Movie*)) {
	int count = 0;
	Node* aux = list.head;
	while (aux != NULL) {
		if (conditionFn(aux->data)) {
			count++;
		}
		aux = aux->next;
	}
	return count;
}

void deleteBeginning(List* list) {
	if (list->head) {
		Node* toDelete = list->head;
		list->head = list->head->next;

		if (list->head) {
			list->head->prev = NULL;
		}
		else {
			list->tail = NULL;
		}

		free(toDelete->data->title);
		free(toDelete->data);
		free(toDelete);
	}
	else {
		printf("List is empty\n");
	}
}

void deleteEnd(List* list) {
	if (list->tail) {
		Node* toDelete = list->tail;
		list->tail = list->tail->prev;

		if (list->tail) {
			list->tail->next = NULL;
		}
		else {
			list->head = NULL;
		}

		free(toDelete->data->title);
		free(toDelete->data);
		free(toDelete);
	}
	else {
		printf("List is empty\n");
	}
}
void deleteByCondition(List* list, int (*conditionFn)(Movie*)) {
	if(list->head){
		Node* current = list->head;
		while (current)
		{
			if (conditionFn(current->data)) {
				Node* toDelete = current;
				current = current->next;
				if (toDelete->prev && toDelete->next) {
					toDelete->next->prev = toDelete->prev;
					toDelete->prev->next = toDelete->next;
					freeNode(toDelete);
				}
				else if (toDelete->prev) {
					list->tail = toDelete->prev;
					list->tail->next = NULL;
					freeNode(toDelete);
				}
				else if (toDelete->next) {
					list->head = toDelete->next;
					list->head->prev = NULL;
					freeNode(toDelete);
				}
				else {
					list->head = NULL;
					list->tail = NULL;
					freeNode(toDelete);
				}
			}
			else {
				current = current->next;
			}
		}
	}
	else {
		printf("List is empty\n");
	}
}

int main() {
	List list = { NULL,NULL };
	int count = loadMovies("movies.csv", &list, addToEnd);
	//int count = loadMovies("movies.csv", &list, addToBeginnig);
	/*Movie film=readMovieFromFile("movie.csv");
	pintf(film);*/


	if (count < 0) {
		printf("Failed to load file\n");
		return 1;
	}
	/*printf("================parseListBtoE============================");
	printf("Loaded %d movie.\n", count);
	parseListBtoE(list);*/
	printf("================parseListEtoB============================");
	printf("Loaded %d movie.\n", count);
	parseListEtoB(list);
	/*printf("================parseList============================");
	printf("Loaded %d movie.\n", count);
	parseList(&list);*/



	
	printf("\n Nr. Filmele dupa 2023 (new ):%d\n", countMoviesByCondition(list, isNewMovie));
	printf("\n Nr. Filmele cu oscar:%d\n", countMoviesByCondition(list, hasOscarCondition));
	
	printf("================deleteBegging and end============================\n");
	deleteBeginning(&list);
	deleteEnd(&list);
	parseListEtoB(list);
	printf("================deleteBegging movies with oscar============================\n");
	deleteByCondition(&list, hasOscarCondition);
	parseListEtoB(list);

	
	freeList(&list);
	parseListBtoE(list);
}