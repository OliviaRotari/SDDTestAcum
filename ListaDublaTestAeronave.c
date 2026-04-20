#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct {
    char* cod;
    char* destinatie;
    char* dataPlecare;
    char* oraPlecare;
    short int nrMinuteIntarziere;
} CursaAeriana;

typedef struct Node {
    CursaAeriana* data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} List;

// PRINT
void printCursa(const CursaAeriana* c) {
    printf("%s | %s | %s %s | Intarziere: %d min\n",
        c->cod, c->destinatie, c->dataPlecare,
        c->oraPlecare, c->nrMinuteIntarziere);
}

// INSERT LA FINAL (stil profesor)
void addToEnd(List* list, CursaAeriana* c) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = c;
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

// PARCURGERE
void parseList(List list) {
    Node* aux = list.head;
    while (aux) {
        printCursa(aux->data);
        aux = aux->next;
    }
}

// ================= CERINTA 1 =================
// NUMARARE DUPA CONDITIE

int countByCondition(List list, int (*conditionFn)(CursaAeriana*)) {
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

// conditie: intarziere > X
int intarziereMaiMareDe30(CursaAeriana* c) {
    return c->nrMinuteIntarziere > 30;
}

// ================= CERINTA 2 =================
// STERGERE DUPA CONDITIE

void deleteByCondition(List* list, int (*conditionFn)(CursaAeriana*)) {

    Node* aux = list->head;

    while (aux) {

        if (conditionFn(aux->data)) {

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

            // free complet (stil profesor)
            free(toDelete->data->cod);
            free(toDelete->data->destinatie);
            free(toDelete->data->dataPlecare);
            free(toDelete->data->oraPlecare);
            free(toDelete->data);
            free(toDelete);
        }
        else {
            aux = aux->next;
        }
    }
}

// conditie: intarziere > 60
int intarziereMare(CursaAeriana* c) {
    return c->nrMinuteIntarziere > 60;
}

// ================= CERINTA 3 =================
// SALVARE IN VECTOR

CursaAeriana** saveToVector(List list, int (*conditionFn)(CursaAeriana*), int* dim) {

    *dim = 0;
    Node* aux = list.head;

    // prima parcurgere (numarare)
    while (aux) {
        if (conditionFn(aux->data)) {
            (*dim)++;
        }
        aux = aux->next;
    }

    if (*dim == 0) return NULL;

    // alocare vector
    CursaAeriana** vector = (CursaAeriana**)malloc(sizeof(CursaAeriana*) * (*dim));

    // a doua parcurgere (copiere)
    aux = list.head;
    int index = 0;

    while (aux) {
        if (conditionFn(aux->data)) {
            vector[index++] = aux->data;
        }
        aux = aux->next;
    }

    return vector;
}

// conditie: ora < 12:00
int pleacaDimineata(CursaAeriana* c) {
    int ora = atoi(c->oraPlecare);
    return ora < 12;
}

void freeList(List* list) {
    Node* current = list->head;

    while (current) {
        Node* next = current->next;

        free(current->data->cod);
        free(current->data->destinatie);
        free(current->data->dataPlecare);
        free(current->data->oraPlecare);
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

    // creare manuala (simplu pentru test)
    for (int i = 0; i < 5; i++) {
        CursaAeriana* c = (CursaAeriana*)malloc(sizeof(CursaAeriana));

        c->cod = _strdup("RO123");
        c->destinatie = _strdup("Paris");
        c->dataPlecare = _strdup("2026-04-20");

        if (i % 2 == 0)
            c->oraPlecare = _strdup("10");
        else
            c->oraPlecare = _strdup("14");

        c->nrMinuteIntarziere = i * 20;

        addToEnd(&list, c);
    }

    printf("Lista initiala:\n");
    parseList(list);

    // CERINTA 1
    int nr = countByCondition(list, intarziereMaiMareDe30);
    printf("\nCurse cu intarziere >30: %d\n", nr);

    // CERINTA 3
    int dim = 0;
    CursaAeriana** vector = saveToVector(list, pleacaDimineata, &dim);

    printf("\nCurse pana in 12:\n");
    for (int i = 0; i < dim; i++) {
        printCursa(vector[i]);
    }

    // CERINTA 2
    printf("\nStergem cursele cu intarziere >60:\n");
    deleteByCondition(&list, intarziereMare);
    parseList(list);

    free(vector);
    freeList(&list);
    return 0;
}