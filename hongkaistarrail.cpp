#include <stdio.h>
const int num=80;
double benchou(int n,double p[]) {
    double mult=1.0;
    if(n==1) {
        mult=0.008;
    }
    else {
        for(int k=0;k<=n-1;k++) {
            mult=mult*(1-p[k]);
            if(k==n-1) {
                mult=mult*p[n];
            }
        }
    }
    return mult;
}
double leiji(int n,double p[]) {
    double sum=1.0;
    for(int k=1;k<=n;k++) {
        sum=sum*(1-p[k]);
    }
    sum=1-sum;
    return sum;
}
int main() {
    double p[num+1];
    for(int j=1;j<=66;j++) {
        p[j]=0.008;
    }
    for(int i=67;i<71;i++) {
        p[i]=0.008+0.112*(i-66);
    }
    for(int i=71;i<num;i++){
        p[i]=0.456+0.056*(i-70);
    }
    p[80]=1.0;
    int n;
    for(n=1;n<=num;n++) {
        printf("%d.benchou:%.10lf\n",n,benchou(n,p));
        printf("leiji:%.10lf\n",leiji(n,p));
        printf("\n");
    }
    double exp=0.0;
    for(int i1=1;i1<=num;i1++) {
        exp=exp+i1*benchou(i1,p);
    }
    printf("the expectation:%lf\n",exp);
    printf("average possibility:%lf\n",1.0/exp);
    double sum=0.0;
    for(int i=1;i<=num;i++) {
        sum=sum+benchou(i,p);
    }
    printf("the sum:%lf\n",sum);
    return 0;
}
//原神：基础概率0.007，第63抽开始概率线性递增每次0.07，直到大于1为第77抽，也就是实际保底次数；但是崩坏星穹铁道目前具体参数依旧未知，暂定为此机制。
