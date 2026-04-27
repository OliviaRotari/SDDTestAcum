//#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<malloc.h>

typedef struct {
    char* country;
    char group;
    int matchesPlayed;
} FootballTeam;

typedef struct Node {
    FootballTeam* data;
    struct Node* next;
} Node;

typedef struct {
    int size;
    Node** buckets;
} HashTable;

void printTeam(const FootballTeam* team) {
    printf("Group %c | %s - Matches played: %d\n", team->group, team->country, team->matchesPlayed);
}



int hash(int size, const char* key) {
    int sum = 0;
    for (int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % size;
}

HashTable initHashTable(int size) {
    HashTable ht;
    ht.size = size;
    ht.buckets = malloc(size * sizeof(Node*));

    for (int i = 0; i < size; i++) {
        ht.buckets[i] = NULL;
    }

    return ht;
}

void insert(HashTable* ht, FootballTeam* team) {
    int position = hash(ht->size, team->country);

    Node* newNode = malloc(sizeof(Node));
    newNode->data = team;
    newNode->next = ht->buckets[position];

    ht->buckets[position] = newNode;
}

FootballTeam* search(HashTable* ht, const char* country) {
    int position = hash(ht->size, country);

    Node* current = ht->buckets[position];

    while (current) {
        if (strcmp(current->data->country, country) == 0) {
            return current->data;
        }
        current = current->next;
    }

    return NULL;
}

void deleteByCountry(HashTable* ht, const char* country) {
    int position = hash(ht->size, country);

    Node* current = ht->buckets[position];
    Node* previous = NULL;

    while (current) {
        if (strcmp(current->data->country, country) == 0) {
            if (previous) {
                previous->next = current->next;
            }
            else {
                ht->buckets[position] = current->next;
            }

            free(current->data->country);
            free(current->data);
            free(current);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Country '%s' not found\n", country);
}

void printHashTable(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        printf("Bucket %d:\n", i);
        Node* current = ht->buckets[i];
        while (current) {
            printf("\t");
            printTeam(current->data);
            current = current->next;
        }
    }
}

void freeHashTable(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->buckets[i];
        while (current) {
            Node* next = current->next;
            free(current->data->country);
            free(current->data);
            free(current);
            current = next;
        }
    }
    free(ht->buckets);
    ht->buckets = NULL;
    ht->size = 0;
}

int loadTeams(const char* filename, HashTable* ht) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        return -1;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        FootballTeam* team = malloc(sizeof(FootballTeam));

        char* token = strtok(line, ",");
        team->country = malloc(strlen(token) + 1);
        strcpy(team->country, token);

        token = strtok(NULL, ",");
        team->group = token[0];

        token = strtok(NULL, ",");
        team->matchesPlayed = atoi(token);

        insert(ht, team);
        count++;
    }

    fclose(f);
    return count;
}

int main() {

    HashTable ht = initHashTable(47);
    int count = loadTeams("teams.csv", &ht);

    if (count < 0) {
        printf("Failed to load teams.\n");
        return 1;
    }

    printf("Loaded %d teams.\n\n", count);

    printHashTable(&ht);

    freeHashTable(&ht);

    return 0;
}