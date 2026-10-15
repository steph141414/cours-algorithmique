/*
 * test_structures.c — teste structures.h
 * Compilation : gcc -Wall -Wextra -std=c99 -o test_structures test_structures.c
 * Exécution   : ./test_structures
 */

#include <assert.h>
#include <stdio.h>
#include "structures.h"

static void test_stack(void)
{
    printf("=== Test de la pile ===\n");
    Stack *s = stack_create(2);      /* petite capacité pour tester l'agrandissement */
    assert(s != NULL);
    assert(stack_is_empty(s));

    for (int i = 1; i <= 5; i++) stack_push(s, i * 10);   /* 10 20 30 40 50 */
    assert(stack_size(s) == 5);

    int v;
    assert(stack_peek(s, &v) && v == 50);
    assert(stack_size(s) == 5);                           /* peek ne retire rien */

    printf("Dépilage : ");
    while (stack_pop(s, &v)) printf("%d ", v);            /* 50 40 30 20 10 (LIFO) */
    printf("\n");

    assert(stack_is_empty(s));
    assert(!stack_pop(s, &v));                            /* pile vide */
    stack_free(s);
    printf("Pile : OK\n\n");
}

static void test_queue(void)
{
    printf("=== Test de la file ===\n");
    Queue *q = queue_create(3);
    assert(q != NULL);
    assert(queue_is_empty(q));

    int v;
    queue_enqueue(q, 1);
    queue_enqueue(q, 2);
    queue_enqueue(q, 3);
    assert(queue_dequeue(q, &v) && v == 1);               /* FIFO */
    assert(queue_dequeue(q, &v) && v == 2);

    /* La tête a avancé : ces ajouts "bouclent" dans le tableau circulaire
     * puis forcent un agrandissement. */
    queue_enqueue(q, 4);
    queue_enqueue(q, 5);
    queue_enqueue(q, 6);
    queue_enqueue(q, 7);
    assert(queue_size(q) == 5);
    assert(queue_peek(q, &v) && v == 3);

    printf("Défilage : ");
    while (queue_dequeue(q, &v)) printf("%d ", v);        /* 3 4 5 6 7 */
    printf("\n");

    assert(queue_is_empty(q));
    assert(!queue_dequeue(q, &v));
    queue_free(q);
    printf("File : OK\n\n");
}

static void test_hash_table(void)
{
    printf("=== Test de la table de hachage ===\n");
    HashTable *ht = ht_create(4);    /* peu de cases => collisions garanties */
    assert(ht != NULL);

    ht_put(ht, "pomme", 3);
    ht_put(ht, "poire", 7);
    ht_put(ht, "banane", 12);
    ht_put(ht, "cerise", 25);
    ht_put(ht, "kiwi", 1);
    assert(ht->count == 5);

    printf("Contenu (cases) :\n");
    ht_print(ht);

    int v;
    assert(ht_get(ht, "banane", &v) && v == 12);
    assert(!ht_get(ht, "mangue", &v));                    /* clé absente */
    assert(ht_contains(ht, "kiwi"));

    ht_put(ht, "pomme", 99);                              /* mise à jour */
    assert(ht_get(ht, "pomme", &v) && v == 99);
    assert(ht->count == 5);                               /* pas de doublon */

    assert(ht_remove(ht, "poire"));
    assert(!ht_contains(ht, "poire"));
    assert(!ht_remove(ht, "poire"));                      /* déjà supprimée */
    assert(ht->count == 4);

    /* Les autres clés restent accessibles après la suppression */
    assert(ht_get(ht, "cerise", &v) && v == 25);
    assert(ht_get(ht, "kiwi", &v) && v == 1);

    ht_free(ht);
    printf("Table de hachage : OK\n\n");
}

int main(void)
{
    test_stack();
    test_queue();
    test_hash_table();
    printf("Tous les tests sont passés.\n");
    return 0;
}
