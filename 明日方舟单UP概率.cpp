#include<stdio.h>
#include<math.h>
#define UP_CHAR_NUM 1
const int pity=99;
const int count=30;
const int total=400;
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
    }double proba[pity*count+1]={0.0};double cum[pity*count+1]={0.0};
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
    double p1[total+1]={0.0};
    for(int i=1;i<=total;i++){
        for(int j=1;j<=count;j++){
            p1[i]+=dp2[j][i];
        }
    }
    const int up_num=UP_CHAR_NUM;
    double final[up_num*249+1]={0.0};
    double d_up[up_num+1][up_num*249+1]={0.0};
    d_up[0][0]=1.00;
    for(int i=1;i<=up_num;i++){
        for(int j=0;j<=up_num*249;j++){
            if(d_up[i-1][j]>0){
                for(int k=1;k<=249;k++){
                    if(j+k<=up_num*249){
                        d_up[i][j+k]+=d_up[i-1][j]*proba[k];
                    }
                }
            }
        }
    }
    for(int j=0;j<=up_num*249;j++){
        final[j]=d_up[up_num][j];
    }
    for(int i=1;i<=up_num*249;i++){
        cum[i]=cum[i-1]+final[i];
    }
    for(int i=1;i<=up_num*249;i++){
        printf("%d %g\n",i,final[i]);
    }
    printf("--------------------\n");
    double exp=0.0,var=0.0;
    for(int i=1;i<=up_num*249;i++){
        printf("%d %g\n",i,cum[i]);
        exp+=i*final[i];
    }
    for(int i=1;i<=up_num*249;i++){
        var+=pow(i-exp,2)*final[i];
    }
    printf("--------------------\n");
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.1) {
            printf("10%% at:%d\n",i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.2) {
            printf("20%% at:%d\n",i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.25) {
            printf("25%% at:%d\n",i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.3) {
            printf("30%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.4) {
            printf("40%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.5) {
            printf("50%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.6) {
            printf("60%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.7) {
            printf("70%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.75) {
            printf("75%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.8) {
            printf("80%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.9) {
            printf("90%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.95) {
            printf("95%% at:%d\n", i);
            break;
        }
    }
    for (int i = 1; i <= up_num*249; i++) {
        if (cum[i] >= 0.99) {
            printf("99%% at:%d\n", i);
            break;
        }
    }
    printf("--------------------\n");
    printf("expectation:%lf\n",exp);
    printf("var:%lf\n",var);
    printf("standard deviation:%lf\n",sqrt(var));
    return 0;
}
