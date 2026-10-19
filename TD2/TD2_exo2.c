#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <stdbool.h>


#define N_TEST 10

// Temps : O(1) | Mémoire : O(1)
void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Temps : O(n) | Mémoire : O(1)
bool is_sorted_ascending(int *tab, int n) {
    for (int i = 1; i < n; i = i +1) {
        if (tab[i] < tab[i - 1]) {
            return false;
        }
    }
    return true;
}

// Temps : O(n) | Mémoire : O(1)
void copy_array(int *src, int *dst, int n) {
    for (int i = 0; i < n; i = i + 1) {
        dst[i] = src[i];
    }
}

// Temps : O(n) | Mémoire : O(1)
void print_array(int *tab, int n) {
    for (int i= 0; i < n; i = i+ 1) {
        printf("%d ", tab[i]);
    }
    printf("\n");
}

// Temps : O(n^2) dans tous les cas | Mémoire : O(1)
void selection_sort(int *tab, int n) {
    for (int i = 0; i < n - 1; i = i + 1) {
        int min =i;
        for (int j = i + 1; j< n; j = j +1) {
            if (tab[j] < tab[min]) {
                min = j;
            }
        }

        swap_int(&tab[i], &tab[min]);
    }
}

// Temps : O(n) au mieux, O(n^2) au pire | Mémoire : O(1)
void insertion_sort(int *tab, int n) {
    for (int i = 1; i < n; i = i + 1) {
        int j =i;
        while (j >0 && tab[j - 1] > tab[j]) {
            swap_int(&tab[j - 1],&tab[j]);
            j = j - 1;
        }
    }
}

// Temps : O(n^2) au pire | Mémoire : O(1)
void bubble_sort(int *tab, int n) {
    for (int i = 0; i < n - 1; i = i + 1) {
        for (int j = 0; j < n - 1 - i; j = j + 1) {
            if (tab[j] > tab[j + 1]) {
                swap_int(&tab[j], &tab[j + 1]);
            }
        }
    }
}

// Temps : O(n) | Mémoire : O(n) (tableau temporaire)
void merge(int *tab, int left, int mid, int right) {
    int size = right - left +1;
    int *tmp = malloc(size * sizeof(int));

    int i = left;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= right) {
        if (tab[i] <= tab[j]) {
            tmp[k] = tab[i];
            i = i + 1;
        } else {
            tmp[k] = tab[j];
            j = j+ 1;
        }
        k = k + 1;
    }
    while (i <= mid) {
        tmp[k] = tab[i];
        i = i +1;
        k = k + 1;
    }
    while (j <= right) {
        tmp[k] = tab[j];
        j = j + 1;
        k = k + 1;
    }

    for (k = 0; k <size; k = k +1) {
        tab[left + k] = tmp[k];
    }
    free(tmp);
}

// Temps : O(n log n) dans tous les cas | Mémoire : O(n)
void merge_sort(int *tab, int left, int right) {
    if (left < right) {
        int mid = (left +right) / 2;
        merge_sort(tab, left, mid);

        merge_sort(tab, mid +1, right);
        merge(tab,left, mid, right);
    }
}

// Temps : O(n) | Mémoire : O(1)
int partition(int *tab, int low, int high) {
    int pivot = tab[high];
    int i = low -1;

    for (int j = low; j < high; j =j + 1) {
        if (tab[j] <= pivot) {
            i = i + 1;
            swap_int(&tab[i],&tab[j]);
        }
    }
    swap_int(&tab[i + 1], &tab[high]);
    return i + 1;
}

// Temps : O(n log n) au mieux, O(n^2) au pire
// Mémoire : O(log n) au mieux (pile récursive), O(n) au pire
void quick_sort(int *tab, int low, int high) {
    if (low < high) {
        int p = partition(tab, low, high);
        quick_sort(tab, low, p - 1);
        quick_sort(tab, p + 1, high);
    }
}

