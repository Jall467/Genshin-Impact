#include<stdio.h>
#include<math.h>
const int pity=99;
const int count=20;
double benchou(int n, double* p) {
    if (n < 1) return 0.0;

    double mult = 1.0;
    for (int k = 1; k < n; k++) {
        mult *= (1.0 - p[k]);
    }
    return mult * p[n];
}
int main(){
    double p[pity+1];
    for(int i=0;i<=50;i++){
        p[i]=0.02;
    }
    for(int i=51;i<=99;i++){
        p[i]=0.02+0.02*(i-50);
    }
    double ben[pity+1];
    for(int i=1;i<=pity;i++){
        ben[i]=benchou(i,p);
    }
    double dp1[count+1][pity*count+1]={0.0};double dp2[count+1][pity*count+1]={0.0};
    dp1[0][0]=1.0;dp2[0][0]=1.0;
    for(int i=1;i<=count;i++){
        for(int j=0;j<=pity*count;j++){
            if(dp1[i-1][j]>0) {
                for (int k = 1; k <= pity; k++) {
                    if (j + k <= pity*count) {
                        dp1[i][j + k] += dp1[i - 1][j] * ben[k] * 0.5;
                        dp2[i][j + k] += dp1[i - 1][j] * ben[k] * 0.5;
                    }
                }
            }
        }
    }
    double pn[151]={0.0};
    for(int i=1;i<=150;i++){
        for(int j=1;j<=20;j++){
            pn[i]+=dp1[j][i];
        }
    }
    double sum=0.0;double proba[pity*count+1]={0.0};double cum[pity*count+1]={0.0};
    for(int i=1;i<=150;i++){
        for(int j=1;j<=count;j++){
            proba[i]+=dp2[j][i];
        }
    }
    for(int i=151;i<=249;i++){
        for(int j=1;j<=150;j++){
            if(i-j<=99){
                proba[i]+=pn[j]*ben[i-j];
            }
        }
    }
    for(int i=1;i<=249;i++){
        sum+=proba[i];
        cum[i]=cum[i-1]+proba[i];
    }
    for(int i=1;i<=249;i++){
        printf("%d %e\n",i,proba[i]);
    }
    printf("\n");
    double exp=0.0,var=0.0;
    for(int i=1;i<=249;i++){
        printf("%d %e\n",i,cum[i]);
        exp+=i*proba[i];
    }
    for(int i=1;i<=249;i++){
        var+=pow(i-exp,2)*proba[i];
    }
    printf("expectation:%lf\n",exp);
    printf("sum:%lf\n",sum);
    printf("var:%lf\n",var);
    printf("standard deviation:%lf\n",sqrt(var));
    return 0;
}
