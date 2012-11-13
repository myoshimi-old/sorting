#include<iostream>

#include<sys/time.h>
#include<omp.h>
#include<math.h>

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

void quick_sort(REAL a[], int left, int right, int flag){
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
        quick_sort(a, left, i-1, flag);
        quick_sort(a, j+1, right, flag);
    }
}

void merge_sort(REAL sa1[], REAL sa2[], REAL da1[], REAL da2[], int num){
    int i;
    int ps, qs;
    
    ps = 0; qs = 0;

    for(i=0;i<num*2;i++){
        if(ps >= num){
            if(i < num) da1[i] = sa2[qs];
            else        da2[i-num] = sa2[qs];
            qs++;
        }
        else if(qs >= num){
            if(i < num) da1[i] = sa1[ps];
            else        da2[i-num] = sa1[ps];
            ps++;
        }
        else{
            if(sa1[ps] < sa2[qs]){
                if(i < num) da1[i] = sa1[ps];
                else        da2[i-num] = sa1[ps];
                ps++;
            }
            else{
                if(i < num) da1[i] = sa2[qs];
                else        da2[i-num] = sa2[qs];
                qs++;
            }
        }
    }
}

int main(int argc, char** argv){
    int i;
    int num;
    REAL* array;
    REAL* tmp_array;
    double d0, d1, d2;

    int order = ASC;
    
    if(argc != 2){
        cout << argv[0] << " [num]" << endl;
        exit(1);
    }

    num = atoi(argv[1]);
    array = (REAL*)malloc(sizeof(REAL)*num);

    srand((unsigned int)time(NULL));
    
    for(i=0;i<num;i++){
        array[i] = (REAL)rand() / (REAL)RAND_MAX;
    }

#ifdef DEBUG
    for(i=0;i<num;i++){
        cout << i << " : " << array[i] << endl;
    }
#endif
    
    int k;
    REAL* array2;
    array2 = (REAL*)malloc(sizeof(REAL)*num);

    d0 = get_dtime();
    
    int dnum    = 16;
    int blkelem = num / dnum;
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(k=0;k<dnum;k++){
        // cout << "QSort : " << k * blkelem << " : " << (k+1)*blkelem-1 << endl;
        quick_sort(array, k*blkelem, (k+1)*blkelem-1, ASC);
    }

    d1 = get_dtime();

    for(int fb=1; fb<=(int)log2(dnum); fb++) {
        // printf("fb %d\n", fb);
        for(int sb=fb-1; sb>=0; sb--) {
            // printf(" sb  %d\n", sb);
            // this loop can be parallelized
            //for(int i = 0; i < 1<<(int)log2(dnum)); i++) {
#ifdef _OPENMP
#pragma omp parallel for
#endif
            for(int i = 0; i < dnum; i++) {
                if((i>>fb)&1^(i>>sb)&1){
                    merge_sort(&array[(i^(1<<sb))*blkelem],  &array[i*blkelem],
                               &array2[(i^(1<<sb))*blkelem], &array2[i*blkelem],
                               blkelem);
                }
            }
            
            tmp_array = array2;
            array2 = array;
            array = tmp_array;

            /*
            for(int i=0;i<num;i++){
                cout << "[" << i << "] " << array[i] << endl;
            }
            */
            //cout << endl;
        }
    }

    d2 = get_dtime();

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
                     << "] : " << array[i] <<endl;
            }
        }
        else{
            if(array[i-1] < array[i]){
                cout << "num[" << i-1 << "] : " << array[i-1]
                     << " is smaller than num[" << i
                     << "] : " << array[i] <<endl;
            }
        }
    }

    cout << "Sorting  : ,     " << d2 - d0 << ", [sec],  (, " << (d1-d0) << ", " << (d2-d1) << ", )"<< endl;

    return 0;
}
