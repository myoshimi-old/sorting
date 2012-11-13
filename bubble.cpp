#include<iostream>

#include<sys/time.h>

#define REAL double
//#define DEBUG

using namespace std;

double get_dtime(void){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((double)(tv.tv_sec)
            + (double)(tv.tv_usec) * 0.001 * 0.001);
}

int main(int argc, char** argv){
    int i;
    int num;
    REAL* array;
    REAL tmp;
    double d0, d1;

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

    d0 = get_dtime();
    
    for(i=0;i<num;i++){
        for(j=i;j<num;j++){
            if(array[j] < array[i]){
                tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
    
    d1 = get_dtime();

#ifdef DEBUG
    cout << endl;
    for(i=0;i<num;i++){
        cout << i << " : " << array[i] << endl;
    }
#endif

    for(i=1;i<num;i++){
        if(array[i-1] > array[i]){
            cout << "num[" << i-1 << "] : " << array[i-1]
                 << " is larger than num[" << i
                 << "]" << array[i-1] <<endl;
        }
    }
    
    cout << "Sorting  : ,     " << d1 - d0 << ", [sec]" << endl;
}

