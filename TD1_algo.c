#include <stdio.h>
#include <time.h>

int fib(int n){
    if (n < 2){
        return n;
    } else {
        return fib(n-1) + fib(n-2);
    }
}

int fib_dynamic(int n){
    if (n <= 1){
        return n;
    }
    int fib1[n + 1];
    fib1[0] = 0;
    fib1[1] = 1;
    for (int i = 2; i <= n; i++){
        fib1[i] = fib1[i-1] + fib1[i-2];
    }
    return fib1[n];
} 




int naive_primes(int n){
    int p_count=0;
    for (int i=2; i<n ; i=i+1){
        int d=2;
        while (d*d <= i){
            if (i%d==0){
                p_count=p_count+1;
                break;
            } else {
                d=d+1;
            }

        }

    }
    return p_count;

}



//int dynamic_primes(int n){
    //bool tab[n]={true};
    //for (int i; i<n; i=i+1){

    //}
//}


int main(){
    int n=40;
    clock_t begin = clock();
    printf("Resultat naïf : %i\n", fib(n)); // Ajout de \n
    clock_t end = clock();
    unsigned long millis = (end - begin) * 1000 / CLOCKS_PER_SEC;
    printf("naive finished in %ld ms\n\n", millis);

    clock_t begin2 = clock();
    printf("Resultat dynamique : %i\n", fib_dynamic(n)); // Ajout de \n
    clock_t end2 = clock();
    unsigned long millis2 = (end2 - begin2) * 1000 / CLOCKS_PER_SEC;
    printf("dynamic finished in %ld ms\n", millis2);

    printf("Resultat naïf pour le nombre de premiers: %i\n", naive_primes(n)); // Ajout de \n

    return 0;
}