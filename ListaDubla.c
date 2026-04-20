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
	struct Node* prev;
}Node;

typedef struct {
	Node* head;
	Node* tail;
}List;

typedef void (*InsertListFn)(List*, Movie*);

void printMovie(const Movie* movie) {
	printf("[%d] %s-Oscar:%s\n",
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
			list.tail = list.tail->prev;
		}
	}
	else {
		printf("List is empty\n");
	}
}

Movie deleteFromBeginning(List* list) {

	if (list->head) {

		Node* toBeDeleted = list->head;
		Movie deletedMovie =*( toBeDeleted->data);

		if (list->head->next) {
			list->head = list->head->next;
			list->head->prev = NULL;

			free(toBeDeleted);
			return deletedMovie;
		}
		else {
			free(toBeDeleted);
			list->head = NULL;
			list->tail = NULL;

			return deletedMovie;
		}

	}
	else {
		printf("List is empty\n");
		Movie err = { .year = 0, .title = "ERR", .hasOscar = 0 };
		return err;
	}


}

Movie deletedFromEnd(List* list) {

	if (list->tail) {

		Node* toBeDeleted = list->tail;
		Movie deletedMovie = *(toBeDeleted->data);

		if (list->tail->prev) {
			list->tail = list->tail->prev;
			list->tail->next = NULL;

			free(toBeDeleted);
			return deletedMovie;
		}
		else {
			free(toBeDeleted);
			list->tail = NULL;
			list->head = NULL;

			return deletedMovie;
		}

	}
	else {
		printf("List is empty\n");
		Movie err = { .year = 0, .title = "ERR", .hasOscar = 0 };
		return err;
	}

}

void deleteByCondition(List* list, int (*conditionFn)(Movie*)) {
	if (list->head) {
		Node* aux = list->head;

		while (aux) {
			if (conditionFn(aux->data)) {
				Node* toDelete = aux;

				// 1. Gestion?m leg?tura cu nodul anterior
				if (aux->prev) {
					aux->prev->next = aux->next;
				}
				else {
					// Dac? nu are anterior, înseamn? c? e capul listei
					list->head = aux->next;
				}

				// 2. Gestion?m leg?tura cu nodul urm?tor
				if (aux->next) {
					aux->next->prev = aux->prev;
				}
				else {
					// Dac? nu are urm?tor, înseamn? c? e coada listei
					list->tail = aux->prev;
				}

				// Trecem la urm?torul nod înainte de a ?terge
				aux = aux->next;

				// 3. Eliber?m memoria (Maniera profesorului: eliber?m câmpurile din structur? + structura + nodul)
				free(toDelete->data->title);
				free(toDelete->data);
				free(toDelete);
			}
			else {
				// Dac? nu îndepline?te condi?ia, mergem pur ?i simplu mai departe
				aux = aux->next;
			}
		}
	}
	else {
		printf("List is empty\n");
	}
}

/*Movie readMovieFromFile(FILE* f) {

	char line[256];

	fgets(line, 256, f);

	Movie movie;

	char* token = strtok(line, ",");
	movie.year = atoi(token);

	token = strtok(NULL, ",");
	movie.title = malloc((strlen(token) + 1));
	strcpy(movie.title, token);

	token = strtok(NULL, ",");
	movie.hasOscar = atoi(token);

	return movie;

}*/

int hasOscarCondition(Movie* m) {
	return m->hasOscar == 1;
}

int isNewMovie(Movie* m) {
	return m->year >2023;
}

void freeList(List* list) {
	Node* current = list->head;
	while (current != NULL) {
		// Salv?m referin?a c?tre urm?torul nod înainte de a-l ?terge pe cel curent
		Node* nextNode = current->next;

		// 1. Eliber?m titlul (char* title)
		if (current->data->title != NULL) {
			free(current->data->title);
		}

		// 2. Eliber?m structura Movie
		free(current->data);

		// 3. Eliber?m nodul propriu-zis
		free(current);

		// Trecem la urm?torul
		current = nextNode;
	}

	// Reset?m pointerii listei la NULL pentru a evita "dangling pointers"
	list->head = NULL;
	list->tail = NULL;
}





//cerinta test count elemente conditie

int countMoviesByCondition(List list, int (*conditionFn)(Movie*)) {
	int count = 0;
	Node* aux = list.head; // Pornim de la începutul listei

	while (aux != NULL) {
		if (conditionFn(aux->data)) {
			count++;
		}
		aux = aux->next;
	}

	return count;
}

// Condi?ie: Filme lansate dup? un anumit an (X)
// Nota: Deoarece semn?tura trebuie s? fie int (*fn)(Movie*), 
// dac? vrem un an variabil, putem folosi o variabil? global? sau 
// facem func?ii specifice.

int isMovieAfter2010(Movie* m) {
	return m->year > 2010;
}

int hasNoOscar(Movie* m) {
	return m->hasOscar == 0;
}



int main() {
	List list = { NULL,NULL };
	int count = loadMovies("movie.csv", &list, addToEnd);
	/*Movie film=readMovieFromFile("movie.csv");
	pintf(film);*/


	if (count <0) {
		printf("Failed to load file\n");
		return 1;
	}
	printf("================parseListBtoE============================");
	printf("Loaded %d movie.\n", count);
	parseListBtoE(list);

	//printf("================parseListBtoE============================");
	//printf("Loaded %d movie.\n", count);
	//parseListEtoB(list);
	printf("================deleetFRomBeggining============================");
	
	printf("-------------------\n");
	//Movie deletedMovie = deleteFromBeginning(&list);
	//printf("Deleted movie: \n");
	//printMovie(&deletedMovie);

	deleteFromBeginning(&list);
	deletedFromEnd(&list);
	printf("Loaded %d movie.\n", count);
	parseListBtoE(list);
	
	// Exemplu de apel
	
	printf("\nStergem filmele cu Oscar:\n");
	deleteByCondition(&list, hasOscarCondition);
	parseListBtoE(list);


	parseListBtoE(list);

	// Exemplu de apel
	printf("\nStergem filmele mai noi de 2023:\n");
	deleteByCondition(&list, isNewMovie);
	parseListBtoE(list);


	freeList(&list);
	parseListBtoE(list);



	///cerinta test

	printf("\n\n=================\n\n");
	int loaded = loadMovies("movie.csv", &list, addToEnd);

	if (loaded > 0) {
		// 1. Num?r?m filmele care au Oscar
		int oscarCount = countMoviesByCondition(list, hasOscarCondition);
		printf("\nNumar de filme cu Oscar: %d\n", oscarCount);

		// 2. Num?r?m filmele lansate dup? 2010
		int recentCount = countMoviesByCondition(list, isMovieAfter2010);
		printf("Numar de filme dupa 2010: %d\n", recentCount);

		// 3. Num?r?m filmele fara Oscar
		int noOscarCount = countMoviesByCondition(list, hasNoOscar);
		printf("Numar de filme fara Oscar: %d\n", noOscarCount);
	}

	// Cur???m memoria la final
	freeList(&list);
	
	return 0;

}