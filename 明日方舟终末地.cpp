#include "stdio.h"
#include "stdlib.h"
#include "math.h"
double benchou(int n, double* p) {
    if (n < 1) return 0.0;
    double mult = 1.0;
    for (int k = 1; k < n; k++) {
        mult *= (1.0 - p[k]);
    }
    return mult * p[n];
}
int main(){
    double char_five_pos[81]={0.0};
    for(int i=1;i<=65;i++){
        char_five_pos[i]=0.008;
    }
    for(int i=66;i<=79;i++){
        char_five_pos[i]=0.008+0.05*(i-65);
    }
    char_five_pos[80]=1.00;
    double char_ben_pos[81]={0.0};
    for(int i=1;i<=29;i++){
        char_ben_pos[i]=benchou(i,char_five_pos);
    }
    char_ben_pos[30]=pow(1-0.008,29)*(1-pow(1-0.008,11));
    for(int i=31;i<=80;i++){
        char_ben_pos[i]=benchou(i,char_five_pos)*pow(1-0.008,10);
    }
    for(int i=1;i<=80;i++){
        printf("%d | %g\n",i,char_ben_pos[i]);
    }
    printf("--------------------\n");
    double cum_ben_five[81]={0.0};
    for(int i=1;i<=80;i++){
        cum_ben_five[i]=cum_ben_five[i-1]+char_ben_pos[i];
    }
    for(int i=1;i<=80;i++){
        printf("%d | %g\n",i,cum_ben_five[i]);
    }
    double exp=0.0,var=0.0;
    for(int i=1;i<=80;i++){
        exp+=i*char_ben_pos[i];
    }
    for(int i=1;i<=80;i++){
        var+= pow(i-exp,2)*char_ben_pos[i];
    }
    printf("--------------------\n");
    printf("expectation:%lf\n",exp);
    printf("var:%lf\n",var);
    printf("standard deviation:%lf\n", sqrt(var));
    return 0;
}
