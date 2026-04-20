#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= STRUCTURA =================

typedef struct {
	char* denumire;
	float suprafata;
	unsigned char nrAngajati;
	char* adresa;
} Farmacie;

typedef struct Node {
	Farmacie* data;
	struct Node* next;
} Node;

// ================= FUNCTII UTILE =================

Farmacie* createFarmacie(const char* den, float sup, unsigned char nr, const char* adr) {
	Farmacie* f = (Farmacie*)malloc(sizeof(Farmacie));

	f->denumire = (char*)malloc(strlen(den) + 1);
	strcpy(f->denumire, den);

	f->adresa = (char*)malloc(strlen(adr) + 1);
	strcpy(f->adresa, adr);

	f->suprafata = sup;
	f->nrAngajati = nr;

	return f;
}

void printFarmacie(const Farmacie* f) {
	printf("Denumire: %s | Suprafata: %.2f | Angajati: %d | Adresa: %s\n",
		f->denumire, f->suprafata, f->nrAngajati, f->adresa);
}

// ================= OPERATII LISTA =================

void addToEnd(Node** list, Farmacie* f) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = f;
	newNode->next = NULL;

	if (*list) {
		Node* aux = *list;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = newNode;
	}
	else {
		*list = newNode;
	}
}

void parseList(Node* list) {
	if (!list) {
		printf("Lista este goala\n");
		return;
	}

	while (list) {
		printFarmacie(list->data);
		list = list->next;
	}
}

// ================= CERINTA 2 =================
// numar farmacii cu suprafata < prag

int countFarmaciiBySuprafata(Node* list, float prag) {
	int count = 0;

	while (list) {
		if (list->data->suprafata < prag) {
			count++;
		}
		list = list->next;
	}
	return count;
}

// ================= CERINTA 3 =================
// stergere farmacii cu nr angajati > prag

void deleteByNrAngajati(Node** list, unsigned char prag) {

	Node* aux = *list;
	Node* prev = NULL;

	while (aux) {

		if (aux->data->nrAngajati > prag) {

			Node* toDelete = aux;

			if (prev) {
				prev->next = aux->next;
				aux = aux->next;
			}
			else {
				*list = (*list)->next;
				aux = *list;
			}

			free(toDelete->data->denumire);
			free(toDelete->data->adresa);
			free(toDelete->data);
			free(toDelete);
		}
		else {
			prev = aux;
			aux = aux->next;
		}
	}
}

// ================= FREE =================

void freeList(Node** list) {
	Node* current = *list;

	while (current) {
		Node* next = current->next;

		free(current->data->denumire);
		free(current->data->adresa);
		free(current->data);
		free(current);

		current = next;
	}

	*list = NULL;
}

// ================= MAIN =================

int main() {

	Node* list = NULL;

	// minim 5 farmacii
	addToEnd(&list, createFarmacie("Catena", 120.5, 10, "Bucuresti"));
	addToEnd(&list, createFarmacie("HelpNet", 80.2, 5, "Cluj"));
	addToEnd(&list, createFarmacie("Sensiblu", 60.0, 8, "Iasi"));
	addToEnd(&list, createFarmacie("Dona", 150.3, 12, "Timisoara"));
	addToEnd(&list, createFarmacie("Farmacia Tei", 200.0, 20, "Bucuresti"));

	printf("=========== LISTA INITIALA ===========\n");
	parseList(list);

	// CERINTA 2
	float pragSuprafata = 100;
	int nr = countFarmaciiBySuprafata(list, pragSuprafata);

	printf("\nFarmacii cu suprafata < %.2f: %d\n", pragSuprafata, nr);

	// CERINTA 3
	printf("\n=========== INAINTE DE STERGERE ===========\n");
	parseList(list);

	unsigned char pragAngajati = 10;
	deleteByNrAngajati(&list, pragAngajati);

	printf("\n=========== DUPA STERGERE (nrAngajati > %d) ===========\n", pragAngajati);
	parseList(list);

	freeList(&list);

	return 0;
}