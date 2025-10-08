#include<stdio.h>
#include<string.h>
const int num1 = 90;
const int num2 = 80;
const double nup1 = 0.5;
const double nup2 = 0.25;
double benchou(int n, double* p, int c) {//c只取0或1，为控制角色或光锥的变量，0为角色，1为光锥
	double mult = 1.0;
	if (n == 1) {
		if (c == 0) {
			mult = 0.006;
		}
		else if (c == 1) {
			mult = 0.008;
		}
	}
	for (int k = 1; k < n; k++) {
		mult *= (1 - p[k]);
		if (k == n - 1) {
			mult *= p[n];
		}
	}
	return mult;
}
double upwai(int n, double* p, int c) {//计算歪过一次再出UP的情况的总概率
	double proba = 0.0;
	int num; double nup;
	if (c == 0) {
		num = num1; nup = nup1;
	}
	else if (c == 1) {
		num = num2; nup = nup2;
	}
	if (n >= 2 && n <= num) {
		for (int k = 1; k <= n - 1; k++) {
			proba += benchou(k, p, c) * nup * benchou(n - k, p, c);
		}
	}
	else if (n > num && n <= 2 * num) {
		for (int k = 1; k <= num; k++) {
			if (n - k <= num) {
				proba += benchou(k, p, c) * nup * benchou(n - k, p, c);
			}
		}
	}
	return proba;
}
double nchoubuchu(int n, double* p, int c) {
	return benchou(n + 1, p, c) / p[n + 1];
}
double upleijinwai(int n, double* p, int c) {//计算歪过一次之后到第n抽没有出UP的概率
	double prob = 0.0;
	int num; double nup;
	if (c == 0) {
		num = num1; nup = nup1;
	}
	else if (c == 1) {
		num = num2; nup = nup2;
	}
	if (n >= 2 && n <= num) {
		for (int k = 1; k <= n - 1; k++) {
			prob += benchou(k, p, c) * nup * nchoubuchu(n - k, p, c);
		}
	}
	else if (n > num && n <= 2 * num) {
		for (int k = 1; k <= num; k++) {
			if (n - k < num) {
				prob += benchou(k, p, c) * nup * nchoubuchu(n - k, p, c);
			}
		}
	}
	return prob;
}
int main() {
	double charfive_pos[91]; double guangzhuifive_pos[81];
	for (int i = 1; i <= 73; i++) {
		charfive_pos[i] = 0.006;
	}
	for (int i = 74; i <= 89; i++) {
		charfive_pos[i] = 0.006 + 0.06 * (i - 73);
	}
	charfive_pos[90] = 1.00;
	for (int j = 1; j <= 66; j++) {
		guangzhuifive_pos[j] = 0.008;
	}
	for (int j = 67; j < 71; j++) {
		guangzhuifive_pos[j] = 0.008 + 0.112 * (j - 66);
	}
	for (int j = 71; j <= 79; j++) {
		guangzhuifive_pos[j] = 0.456 + 0.056 * (j - 70);
	}
	guangzhuifive_pos[80] = 1.00;
	double charuppos[181]; double guazhupos[161];
	double charleijin[181]; double guazhleijin[161];
	for (int a = 1; a <= 180; a++) {
		if (a == 1) {
			charuppos[a] = benchou(a, charfive_pos, 0) * 0.5;
		}
		else if (a >= 2 && a <= 90) {
			charuppos[a] = benchou(a, charfive_pos, 0) * 0.5 + upwai(a, charfive_pos, 0);
		}
		else if (a > 90 && a <= 180) {
			charuppos[a] = upwai(a, charfive_pos, 0);
		}
	}
	for (int a = 1; a <= 160; a++) {
		if (a == 1) {
			guazhupos[a] = benchou(a, guangzhuifive_pos, 1) * 0.75;
		}
		else if (a >= 2 && a <= 80) {
			guazhupos[a] = benchou(a, guangzhuifive_pos, 1) * 0.75 + upwai(a, guangzhuifive_pos, 1);
		}
		else if (a >= 80 && a <= 160) {
			guazhupos[a] = upwai(a, guangzhuifive_pos, 1);
		}
	}
	for (int b = 1; b <= 180; b++) {
		if (b == 1) {
			charleijin[b] = 1.0 - benchou(b, charfive_pos, 0) * 0.5;
		}
		else if (b >= 2 && b < 90) {
			charleijin[b] = nchoubuchu(b, charfive_pos, 0) + upleijinwai(b, charfive_pos, 0) + benchou(b, charfive_pos, 0) * 0.5;
		}
		else if (b == 90) {
			charleijin[b] = benchou(b, charfive_pos, 0) * 0.5 + upleijinwai(b, charfive_pos, 0);
		}
		else if (b > 90) {
			charleijin[b] = upleijinwai(b, charfive_pos, 0);
		}

	}
	for (int b = 1; b <= 160; b++) {
		if (b == 1) {
			guazhleijin[b] = 1.0 - benchou(b, guangzhuifive_pos, 1) * 0.75;
		}
		else if (b >= 2 && b < 80) {
			guazhleijin[b] = nchoubuchu(b, guangzhuifive_pos, 1) + upleijinwai(b, guangzhuifive_pos, 1) + benchou(b, guangzhuifive_pos, 1) * 0.25;
		}
		else if (b == 80) {
			guazhleijin[b] = benchou(b, guangzhuifive_pos, 1) * 0.25 + upleijinwai(b, guangzhuifive_pos, 1);
		}
		else if (b > 80) {
			guazhleijin[b] = upleijinwai(b, guangzhuifive_pos, 1);
		}
	}//初始化本抽和累进概率列表
	const int num_chars = 3;
	const int num_guang = 1;
	const int max_numch = num_chars * 180;
	const int max_numgu = num_guang * 160;
	double dchar[num_chars + 1][max_numch + 1];
	memset(dchar, 0, sizeof(dchar));
	dchar[0][0] = 1.0;
	for (int i = 1; i <= num_chars; i++) {
		for (int j = 0; j <= max_numch; j++) {
			if (dchar[i - 1][j] > 0) {
				for (int k = 1; k <= 2 * num1; k++) {
					if (j + k <= max_numch) {
						dchar[i][j + k] += dchar[i - 1][j] * charuppos[k];
					}
				}
			}
		}
	}
	double dguan[num_guang + 1][max_numgu + 1];
	memset(dguan, 0, sizeof(dguan));
	dguan[0][0] = 1.0;
	for (int i = 1; i <= num_guang; i++) {
		for (int j = 0; j <= max_numgu; j++) {
			if (dguan[i - 1][j] > 0) {
				for (int k = 1; k <= 2 * num2; k++) {
					if (j + k <= max_numgu) {
						dguan[i][j + k] += dguan[i - 1][j] * guazhupos[k];
					}
				}
			}
		}
	}//由各自的单个UP物品计算多个UP物品的卷积概率
	double xinhun[max_numch + 1] = { 0.0 };
	for (int j = 0; j <= max_numch; j++) {
		xinhun[j] = dchar[num_chars][j];
	}
	double dieyin[max_numgu + 1] = { 0.0 };
	for (int j = 0; j <= max_numgu; j++) {
		dieyin[j] = dguan[num_guang][j];
	}
	double leijinxinhun[max_numch + 1] = { 0.0 };
	for (int i = 1; i <= max_numch; i++) {
		leijinxinhun[i] = leijinxinhun[i - 1] + xinhun[i];
	}
	double leijindieyin[max_numgu+1] = { 0.0 };
	for (int i = 1; i <= max_numgu; i++) {
		leijindieyin[i] = leijindieyin[i - 1] + dieyin[i];
	}
	double totalprob[max_numch + max_numgu + 1] = { 0.0 };
	for (int i = 0; i <= max_numch; i++) {
		for (int j = 0; j <= max_numgu; j++) {
			if (i + j <= max_numch + max_numgu) {
				totalprob[i + j] += xinhun[i] * dieyin[j];
			}
		}
	}//由角色和光锥各自概率累加计算总共的概率
	double leijintotalprob[max_numch + max_numgu + 1] = { 0.0 };
	for (int i = 0; i <= max_numch + max_numgu; i++) {
		leijintotalprob[i + 1] = leijintotalprob[i] + totalprob[i + 1];
	}
	//printf("characters:\n");
	//for (int i = 1; i <= max_numch; i++) {
	//	printf("%d:%.50lf\n", i, xinhun[i]);
	//	printf("%.50lf\n", leijinxinhun[i]);
	//	printf("\n");
	//}
	//printf("guangzhui:\n");
	//for (int j = 1; j <= max_numgu; j++) {
	//	printf("%d:%.50lf\n", j, dieyin[j]);
	//	printf("%.50lf\n", leijindieyin[j]);
	//	printf("\n");
	//}
	printf("total:\n");
	for (int k = 1; k <= max_numch + max_numgu; k++) {
		printf("%d:%.50lf\n", k, totalprob[k]);
		printf("%.50lf\n", leijintotalprob[k]);
		printf("\n");
	}
	return 0;
}
