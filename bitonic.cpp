#include <cstdio>
#include <cstdlib>
#include <utility>
using namespace std;

void bitonicsort(int lgn, int *ary) {
    int tmp;
    for(int fb=1; fb<=lgn; fb++) {
        printf("fb %d\n", fb);
        for(int sb=fb-1; sb>=0; sb--) {
            printf(" sb  %d\n", sb);
            // this loop can be parallelized
            for(int i = 0; i < (1<<lgn); i++) {
                if((i>>fb)&1^(i>>sb)&1){
                    printf("   %d : %d\n", i, i^(1<<sb));
                    if(ary[i] < ary[i^(1<<sb)]) {
                        tmp = ary[i];
                        ary[i] = ary[i^(1<<sb)];
                        ary[i^(1<<sb)] = tmp;
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    srand(99);
    int lgn = 2;
    int *ary = new int[1<<lgn];
    for(int i = 0; i < (1<<lgn); i++) {
        ary[i] = rand()%10000;
    }
    
    bitonicsort(lgn, ary);
    
    for(int i = 0; i < (1<<lgn); i++) {
        // printf("[%d] : %d\n", i, ary[i]);
    }
    
    printf("\n");
    return 0;
}
