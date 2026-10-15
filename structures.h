#ifndef STRUCTURES_H
#define STRUCTURES_H

/*
 * structures.h
 * Piles (stack), files (queue) et tables de hachage (hash table) en C.
 *
 * Les fonctions sont "static inline" : on peut donc inclure ce fichier
 * dans plusieurs .c sans erreur de définitions multiples.
 *
 * Les malloc/calloc/realloc sont "castés" : le fichier est ainsi valide
 * en C ET en C++ (VS Code analyse les .h comme du C++ par défaut).
 *
 * Pile et file : tableaux dynamiques d'entiers (int).
 * Table de hachage : clés = chaînes de caractères, valeurs = int,
 *                    collisions gérées par chaînage (listes chaînées).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================
 *  PILE (STACK) — principe LIFO : Last In, First Out
 *  Le dernier élément empilé est le premier dépilé (pile d'assiettes).
 *  Usages : annuler/refaire (undo), appels de fonctions, parenthésage...
 *  Implémentation : tableau dynamique, le "sommet" est à data[size - 1].
 * ======================================================================== */

typedef struct {
    int *data;      /* tableau des éléments            */
    int  size;      /* nombre d'éléments présents      */
    int  capacity;  /* taille allouée du tableau       */
} Stack;

/* Crée une pile vide. Renvoie NULL en cas d'échec. */
static inline Stack *stack_create(int capacity)
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (s == NULL) return NULL;
    if (capacity < 1) capacity = 4;
    s->data = (int *)malloc((size_t)capacity * sizeof(int));
    if (s->data == NULL) { free(s); return NULL; }
    s->size = 0;
    s->capacity = capacity;
    return s;
}

static inline int stack_is_empty(const Stack *s) { return s->size == 0; }
static inline int stack_size(const Stack *s)     { return s->size; }

/* Empile une valeur (le tableau double de taille si besoin).
 * Renvoie 1 si OK, 0 si échec d'allocation. */
static inline int stack_push(Stack *s, int value)
{
    if (s->size == s->capacity) {
        int new_cap = s->capacity * 2;
        int *tmp = (int *)realloc(s->data, (size_t)new_cap * sizeof(int));
        if (tmp == NULL) return 0;
        s->data = tmp;
        s->capacity = new_cap;
    }
    s->data[s->size++] = value;
    return 1;
}

/* Dépile : écrit la valeur dans *out. Renvoie 0 si la pile est vide. */
static inline int stack_pop(Stack *s, int *out)
{
    if (stack_is_empty(s)) return 0;
    s->size--;
    if (out != NULL) *out = s->data[s->size];
    return 1;
}

/* Lit le sommet sans le retirer. Renvoie 0 si la pile est vide. */
static inline int stack_peek(const Stack *s, int *out)
{
    if (stack_is_empty(s)) return 0;
    *out = s->data[s->size - 1];
    return 1;
}

static inline void stack_free(Stack *s)
{
    if (s == NULL) return;
    free(s->data);
    free(s);
}

/* ========================================================================
 *  FILE (QUEUE) — principe FIFO : First In, First Out
 *  Le premier élément entré est le premier sorti (file d'attente).
 *  Usages : parcours en largeur (BFS), ordonnancement, tampons...
 *  Implémentation : tableau CIRCULAIRE. On garde l'indice de la tête
 *  (head) ; la queue se calcule : (head + size) % capacity.
 *  Ainsi enfiler et défiler sont en O(1), sans décaler les éléments.
 * ======================================================================== */

typedef struct {
    int *data;
    int  head;      /* indice du premier élément       */
    int  size;      /* nombre d'éléments présents      */
    int  capacity;
} Queue;

static inline Queue *queue_create(int capacity)
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL) return NULL;
    if (capacity < 1) capacity = 4;
    q->data = (int *)malloc((size_t)capacity * sizeof(int));
    if (q->data == NULL) { free(q); return NULL; }
    q->head = 0;
    q->size = 0;
    q->capacity = capacity;
    return q;
}

static inline int queue_is_empty(const Queue *q) { return q->size == 0; }
static inline int queue_size(const Queue *q)     { return q->size; }

/* Enfile une valeur à la fin. Si la file est pleine, on agrandit le
 * tableau en recopiant les éléments "dans l'ordre" à partir de l'indice 0.
 * Renvoie 1 si OK, 0 si échec d'allocation. */
static inline int queue_enqueue(Queue *q, int value)
{
    if (q->size == q->capacity) {
        int new_cap = q->capacity * 2;
        int *tmp = (int *)malloc((size_t)new_cap * sizeof(int));
        if (tmp == NULL) return 0;
        for (int i = 0; i < q->size; i++)
            tmp[i] = q->data[(q->head + i) % q->capacity];
        free(q->data);
        q->data = tmp;
        q->head = 0;
        q->capacity = new_cap;
    }
    q->data[(q->head + q->size) % q->capacity] = value;
    q->size++;
    return 1;
}

/* Défile le premier élément : écrit la valeur dans *out.
 * Renvoie 0 si la file est vide. */
static inline int queue_dequeue(Queue *q, int *out)
{
    if (queue_is_empty(q)) return 0;
    if (out != NULL) *out = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return 1;
}

