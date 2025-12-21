#include<stdio.h>
#include<math.h>
double sgn(double n) {
    if (n == 0.0) {
        return 0.0;
    }
    if (n != 0.0) {
        return fabs(n) / n;
    }
}
int main() {
    printf("please enter a,b,c,d,e:");
    double a, b, c, d, e, A, B, C, D, E, F, x1, x2, x3, x4, delta;
    scanf_s("%lf %lf %lf %lf %lf", &a, &b, &c, &d, &e);
    D = 3 * b * b - 8 * a * c; E = -pow(b, 3) + 4 * a * b * c - 8 * a * a * d; F = 3 * pow(b, 4) + 16 * a * a * c * c - 16 * a * b * b * c + 16 * a * a * b * d - 64 * pow(a, 3) * e;
    A = D * D - 3 * F; B = D * F - 9 * E * E; C = F * F - 3 * D * E * E;
    delta = B * B - 4 * A * C;
    if (D == 0 && E == 0 && F == 0) {
        printf("It has four same real roots:x1=x2=x3=x4=%lf", -b / 4.0 * a);
    }
    if (D * E * F != 0 && A == 0 && B == 0 && C == 0) {
        x1 = (-b * D + 9 * E) / (4.0 * a * D); x2 = (-b * D - 3 * E) / (4.0 * a * D);
        printf("It has four roots, a pair of three same roots:x1=%lf,x2=x3=x4=%lf", x1, x2);
    }
    if (E == 0 && F == 0 && D != 0) {
        if (D > 0) {
            x1 = (-b + sqrt(D)) / (4.0 * a);
            x3 = (-b - sqrt(D)) / (4.0 * a);
            printf("It has two pair of same real roots:x1=x2=%lf,x3=x4=%lf", x1, x3);
        }
        else if (D < 0) {
            double rp = -b / 4.0 * a, ip = sqrt(-D) / 4.0 * a;
            printf("It has two pair of same complex roots and they are conjugate:x1=x2=%lf+%lfi,x3=x4=%lf-%lfi", rp, ip, rp, ip);
        }
    }
    if (A * B * C != 0 && delta == 0) {
        x1 = (-b - 2 * A * E / B) / 4.0 * a;
        if (A * B > 0) {
            x3 = (-b + 2 * A * E / B + sqrt(2 * B / A)) / (4.0 * a);
            x4 = (-b + 2 * A * E / B - sqrt(2 * B / A)) / (4.0 * a);
            printf("It has a pair of same real roots and two real roots:x1=x2=%lf,x3=%lf,x4=%lf", x1, x3, x4);
        }
        else if (A * B < 0) {
            double rp = (-b + 2 * A * E / B) / (4.0 * a), ip = sqrt(-2 * B / A) / (4.0 * a);
            printf("It has a pair of same real roots and a pair of conjugate complex roots:x1=x2=%lf,x3=%lf+%lf,x4=%lf-%lf", x1, rp, ip, rp, ip);
        }
    }
    if (delta > 0) {
        double z1 = A * D + 3 * ((-B + sqrt(delta)) / 2.0), z2 = A * D + 3 * ((-B - sqrt(delta)) / 2.0);
        double z = D * D - D * (cbrt(z1) + cbrt(z2)) + pow((cbrt(z1) + cbrt(z2)), 2) - 3 * A;
        x1 = (-b + sgn(E) * sqrt((D + cbrt(z1) + cbrt(z2)) / 3.0) + sqrt((2 * D - cbrt(z1) - cbrt(z2) + 2 * sqrt(z)) / 3)) / (4.0 * a);
        x2 = (-b + sgn(E) * sqrt((D + cbrt(z1) + cbrt(z2)) / 3.0) - sqrt((2 * D - cbrt(z1) - cbrt(z2) + 2 * sqrt(z)) / 3)) / (4.0 * a);
        double rp = (-b - sgn(E) * sqrt((D + cbrt(z1) + cbrt(z2)) / 3.0)) / (4.0 * a);
        double ip = sqrt((-2 * D + cbrt(z1) + cbrt(z2) + 2 * sqrt(z)) / 3) / (4.0 * a);
        printf("It has two real roots and a pair of complex conjugate roots:x1=%lf,x2=%lf,x3=%lf+%lfi,x4=%lf-%lfi", x1, x2, rp, ip, rp, ip);
    }
    if (delta < 0) {
        double theta = acos((3 * B - 2 * A * D) / (2 * A * sqrt(A)));
        double y1 = (D - 2 * sqrt(A) * cos(theta / 3.0)) / 3.0, y2 = (D + sqrt(A) * (cos(theta / 3.0) + sqrt(3) * sin(theta / 3.0))) / 3.0, y3 = (D + sqrt(A) * (cos(theta / 3.0) - sqrt(3) * sin(theta / 3.0))) / 3.0;
        if (E == 0 && D > 0 && F > 0) {
            x1 = (-b + sqrt(D + 2 * sqrt(F))) / (4.0 * a);
            x2 = (-b - sqrt(D + 2 * sqrt(F))) / (4.0 * a);
            x3 = (-b + sqrt(D - 2 * sqrt(F))) / (4.0 * a);
            x4 = (-b - sqrt(D - 2 * sqrt(F))) / (4.0 * a);
            printf("1.It has four roots:x1=%lf,x2=%lf,x3=%lf,x4=%lf", x1, x2, x3, x4);
        }
        else if (E == 0 && D < 0 && F>0) {
            double rp = -b / (4.0 * a), ip1 = sqrt(-D + 2 * sqrt(F)) / (4.0 * a), ip2 = sqrt(-D - 2 * sqrt(F)) / (4.0 * a);
            printf("1.It has two pair of complex conjugate roots:x1=%lf+%lfi,x2=%lf-%lfi,x3=%lf+%lfi,x4=%lf-%lfi", rp, ip1, rp, ip1, rp, ip2, rp, ip2);
        }
        else if (E == 0 && F < 0) {
            double rp1 = (-2 * b + sqrt(2 * D + 2 * sqrt(A - F))) / (8.0 * a), rp2 = (-2 * b - sqrt(2 * D + 2 * sqrt(A - F))) / (8.0 * a);
            double ip = sqrt(-2 * D + 2 * sqrt(A - F)) / (8.0 * a);
            printf("2.It has two pair of complex conjugate roots:x1=%lf+%lfi,x2=%lf-%lfi,x3=%lf+%lfi,x4=%lf-%lfi", rp1, ip, rp1, ip, rp2, ip, rp2, ip);
        }
        if (E != 0 && D > 0 && F > 0) {
            x1 = (-b + sgn(E) * sqrt(y1) + sqrt(y2) + sqrt(y3)) / (4.0 * a);
            x2 = (-b + sgn(E) * sqrt(y1) - sqrt(y2) - sqrt(y3)) / (4.0 * a);
            x3 = (-b - sgn(E) * sqrt(y1) + sqrt(y2) - sqrt(y3)) / (4.0 * a);
            x4 = (-b - sgn(E) * sqrt(y1) - sqrt(y2) + sqrt(y3)) / (4.0 * a);
            printf("2.It has four roots:x1=%lf,x2=%lf,x3=%lf,x4=%lf", x1, x2, x3, x4);
        }
        else if (E != 0 && (D < 0 || F < 0)) {
            double rp1 = (-b - sqrt(y2)) / (4.0 * a), rp2 = (-b + sqrt(y2)) / (4.0 * a);
            double ip1 = (sgn(E) * sqrt(-y1) + sqrt(-y3)) / (4.0 * a), ip2 = (sgn(E) * sqrt(-y1) - sqrt(-y3)) / (4.0 * a);
            printf("3.It has two pair of complex conjugate roots:x1=%lf+%lfi,x2=%lf-%lfi,x3=%lf+%lfi,x4=%lf-%lfi", rp1, ip1, rp1, ip1, rp2, ip2, rp2, ip2);
        }
    }
    return 0;
}
