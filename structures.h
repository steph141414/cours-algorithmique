#ifndef STRUCTURES_H
#define STRUCTURES_H
 
/*
 * structures.h
 * Liste chaînée, pile, file et table de hachage en C.
 * Fonctions "static inline" : le fichier peut être inclus dans plusieurs .c.
 * Les allocations sont castées : valide en C et en C++.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/* ========================================================================
 *  LISTE CHAÎNÉE SIMPLE
 *  Chaque noeud contient une valeur et un pointeur vers le noeud suivant.
 *  Insertion en tête en O(1), accès par indice en O(n).
 * ======================================================================== */
 
typedef struct ListNode {
    int value;
    struct ListNode *next;
} ListNode;
 
typedef struct {
    ListNode *head;
    int size;
} List;
 
static inline List *list_create(void)
{
    List *l = (List *)malloc(sizeof(List));
    if (l == NULL) return NULL;
    l->head = NULL;
    l->size = 0;
    return l;
}
 
static inline int list_is_empty(const List *l) { return l->size == 0; }
static inline int list_size(const List *l)     { return l->size; }
 
static inline int list_push_front(List *l, int value)
{
    ListNode *n = (ListNode *)malloc(sizeof(ListNode));
    if (n == NULL) return 0;
    n->value = value;
    n->next = l->head;
    l->head = n;
    l->size++;
    return 1;
}
 
static inline int list_push_back(List *l, int value)
{
    ListNode *n = (ListNode *)malloc(sizeof(ListNode));
    if (n == NULL) return 0;
    n->value = value;
    n->next = NULL;
    if (l->head == NULL) {
        l->head = n;
    } else {
        ListNode *cur = l->head;
        while (cur->next != NULL) cur = cur->next;
        cur->next = n;
    }
    l->size++;
    return 1;
}
 
/* Insère à la position index (0 = tête, size = fin). */
static inline int list_insert(List *l, int index, int value)
{
    if (index < 0 || index > l->size) return 0;
    if (index == 0) return list_push_front(l, value);
 
    ListNode *prev = l->head;
    for (int i = 0; i < index - 1; i++) prev = prev->next;
 
    ListNode *n = (ListNode *)malloc(sizeof(ListNode));
    if (n == NULL) return 0;
    n->value = value;
    n->next = prev->next;
    prev->next = n;
    l->size++;
    return 1;
}
 
/* Écrit la valeur à la position index dans *out. */
static inline int list_get(const List *l, int index, int *out)
{
    if (index < 0 || index >= l->size) return 0;
    ListNode *cur = l->head;
    for (int i = 0; i < index; i++) cur = cur->next;
    *out = cur->value;
    return 1;
}
 
/* Renvoie l'indice de la première occurrence de value, ou -1. */
static inline int list_find(const List *l, int value)
{
    int i = 0;
    for (ListNode *cur = l->head; cur != NULL; cur = cur->next, i++)
        if (cur->value == value) return i;
    return -1;
}
 
static inline int list_pop_front(List *l, int *out)
{
    if (list_is_empty(l)) return 0;
    ListNode *n = l->head;
    if (out != NULL) *out = n->value;
    l->head = n->next;
    free(n);
    l->size--;
    return 1;
}
 
/* Supprime le noeud à la position index. */
static inline int list_remove_at(List *l, int index, int *out)
{
    if (index < 0 || index >= l->size) return 0;
    if (index == 0) return list_pop_front(l, out);
 
    ListNode *prev = l->head;
    for (int i = 0; i < index - 1; i++) prev = prev->next;
 
    ListNode *n = prev->next;
    if (out != NULL) *out = n->value;
    prev->next = n->next;
    free(n);
    l->size--;
    return 1;
}
 
/* Supprime la première occurrence de value. */
static inline int list_remove_value(List *l, int value)
{
    int i = list_find(l, value);
    if (i < 0) return 0;
    return list_remove_at(l, i, NULL);
}
 
static inline void list_reverse(List *l)
{
    ListNode *prev = NULL;
    ListNode *cur = l->head;
    while (cur != NULL) {
        ListNode *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    l->head = prev;
}
 
static inline void list_print(const List *l)
{
    for (ListNode *cur = l->head; cur != NULL; cur = cur->next)
        printf("%d -> ", cur->value);
    printf("NULL\n");
}
 
static inline void list_free(List *l)
{
    if (l == NULL) return;
    ListNode *cur = l->head;
    while (cur != NULL) {
        ListNode *next = cur->next;
        free(cur);
        cur = next;
    }
    free(l);
}
 
/* ========================================================================
 *  PILE (STACK) — LIFO : le dernier empilé est le premier dépilé.
 *  Tableau dynamique, le sommet est à data[size - 1].
 * ======================================================================== */
 
typedef struct {
    int *data;
    int  size;
    int  capacity;
} Stack;
 
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
 
static inline int stack_pop(Stack *s, int *out)
{
    if (stack_is_empty(s)) return 0;
    s->size--;
    if (out != NULL) *out = s->data[s->size];
    return 1;
}
 
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
 *  FILE (QUEUE) — FIFO : le premier entré est le premier sorti.
 *  Tableau circulaire : la queue est à l'indice (head + size) % capacity.
 * ======================================================================== */
 
typedef struct {
    int *data;
    int  head;
    int  size;
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
 
static inline int queue_dequeue(Queue *q, int *out)
{
    if (queue_is_empty(q)) return 0;
    if (out != NULL) *out = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return 1;
}
 
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
 *  Clés = chaînes, valeurs = int. L'indice de la case est
 *  hash(clé) % nb_buckets ; les collisions sont gérées par chaînage.
 * ======================================================================== */
 
typedef struct HashEntry {
    char *key;
    int   value;
    struct HashEntry *next;
} HashEntry;
 
typedef struct {
    HashEntry **buckets;
    int nb_buckets;
    int count;
} HashTable;
 
/* Fonction de hachage djb2. */
static inline unsigned long ht_hash(const char *key)
{
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*key++) != 0)
        h = h * 33 + (unsigned long)c;
    return h;
}
 
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
 
static inline HashEntry *ht_find(const HashTable *ht, const char *key)
{
    unsigned long idx = ht_hash(key) % (unsigned long)ht->nb_buckets;
    for (HashEntry *e = ht->buckets[idx]; e != NULL; e = e->next)
        if (strcmp(e->key, key) == 0) return e;
    return NULL;
}
 
/* Insère, ou met à jour la valeur si la clé existe déjà. */
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
    e->next = ht->buckets[idx];
    ht->buckets[idx] = e;
    ht->count++;
    return 1;
}
 
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
 
