#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//==================== STRUCTURI ====================

typedef struct {
	char* denumire;
	float suprafata;
	unsigned char nrAngajati;
	char* adresa;
	char* lant; // pt cerinta 4 (lant comercial)
}Farmacie;

typedef struct Node {
	Farmacie* data;
	struct Node* next;
	struct Node* prev;
}Node;

typedef struct {
	Node* head;
	Node* tail;
}List;

//==================== FUNCTII AFISARE ====================

void printFarmacie(const Farmacie* f) {
	printf("Denumire: %s | Suprafata: %.2f | Angajati: %d | Adresa: %s | Lant: %s\n",
		f->denumire, f->suprafata, f->nrAngajati, f->adresa, f->lant);
}

//==================== INSERARE ====================

void addToEnd(List* list, Farmacie* f) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = f;
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

//==================== PARCURGERE ====================

void parseListBtoE(List list) {
	Node* aux = list.head;
	while (aux) {
		printFarmacie(aux->data);
		aux = aux->next;
	}
}

//==================== CERINTA 2 ====================

int countFarmaciiSuprafata(List list, float prag) {
	int count = 0;
	Node* aux = list.head;

	while (aux) {
		if (aux->data->suprafata < prag) {
			count++;
		}
		aux = aux->next;
	}
	return count;
}

//==================== CERINTA 3 ====================

int conditieAngajati(Farmacie* f, unsigned char prag) {
	return f->nrAngajati > prag;
}

void deleteByAngajati(List* list, unsigned char prag) {
	Node* aux = list->head;

	while (aux) {
		if (conditieAngajati(aux->data, prag)) {

			Node* toDelete = aux;

			if (aux->prev)
				aux->prev->next = aux->next;
			else
				list->head = aux->next;

			if (aux->next)
				aux->next->prev = aux->prev;
			else
				list->tail = aux->prev;

			aux = aux->next;

			free(toDelete->data->denumire);
			free(toDelete->data->adresa);
			free(toDelete->data->lant);
			free(toDelete->data);
			free(toDelete);
		}
		else {
			aux = aux->next;
		}
	}
}

//==================== CERINTA 4 ====================

Farmacie* copyFarmacie(Farmacie* f) {
	Farmacie* copie = (Farmacie*)malloc(sizeof(Farmacie));

	copie->denumire = (char*)malloc(strlen(f->denumire) + 1);
	strcpy(copie->denumire, f->denumire);

	copie->adresa = (char*)malloc(strlen(f->adresa) + 1);
	strcpy(copie->adresa, f->adresa);

	copie->lant = (char*)malloc(strlen(f->lant) + 1);
	strcpy(copie->lant, f->lant);

	copie->suprafata = f->suprafata;
	copie->nrAngajati = f->nrAngajati;

	return copie;
}

Farmacie** vectorFarmaciiLant(List list, char* lantCautat, int* dim) {
	*dim = 0;

	Node* aux = list.head;

	// determinare dimensiune
	while (aux) {
		if (strcmp(aux->data->lant, lantCautat) == 0) {
			(*dim)++;
		}
		aux = aux->next;
	}

	Farmacie** vect = (Farmacie**)malloc(sizeof(Farmacie*) * (*dim));

	aux = list.head;
	int i = 0;

	while (aux) {
		if (strcmp(aux->data->lant, lantCautat) == 0) {
			vect[i++] = copyFarmacie(aux->data);
		}
		aux = aux->next;
	}

	return vect;
}

//==================== FREE ====================

void freeList(List* list) {
	Node* current = list->head;

	while (current) {
		Node* next = current->next;

		free(current->data->denumire);
		free(current->data->adresa);
		free(current->data->lant);
		free(current->data);
		free(current);

		current = next;
	}

	list->head = NULL;
	list->tail = NULL;
}

//==================== MAIN ====================

Farmacie* createFarmacie(char* den, float sup, unsigned char nr, char* adr, char* lant) {
	Farmacie* f = (Farmacie*)malloc(sizeof(Farmacie));

	f->denumire = (char*)malloc(strlen(den) + 1);
	strcpy(f->denumire, den);

	f->adresa = (char*)malloc(strlen(adr) + 1);
	strcpy(f->adresa, adr);

	f->lant = (char*)malloc(strlen(lant) + 1);
	strcpy(f->lant, lant);

	f->suprafata = sup;
	f->nrAngajati = nr;

	return f;
}

int main() {

	List list = { NULL, NULL };

	// minim 5 farmacii
	addToEnd(&list, createFarmacie("Sensiblu", 120, 10, "Bucuresti", "A"));
	addToEnd(&list, createFarmacie("Catena", 80, 15, "Cluj", "B"));
	addToEnd(&list, createFarmacie("HelpNet", 60, 8, "Iasi", "A"));
	addToEnd(&list, createFarmacie("DrMax", 200, 20, "Timisoara", "C"));
	addToEnd(&list, createFarmacie("FarmaciaTei", 90, 5, "Constanta", "A"));

	printf("\n=== Lista initiala ===\n");
	parseListBtoE(list);

	// CERINTA 2
	int nr = countFarmaciiSuprafata(list, 100);
	printf("\nFarmacii cu suprafata < 100: %d\n", nr);

	// CERINTA 3
	printf("\n=== Stergere farmacii cu angajati > 12 ===\n");
	deleteByAngajati(&list, 12);
	parseListBtoE(list);

	// CERINTA 4
	int dim = 0;
	Farmacie** vect = vectorFarmaciiLant(list, "A", &dim);

	printf("\n=== Vector farmacii lant A ===\n");
	for (int i = 0; i < dim; i++) {
		printFarmacie(vect[i]);
	}

	// free vector
	for (int i = 0; i < dim; i++) {
		free(vect[i]->denumire);
		free(vect[i]->adresa);
		free(vect[i]->lant);
		free(vect[i]);
	}
	free(vect);

	freeList(&list);

	return 0;
}


