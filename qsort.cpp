#include<iostream>

#include<sys/time.h>

#define REAL double
#define ASC  0  // ascending 
#define DESC 1  // descending

//#define DEBUG

using namespace std;

double get_dtime(void){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((double)(tv.tv_sec)
            + (double)(tv.tv_usec) * 0.001 * 0.001);
}

REAL med3(REAL x, REAL y, REAL z){
    REAL ret;
    if(x<y)
        if(y<z) ret = y; else if(z<x) ret = x; else ret = z;
    else
        if(z<y) ret = y; else if(x<z) ret = x; else ret = z;
    return ret;
}

void quicksort(REAL a[], int left, int right, int flag){
    int i, j;
    REAL tmp, pivot;
    if(left < right){
        i = left; j = right;
        pivot = med3(a[i], a[(i+j)/2], a[j]);
        while(1){
            if(flag == ASC) while(a[i] < pivot) i++;
            else            while(a[i] > pivot) i++;
            if(flag == ASC) while(pivot < a[j]) j--;
            else            while(pivot > a[j]) j--;
            if(i >= j) break;
            tmp = a[i]; a[i] = a[j]; a[j] = tmp;
            i++; j--;
        }
        quicksort(a, left, i-1, flag);
        quicksort(a, j+1, right, flag);
    }
}

int main(int argc, char** argv){
    int i;
    int num;
    REAL* array;
    REAL tmp;
    double d0, d1;

    int order = ASC;
    
    if(argc != 2){
        cout << argv[0] << endl;
        exit(1);
    }

    num = atoi(argv[1]);
    array = (REAL*)malloc(sizeof(REAL)*num);
    
    for(i=0;i<num;i++){
        array[i] = (REAL)rand() / (REAL)RAND_MAX;
    }

#ifdef DEBUG
    for(i=0;i<num;i++){
        cout << i << " : " << array[i] << endl;
    }
#endif
    
    int j;
    int s, e;
    
    d0 = get_dtime();

    s = 0; e = num-1;
    cout << "(" << s << ", " << e << ")" << endl;
    quicksort(array, s, e, order);

    d1 = get_dtime();

#ifdef DEBUG
    cout << endl;
    for(i=0;i<num;i++){
        cout << i << " : " << array[i] << endl;
    }
#endif

    for(i=1;i<num;i++){
        if(order == ASC){
            if(array[i-1] > array[i]){
                cout << "num[" << i-1 << "] : " << array[i-1]
                     << " is larger than num[" << i
                     << "]" << array[i-1] <<endl;
            }
        }
        else{
            if(array[i-1] < array[i]){
                cout << "num[" << i-1 << "] : " << array[i-1]
                     << " is smaller than num[" << i
                     << "]" << array[i-1] <<endl;
            }
        }
    }
    
    cout << "Sorting  : ,     " << d1 - d0 << ", [sec]" << endl;
}

