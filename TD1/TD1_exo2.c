#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


// Naive prime numbers (O(n sqrt(n)))
int naive_prime(int n){
    if (n <= 2) return 0;
    int prime_count = 0;

    for (int m=2; m<n; m++){
        int premier = 1;
        int limit = sqrt(m);
        for (int i=2; i<=limit; i++){
            if (m%i == 0){
                premier = 0;
                break;
            }
        }
        if (premier){
            prime_count++;
        }
    }

    return prime_count;
}

// Efficient prime numbers 

int opti_prime(int n) {
    if (n <= 2) return 0;

    int sum = 0;
    int *premiers = (int *)malloc(n * sizeof(int));
    if (!premiers) return -1;

    for (int i = 0; i < n; i++) {
        premiers[i] = 0; // non visité
    }

    for (long long i = 2; i < n; i++) {
        if (premiers[i] != -1) {
            sum++;
            for (long long k = i; i * k < n; k++) {
                premiers[i * k] = -1; // non premier
            }
        }
    }
    free(premiers);
    return sum;
}

int main(){
    int n_large = 1000000; 
    int n_small = 10000;   

    // Time naive 
    clock_t start_naive = clock();
    int res_naive = naive_prime(n_small);
    clock_t end_naive = clock();
    double time_naive = (double)(end_naive - start_naive) / CLOCKS_PER_SEC;

    // Time opti 
    clock_t start_opti = clock();
    int res_opti = opti_prime(n_large);
    clock_t end_opti = clock();
    double time_opti = (double)(end_opti - start_opti) / CLOCKS_PER_SEC;

    printf("Temps Naif      : %f secondes (Premiers < %d : %d)\n", time_naive, n_small, res_naive);
    printf("Temps Opti      : %f secondes (Premiers < %d : %d)\n", time_opti, n_large, res_opti);

    return 0;
}
