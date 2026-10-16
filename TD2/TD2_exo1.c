#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <time.h>


bool is_sorted_increasing(int* tab, int n){ // worst case complexity is O(n)
    int old_int = tab[0];
    for (int i = 1 ; i < n ; i= i+1){
        if (tab[i] < old_int){
            return false;
        } else {
            old_int =tab[i];
        }
    }
    return true;
}

int linear_search(int* tab, int target, int n){ //worst case time complexity is O(n)
    for (int i = 0 ; i < n ; i= i+1){
        if (tab[i] == target){
            return i;
        }
    }
    return -1;
}

int jump_search(int *tab, int target, int n) // worst cast time complexity is O(sqrt(n))
{
    if (!is_sorted_increasing(tab, n)) {
        return -1;
    }

    int pas = (int)sqrt(n);      // taille du saut 
    if (pas < 1) {
        pas = 1;
    }

    int prev = 0;               
    int p = pas;                
    int fin = p;                 
    if (fin > n) {
        fin = n;
    }

   
    while (tab[fin - 1] < target) {
        prev = p;
        if (prev >= n) {         // fin du tableau dépassée 
            return -1;
        }
        p += pas;
        fin = p;
        if (fin > n) {
            fin = n;
        }
    }

    
    for (int i = prev; i < fin; i=i+1) {
        if (tab[i] == target) {
            return i;
        }
        if (tab[i] > target) {   // tableau trié, inutile de continuer 
            break;
        }
    }

    return -1;
}

int binary_search(int* tab, int target, int n){
    if (!is_sorted_increasing(tab, n)) {
        return -1;
    }
    int low = 0;
    int high = n-1;
    int mid = low + (high -low)/2;
    while (low <= high){
         if (tab[mid]==target){
            return mid;
         } else if (tab[mid] <= target){
            low = mid +1;
         } else {
            high= mid -1;
         }
         mid =low +(high-low)/2;
    }
    return -1;
}



int main(){

    int tab[10000];
    for (int i =0; i < 10000; i= i+1){
        tab[i]=i;
    }
    int n = sizeof(tab) / sizeof(tab[0]);

    clock_t begin = clock();
    int indice_linear=linear_search(tab, 9999, n);
    clock_t end = clock();

    clock_t begin2 = clock();
    int indice_jump = jump_search(tab,9999,n);
    clock_t end2 = clock();

    clock_t begin3 = clock();
    int indice_binary = binary_search(tab,9999,n);
    clock_t end3 = clock();

    printf("linear : %i\n",indice_linear);
    printf("jump : %i\n",indice_jump);
    printf("binary : %i\n",indice_binary);
    printf("\n");

    float linear_time = (end-begin)*1000.0/CLOCKS_PER_SEC;
    float jump_time = (end2-begin2)*1000.0/CLOCKS_PER_SEC;
    float binary_time = (end3-begin3)*1000.0/CLOCKS_PER_SEC;


    printf("linear time : %f ms\n",linear_time);
    printf("jump : %f ms\n",jump_time);
    printf("binary : %f ms\n",binary_time);


} // ici, à cause des vérifications de croissance des tableaux, les méthodes jump search 
 //binary search ont une efficacité comparable à linear search, vu que la vérification 
 // de la croissane coute O(n) en complexité temporelle