#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS

// ================= STRUCTURI =================

typedef struct {
	char* denumire;
	float suprafata;
	unsigned char nrAngajati;
	char* adresa;
} Farmacie;

typedef struct Node {
	Farmacie* data;
	struct Node* next;
	struct Node* prev;
} Node;

typedef struct {
	Node* head;
	Node* tail;
} List;

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

void parseListBtoE(List list) {
	Node* aux = list.head;
	if (!aux) {
		printf("Lista este goala\n");
		return;
	}

	while (aux) {
		printFarmacie(aux->data);
		aux = aux->next;
	}
}

// ================= CERINTA 2 =================
// numar farmacii cu suprafata < prag

int countFarmaciiBySuprafata(List list, float prag) {
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

// ================= CERINTA 3 =================
// stergere farmacii cu nr angajati > prag

void deleteByNrAngajati(List* list, unsigned char prag) {
	Node* aux = list->head;

	while (aux) {
		if (aux->data->nrAngajati > prag) {

			Node* toDelete = aux;

			// legatura cu anterior
			if (aux->prev) {
				aux->prev->next = aux->next;
			}
			else {
				list->head = aux->next;
			}

			// legatura cu urmator
			if (aux->next) {
				aux->next->prev = aux->prev;
			}
			else {
				list->tail = aux->prev;
			}

			aux = aux->next;

			// eliberare memorie
			free(toDelete->data->denumire);
			free(toDelete->data->adresa);
			free(toDelete->data);
			free(toDelete);
		}
		else {
			aux = aux->next;
		}
	}
}

// ================= FREE =================

void freeList(List* list) {
	Node* current = list->head;

	while (current) {
		Node* next = current->next;

		free(current->data->denumire);
		free(current->data->adresa);
		free(current->data);
		free(current);

		current = next;
	}

	list->head = NULL;
	list->tail = NULL;
}

// ================= MAIN =================

int main() {

	List list = { NULL, NULL };

	// minim 5 farmacii
	addToEnd(&list, createFarmacie("Catena", 120.5, 10, "Bucuresti"));
	addToEnd(&list, createFarmacie("HelpNet", 80.2, 5, "Cluj"));
	addToEnd(&list, createFarmacie("Sensiblu", 60.0, 8, "Iasi"));
	addToEnd(&list, createFarmacie("Dona", 150.3, 12, "Timisoara"));
	addToEnd(&list, createFarmacie("Farmacia Tei", 200.0, 20, "Bucuresti"));

	printf("=========== LISTA INITIALA ===========\n");
	parseListBtoE(list);

	// CERINTA 2
	float pragSuprafata = 100;
	int nr = countFarmaciiBySuprafata(list, pragSuprafata);

	printf("\nFarmacii cu suprafata < %.2f: %d\n", pragSuprafata, nr);

	// CERINTA 3
	printf("\n=========== INAINTE DE STERGERE ===========\n");
	parseListBtoE(list);

	unsigned char pragAngajati = 10;
	deleteByNrAngajati(&list, pragAngajati);

	printf("\n=========== DUPA STERGERE (nrAngajati > %d) ===========\n", pragAngajati);
	parseListBtoE(list);

	// eliberare memorie
	freeList(&list);

	return 0;
}