// Temps : O(n) | Mémoire : O(1)
void report(char *name, int *tab, int n) {
    printf("%-10s : ", name);
    print_array(tab,n);
    if (is_sorted_ascending(tab, n)) {
        printf("             -> trie : OK\n");
    } else {
        printf("             -> trie : ERREUR\n");
    }
}

int main(void) {
    srand(time(NULL));

    // test de correction sur un petit tableau

    int orig_t[N_TEST];
    int work_t[N_TEST];

    for (int i =0; i < N_TEST; i = i +1) {
        orig_t[i] =rand() % 21;
    }

    printf("=== Test de correction (n = %d) ===\n", N_TEST);
    printf("Initial    : ");
    print_array(orig_t, N_TEST);
    printf("\n");

    copy_array(orig_t, work_t, N_TEST);
    selection_sort(work_t, N_TEST);
    report("Selection", work_t, N_TEST);

    copy_array(orig_t, work_t, N_TEST);
    insertion_sort(work_t, N_TEST);
    report("Insertion", work_t, N_TEST);

    copy_array(orig_t, work_t, N_TEST);

    bubble_sort(work_t, N_TEST);
    report("Bulles", work_t, N_TEST);

    copy_array(orig_t, work_t, N_TEST);
    merge_sort(work_t, 0, N_TEST - 1);
    report("Fusion", work_t, N_TEST);

    copy_array(orig_t, work_t, N_TEST);
    quick_sort(work_t, 0, N_TEST - 1);
    report("Rapide", work_t, N_TEST);

    // comparaison des temps d'exécution
    int sizes[] = {1000, 5000, 10000, 50000, 100000};
    int nb_sizes = 5;
    clock_t start, end;

    printf("\n=== Temps d'execution en secondes ===\n");
    printf("%8s %10s %10s %10s %10s %10s\n",
           "n", "Selection", "Insertion","Bulles", "Fusion", "Rapide");

    for (int s = 0; s < nb_sizes; s = s + 1) {
        int n = sizes[s];
        int *orig =malloc(n * sizeof(int));
        int *work = malloc(n * sizeof(int));

        for (int i = 0; i < n; i = i + 1) {
            orig[i] = rand() % 1000001;
        }

        copy_array(orig, work, n);
        start = clock();
        selection_sort(work, n);
        end = clock();
        double t_sel = (double)(end - start) / CLOCKS_PER_SEC;
        if (!is_sorted_ascending(work, n)) {
            printf("ERREUR : selection incorrect pour n = %d\n", n);
        }

        copy_array(orig, work, n);
        start = clock();
        insertion_sort(work, n);
        end = clock();

        double t_ins = (double)(end - start) / CLOCKS_PER_SEC;
        if (!is_sorted_ascending(work, n)) {
            printf("ERREUR : insertion incorrect pour n = %d\n", n);
        }

        copy_array(orig, work, n);
        start = clock();
        bubble_sort(work, n);
        end = clock();
        double t_bub = (double)(end - start) / CLOCKS_PER_SEC;
        if (!is_sorted_ascending(work, n)) {
            printf("ERREUR : bulles incorrect pour n = %d\n", n);
        }

        copy_array(orig, work, n);
        start = clock();
        merge_sort(work, 0, n - 1);
        end = clock();
        double t_mer = (double)(end - start) / CLOCKS_PER_SEC;
        if (!is_sorted_ascending(work, n)) {
            printf("ERREUR : fusion incorrect pour n = %d\n", n);
        }

        copy_array(orig, work, n);
        start = clock();
        quick_sort(work, 0, n - 1);
        end = clock();
        double t_qui = (double)(end - start) / CLOCKS_PER_SEC;
        if (!is_sorted_ascending(work, n)) {
            printf("ERREUR : rapide incorrect pour n = %d\n", n);
        }

        printf("%8d %10.5f %10.5f %10.5f %10.5f %10.5f\n",
               n, t_sel, t_ins, t_bub, t_mer, t_qui);

        free(orig);
        free(work);
    }

    return 0;
}