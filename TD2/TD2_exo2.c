#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <time.h>

 void swap_int(int a, int b){
    int temp=a;
    a=b;
    b=temp;
 }

 bool is_sorted_ascending(int* tab, int n){
    int old_int = tab[0];
    for (int i = 1 ; i < n ; i= i+1){
        if (tab[i] <= old_int){
            return false;
        } else {
            old_int =tab[i];
        }
    }
    return true;
 }

 int* copy_array(int* A, int n){
    int B[n];
    for (int i =0 ; i < n ; i = i+1){
        B[n]=A[n];
    }
    return B;
 }

void selection_sort(int* tab, int n){
    for (int i=0; i < n-1 ; i= i+1){
        int min = i;
        for (int j = i+1 ; j < n; j = j+1){
            if (tab[j] < tab[min]){
                min = j;
            }
        if (min != i){
            swap_int(tab[min], tab[i]);
        }
        }
    }
}



int main(){
    int n = 50;
    int tab[n];
    srand(time(NULL));
    for (int i =0; i <n; i=i+1){
        tab[i]= rand()%n;
    }
    selection_sort(tab,n);
    printf


}
