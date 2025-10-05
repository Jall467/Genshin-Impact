#include<stdio.h>
#include<math.h>
int main() {
    double a,b,c,d,A,B,C,delta,x1,x2,x3,p1,p2;
    printf("enter a,b,c,d:");
    scanf("%lf %lf %lf %lf",&a,&b,&c,&d);
    A=b*b-3*a*c;
    B=b*c-9*a*d;
    C=c*c-3*b*d;
    delta=B*B-4*A*C;
    if(A==0 && B==0){
        printf("It has three same roots:x1=x2=x3=%lf\n",-1.0*b/(3*a));
        return 0;
    }
    if(delta>0) {
        double y1=A*b+3*a*((-B+sqrt(delta))/2);
        double y2=A*b+3*a*((-B-sqrt(delta))/2);
        if(y1<0) {
            p1=-pow(-y1,1.0/3);
        }
        else {
            p1=pow(y1,1.0/3);
        }
        if(y2<0) {
            p2=-pow(-y2,1.0/3);
        }
        else {
            p2=pow(y2,1.0/3);
        }
        x1=(-b-p1-p2)/(3*a);
        double rp=(-b+0.5*(p1+p2))/(3*a);
        double ip=sqrt(3)/2*(p1-p2)/(3*a);
        printf("It has a real root and a pair of conjugate complex roots:x1=%lf,x2=%lf+%lfi,x3=%lf-%lfi\n",x1,rp,ip,rp,ip);
    }
    else if(delta==0) {
        x1=-b/a+B/A;
        x2=-B/(2*a);
        printf("It has three roots,a pair of same roots:x1=%lf,x2=x3=%lf\n",x1,x2);
    }
    else {
        double T=(2*A*b-3*a*B)/(2*pow(A,3.0/2));
        double theta=acos(T);
        x1=(-b-2*sqrt(A)*cos(theta/3))/(3*a);
        x2=(-b+sqrt(A)*(cos(theta/3)+sqrt(3)*sin(theta/3)))/(3*a);
        x3=(-b+sqrt(A)*(cos(theta/3)-sqrt(3)*sin(theta/3)))/(3*a);
        printf("It has three real roots:x1=%lf,x2=%lf,x3=%lf\n",x1,x2,x3);
    }
    return 0;
}