/* Lit le premier élément sans le retirer. Renvoie 0 si vide. */
static inline int queue_peek(const Queue *q, int *out)
{
    if (queue_is_empty(q)) return 0;
    *out = q->data[q->head];
    return 1;
}

static inline void queue_free(Queue *q)
{
    if (q == NULL) return;
    free(q->data);
    free(q);
}

/* ========================================================================
 *  TABLE DE HACHAGE (HASH TABLE)
 *  Associe une clé (ici une chaîne) à une valeur (ici un int), avec un
 *  accès en O(1) en moyenne (contre O(n) pour une recherche linéaire).
 *  Principe : une fonction de hachage transforme la clé en un nombre,
 *  qu'on réduit modulo le nombre de cases (buckets) pour obtenir l'indice.
 *  Collisions (deux clés dans la même case) : chaînage, chaque case
 *  contient une liste chaînée d'entrées.
 *  Usages : dictionnaires, caches, comptage d'occurrences, index...
 * ======================================================================== */

typedef struct HashEntry {
    char *key;                  /* copie de la clé (allouée)        */
    int   value;
    struct HashEntry *next;     /* entrée suivante dans la case     */
} HashEntry;

typedef struct {
    HashEntry **buckets;        /* tableau de listes chaînées       */
    int nb_buckets;
    int count;                  /* nombre de couples clé/valeur     */
} HashTable;

/* Fonction de hachage djb2 (classique et simple). */
static inline unsigned long ht_hash(const char *key)
{
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*key++) != 0)
        h = h * 33 + (unsigned long)c;
    return h;
}

/* Copie d'une chaîne (strdup n'est pas du C standard, on l'écrit). */
static inline char *ht_strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *copy = (char *)malloc(len);
    if (copy != NULL) memcpy(copy, s, len);
    return copy;
}

static inline HashTable *ht_create(int nb_buckets)
{
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (ht == NULL) return NULL;
    if (nb_buckets < 1) nb_buckets = 16;
    ht->buckets = (HashEntry **)calloc((size_t)nb_buckets, sizeof(HashEntry *));
    if (ht->buckets == NULL) { free(ht); return NULL; }
    ht->nb_buckets = nb_buckets;
    ht->count = 0;
    return ht;
}

/* Cherche l'entrée d'une clé (NULL si absente). */
static inline HashEntry *ht_find(const HashTable *ht, const char *key)
{
    unsigned long idx = ht_hash(key) % (unsigned long)ht->nb_buckets;
    for (HashEntry *e = ht->buckets[idx]; e != NULL; e = e->next)
        if (strcmp(e->key, key) == 0) return e;
    return NULL;
}

/* Insère ou met à jour (si la clé existe déjà, on remplace la valeur).
 * Renvoie 1 si OK, 0 si échec d'allocation. */
static inline int ht_put(HashTable *ht, const char *key, int value)
{
    HashEntry *e = ht_find(ht, key);
    if (e != NULL) { e->value = value; return 1; }

    e = (HashEntry *)malloc(sizeof(HashEntry));
    if (e == NULL) return 0;
    e->key = ht_strdup(key);
    if (e->key == NULL) { free(e); return 0; }
    e->value = value;

    unsigned long idx = ht_hash(key) % (unsigned long)ht->nb_buckets;
    e->next = ht->buckets[idx];     /* insertion en tête de liste */
    ht->buckets[idx] = e;
    ht->count++;
    return 1;
}

/* Récupère la valeur d'une clé dans *out. Renvoie 0 si absente. */
static inline int ht_get(const HashTable *ht, const char *key, int *out)
{
    HashEntry *e = ht_find(ht, key);
    if (e == NULL) return 0;
    *out = e->value;
    return 1;
}

static inline int ht_contains(const HashTable *ht, const char *key)
{
    return ht_find(ht, key) != NULL;
}

/* Supprime une clé. Renvoie 1 si supprimée, 0 si elle n'existait pas. */
static inline int ht_remove(HashTable *ht, const char *key)
{
    unsigned long idx = ht_hash(key) % (unsigned long)ht->nb_buckets;
    HashEntry *prev = NULL;
    for (HashEntry *e = ht->buckets[idx]; e != NULL; prev = e, e = e->next) {
        if (strcmp(e->key, key) == 0) {
            if (prev == NULL) ht->buckets[idx] = e->next;
            else              prev->next = e->next;
            free(e->key);
            free(e);
            ht->count--;
            return 1;
        }
    }
    return 0;
}

/* Affiche le contenu, case par case (utile pour voir les collisions). */
static inline void ht_print(const HashTable *ht)
{
    for (int i = 0; i < ht->nb_buckets; i++) {
        if (ht->buckets[i] == NULL) continue;
        printf("[%d]", i);
        for (HashEntry *e = ht->buckets[i]; e != NULL; e = e->next)
            printf(" -> (%s : %d)", e->key, e->value);
        printf("\n");
    }
}

static inline void ht_free(HashTable *ht)
{
    if (ht == NULL) return;
    for (int i = 0; i < ht->nb_buckets; i++) {
        HashEntry *e = ht->buckets[i];
        while (e != NULL) {
            HashEntry *next = e->next;
            free(e->key);
            free(e);
            e = next;
        }
    }
    free(ht->buckets);
    free(ht);
}

#endif /* STRUCTURES_H */
