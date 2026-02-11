#include<stdio.h> 
#include<malloc.h>           //原神、崩坏星穹铁道、绝区零等游戏抽卡模型完全破解代码。
#include<math.h>             //程序说明：在第5行和第6行的NUM_CHAR与NUM_GEAR处输入抽取的目标角色数和武器数目(满命或满魂是7个不是6个)，运行程序会自动打印输出抽数概率分布列和成功率
#define GAME 0               //0为原神，1为星穹铁道，2为绝区零，3为鸣潮，4为少女前线2，5为重返未来1999
#define NUM_CHAR 0           //角色数目
#define NUM_GEAR 0           //武器数目(这里将角色使用的用来攻击的物品统一称为武器)，注意重返未来1999无武器池
#define GENSHIN_IMPACT 0
#define HONGKAI_STARRAIL 1
#define ZENLESS_ZONEZERO 2
#define WUTHERING_WAVES 3
#define GIRLS_FRONTLINE2_EXILIUM 4
#define REVERSE1999 5
typedef struct {
	int pity_count;     //保底抽数
	double base_rate;   //基础概率
	double up_rate;     //出五星时UP概率
	double fail_rate;   //出五星时歪的概率
}Banner;
typedef struct {
	char name[32];
	Banner character;   //角色池
	Banner gear;        //武器池
}Game;
const Game g[6] = { { "Genshin_Impact",{90,0.006,0.5,0.5},{80,0.007,0.75,0.25} },
	{ "Hongkai_Starrail",{90,0.006,0.5625,0.4375},{80,0.008,0.78125,0.21875} },//崩铁角色池实际不歪概率为56.25%，光锥池实际不歪概率为78.125%
	{ "Zenless_Zonezero",{90,0.006,0.5,0.5},{80,0.01,0.75,0.25} },
	{ "Wuthering_Waves",{80,0.008,0.5,0.5},{80,0.008,1.0,0.0}},
	{ "Girls_frontline2_exilium",{80,0.006,0.5,0.5},{80,0.007,0.75,0.25}},
	{ "Reverse1999",{70,0.015,0.5,0.5},{0,0.0,0.0,0.0} } };         //重返未来1999无武器池
void Genshin_Impact(double* char_pos_arr, double* gear_pos_arr);
void Hongkai_Starrail(double* char_pos_arr, double* gear_pos_arr);
void Zenless_Zonezero(double* char_pos_arr, double* gear_pos_arr);
void Wuthering_Waves(double* char_pos_arr, double* gear_pos_arr);
void Girls_frontline2_exilium(double* char_pos_arr, double* gear_pos_arr);
void Reverse1999(double* char_pos_arr, double* gear_pos_arr);
double benchou(int n, double* p) {
	if (n < 1) return 0.0;
	double mult = 1.0;
	for (int k = 1; k < n; k++) {
		mult *= (1.0 - p[k]);
	}
	return mult * p[n];
}
double upwai(int n, double* p, int num, double up_pos) {//计算歪过一次再出UP的情况的总概率
	double proba = 0.0;
	double nup = 1.0 - up_pos;
	if (n >= 2 && n <= num) {
		for (int k = 1; k <= n - 1; k++) {
			proba += benchou(k, p) * nup * benchou(n - k, p);
		}
	}
	else if (n > num && n <= 2 * num) {
		for (int k = 1; k <= num; k++) {
			if (n - k <= num) {
				proba += benchou(k, p) * nup * benchou(n - k, p);
			}
		}
	}
	return proba;
}
void game_select(int i, double* char_pos_arr, double* gear_pos_arr) {
	switch (i)
	{
	case GENSHIN_IMPACT:
		Genshin_Impact(char_pos_arr, gear_pos_arr); break;
	case HONGKAI_STARRAIL:
		Hongkai_Starrail(char_pos_arr, gear_pos_arr); break;
	case ZENLESS_ZONEZERO:
		Zenless_Zonezero(char_pos_arr, gear_pos_arr); break;
	case WUTHERING_WAVES:
		Wuthering_Waves(char_pos_arr, gear_pos_arr); break;
	case GIRLS_FRONTLINE2_EXILIUM:
		Girls_frontline2_exilium(char_pos_arr, gear_pos_arr); break;
	case REVERSE1999:
		Reverse1999(char_pos_arr, gear_pos_arr); break;
	default:
		printf("illegal GAME value!\n"); break;
	}
}
void Genshin_Impact(double* char_pos_arr, double* gear_pos_arr) {
	for (int i = 1; i <= 73; i++) {
		char_pos_arr[i] = 0.006;
	}
	for (int i = 74; i <= 89; i++) {
		char_pos_arr[i] = 0.006 + 0.06 * (i - 73);
	}
	char_pos_arr[90] = 1.00;
	for (int i = 1; i <= 62; i++) {
		gear_pos_arr[i] = 0.007;
	}
	for (int i = 63; i <= 76; i++) {
		gear_pos_arr[i] = 0.007 + 0.07 * (i - 62);
	}
	for (int i = 77; i <= 80; i++) {
		gear_pos_arr[i] = 1.00;
	}
}
void Hongkai_Starrail(double* char_pos_arr, double* gear_pos_arr) {
	for (int i = 1; i <= 73; i++) {
		char_pos_arr[i] = 0.006;
	}
	for (int i = 74; i <= 89; i++) {
		char_pos_arr[i] = 0.006 + 0.06 * (i - 73);
	}
	char_pos_arr[90] = 1.0;
	for (int i = 1; i <= 66; i++) {
		gear_pos_arr[i] = 0.008;
	}
	for (int i = 67; i < 71; i++) {
		gear_pos_arr[i] = 0.008 + 0.112 * (i - 66);
	}
	for (int i = 71; i <= 79; i++) {
		gear_pos_arr[i] = 0.456 + 0.056 * (i - 70);
	}
	gear_pos_arr[80] = 1.00;
}
void Zenless_Zonezero(double* char_pos_arr, double* gear_pos_arr) {
	for (int i = 1; i <= 73; i++) {
		char_pos_arr[i] = 0.006;
	}
	for (int i = 74; i <= 89; i++) {
		char_pos_arr[i] = 0.006 + 0.06 * (i - 73);
	}
	char_pos_arr[90] = 1.0;
	for (int i = 1; i <= 64; i++) {
		gear_pos_arr[i] = 0.01;
	}
	for (int i = 65; i <= 79; i++) {
		gear_pos_arr[i] = 0.01 + 0.06 * (i - 64);
	}
	gear_pos_arr[80] = 1.00;
}
void Wuthering_Waves(double* char_pos_arr, double* gear_pos_arr) {
	for (int i = 1; i <= 65; i++) {
		char_pos_arr[i] = 0.008;
	}
	for (int i = 66; i <= 70; i++) {
		char_pos_arr[i] = 0.008 + 0.04 * (i - 65);
	}
	for (int i = 71; i <= 75; i++) {
		char_pos_arr[i] = 0.208 + 0.08 * (i - 70);
	}
	for (int i = 76; i <= 78; i++) {
		char_pos_arr[i] = 0.608 + 0.1 * (i - 75);
	}
	char_pos_arr[79] = 1.00; char_pos_arr[80] = 1.0;
	for (int i = 1; i <= 65; i++) {
		gear_pos_arr[i] = 0.008;
	}
	for (int i = 66; i <= 70; i++) {
		gear_pos_arr[i] = 0.008 + 0.04 * (i - 65);
	}
	for (int i = 71; i <= 75; i++) {
		gear_pos_arr[i] = 0.208 + 0.08 * (i - 70);
	}
	for (int i = 76; i <= 78; i++) {
		gear_pos_arr[i] = 0.608 + 0.1 * (i - 75);
	}
	gear_pos_arr[79] = 1.00; gear_pos_arr[80] = 1.0;
}
void Girls_frontline2_exilium(double* char_pos_arr, double* gear_pos_arr) {
	for (int i = 1; i <= 58; i++) {
		char_pos_arr[i] = 0.006;
	}
	for (int i = 59; i <= 79; i++) {
		char_pos_arr[i] = 0.006 + 0.047 * (i - 58);
	}
	char_pos_arr[80] = 1.00;
	for (int i = 1; i <= 58; i++) {
		gear_pos_arr[i] = 0.007;
	}
	for (int i = 59; i <= 76; i++) {
		gear_pos_arr[i] = 0.007 + 0.053 * (i - 58);
	}
	gear_pos_arr[77] = 1.00;
}
void Reverse1999(double* char_pos_arr, double* gear_pos_arr) {
	for (int i = 1; i <= 60; i++) {
		char_pos_arr[i] = 0.015;
	}
	for (int i = 61; i <= 69; i++) {
		char_pos_arr[i] = 0.015 + 0.025 * (i - 60);
	}
	char_pos_arr[70] = 1.00;
	for (int i = 1; i <= 80; i++) {
		gear_pos_arr[i] = 0.00;
	}
}
int main() {
	double charfive_pos[91]; double gearfive_pos[81];
	if (!(GAME == 0 || GAME == 1 || GAME == 2 || GAME == 3 || GAME == 4 || GAME == 5)) {
		printf("illegal GAME value! Please enter correct GAME value!");
		return 0;
	}
	Game game = g[GAME];
	game_select(GAME,charfive_pos, gearfive_pos);
	double* char_up_pos = (double*)calloc(2 * game.character.pity_count + 1, sizeof(double));
	double* gear_up_pos = (double*)calloc(2 * game.gear.pity_count + 1, sizeof(double));
	for (int a = 1; a <= 2 * game.character.pity_count; a++) {
		if (a == 1) {
			char_up_pos[a] = benchou(a, charfive_pos) * game.character.up_rate;
		}
		else if (a >= 2 && a <= game.character.pity_count) {
			char_up_pos[a] = benchou(a, charfive_pos) * game.character.up_rate + upwai(a, charfive_pos, game.character.pity_count, game.character.up_rate);
		}
		else if (a > game.character.pity_count && a <= 2 * game.character.pity_count) {
			char_up_pos[a] = upwai(a, charfive_pos, game.character.pity_count, game.character.up_rate);
		}
	}
	for (int a = 1; a <= 2 * game.gear.pity_count; a++) {
		if (a == 1) {
			gear_up_pos[a] = benchou(a, gearfive_pos) * game.gear.up_rate;
		}
		else if (a >= 2 && a <= game.gear.pity_count) {
			gear_up_pos[a] = benchou(a, gearfive_pos) * game.gear.up_rate + upwai(a, gearfive_pos, game.gear.pity_count, game.gear.up_rate);
		}
		else if (a > game.gear.pity_count && a <= 2 * game.gear.pity_count) {
			gear_up_pos[a] = upwai(a, gearfive_pos, game.gear.pity_count, game.gear.up_rate);
		}
	}
	const int num_chars = NUM_CHAR;
	const int num_gears = NUM_GEAR;
	const int max_numch = num_chars * 2 * game.character.pity_count;
	const int max_numge = num_gears * 2 * game.gear.pity_count;
	double** dchar = (double**)calloc(num_chars + 1, sizeof(double*));
	for (int i = 0; i < num_chars + 1; i++) {
		dchar[i] = (double*)calloc(max_numch + 1, sizeof(double));
	}
	dchar[0][0] = 1.0;
	if (num_chars > 0) {
		for (int i = 1; i <= num_chars; i++) {
			for (int j = 0; j <= max_numch; j++) {
				if (dchar[i - 1][j] > 0) {
					for (int k = 1; k <= 2 * game.character.pity_count; k++) {
						if (j + k <= max_numch) {
							dchar[i][j + k] += dchar[i - 1][j] * char_up_pos[k];
						}
					}
				}
			}
		}
	}
	double** dgear = (double**)calloc(num_gears + 1, sizeof(double*));
	for (int i = 0; i < num_gears + 1; i++) {
		dgear[i] = (double*)calloc(max_numge + 1, sizeof(double));
	}
	dgear[0][0] = 1.0;
	if (num_gears > 0) {
		for (int i = 1; i <= num_gears; i++) {
			for (int j = 0; j <= max_numge; j++) {
				if (dgear[i - 1][j] > 0) {
					for (int k = 1; k <= 2 * game.gear.pity_count; k++) {
						if (j + k <= max_numge) {
							dgear[i][j + k] += dgear[i - 1][j] * gear_up_pos[k];
						}
					}
				}
			}
		}
	}//由各自的单个UP物品计算多个UP物品的卷积概率
	double* char_prob_dist = (double*)calloc(max_numch + 1, sizeof(double));
	for (int j = 0; j <= max_numch; j++) {
		char_prob_dist[j] = dchar[num_chars][j];
	}
	double* gear_prob_dist = (double*)calloc(max_numge + 1, sizeof(double));
	for (int j = 0; j <= max_numge; j++) {
		gear_prob_dist[j] = dgear[num_gears][j];
	}
	double* cum_char_prob = (double*)calloc(max_numch + 1, sizeof(double));
	for (int i = 0; i < max_numch; i++) {
		cum_char_prob[i + 1] = cum_char_prob[i] + char_prob_dist[i + 1];
	}
	double* cum_gear_prob = (double*)calloc(max_numge + 1, sizeof(double));
	for (int i = 1; i <= max_numge; i++) {
		cum_gear_prob[i] = cum_gear_prob[i - 1] + gear_prob_dist[i];
	}
	double* total_prob = (double*)calloc(max_numch + max_numge + 1, sizeof(double));
	for (int i = 0; i <= max_numch; i++) {
		for (int j = 0; j <= max_numge; j++) {
			if (i + j <= max_numch + max_numge) {
				total_prob[i + j] += char_prob_dist[i] * gear_prob_dist[j];
			}
		}
	}//由角色和武器各自的概率分布概率计算总的卷积概率分布
	double* cum_total_prob = (double*)calloc(max_numch + max_numge + 1, sizeof(double));
	for (int i = 0; i < max_numch + max_numge; i++) {
		cum_total_prob[i + 1] = cum_total_prob[i] + total_prob[i + 1];
	}
	double exp = 0.0; double var = 0.0;
	for (int i = 0; i <= max_numch + max_numge; i++) {
		exp += i * total_prob[i];
	}
	for (int i = 0; i <= max_numch + max_numge; i++) {
		var += pow((i - exp), 2) * total_prob[i];
	}
	printf("game selected:%s\n", g[GAME].name);
	/*printf("characters:\n");
	for (int i = 1; i <= max_numch; i++) {
		printf("%d:%.13e\n", i, char_prob_dist[i]);
		printf("%.13e\n", cum_char_prob[i]);
		printf("\n");
	}
	printf("gears:\n");
	for (int j = 1; j <= max_numge; j++) {
		printf("%d:%.13e\n", j, gear_prob_dist[j]);
		printf("%.13e\n", cum_gear_prob[j]);
		printf("\n");
	}*/
	printf("total:\n");
	for (int k = 1; k <= max_numch + max_numge; k++) {
		printf("%d | %g\n", k, total_prob[k]);
	}
	printf("\n");
	for (int k = 1; k <= max_numch + max_numge; k++) {
		printf("%d | %g\n", k, cum_total_prob[k]);
	}
	printf("---------------\nimportant points:\n");
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.1) {
			printf("10%% at:%d\n",i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.2) {
			printf("20%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.25) {
			printf("25%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.3) {
			printf("30%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.4) {
			printf("40%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.5) {
			printf("50%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.6) {
			printf("60%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.7) {
			printf("70%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.75) {
			printf("75%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.8) {
			printf("80%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.9) {
			printf("90%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.95) {
			printf("95%% at:%d\n", i);
			break;
		}
	}
	for (int i = 1; i <= max_numch + max_numge; i++) {
		if (cum_total_prob[i] >= 0.99) {
			printf("99%% at:%d\n", i);
			break;
		}
	}
	printf("---------------\n");
	double p1 = 0.0, p2 = 0.0, p3 = 0.0; bool judge1 = false, judge2 = false, judge3 = false;
	printf("expectation:%lf\n", exp);                                //期望
	printf("Var:%lf\n", var);                                        //方差
	printf("Standard Deviation:%lf\n", sqrt(var));                   //标准差
	printf("average possibility:%lf\n", 1.0 / exp);                  //平均概率
	printf("1sigma:(%lf,%lf)\n", exp - 1 * sqrt(var), exp + 1 * sqrt(var));   //1倍σ范围
	printf("2sigma:(%lf,%lf)\n", exp - 2 * sqrt(var), exp + 2 * sqrt(var));   //2倍σ范围
	printf("3sigma:(%lf,%lf)\n", exp - 3 * sqrt(var), exp + 3 * sqrt(var));   //3倍σ范围
	if (exp - 1 * sqrt(var) > 0 && exp + 1 * sqrt(var) < max_numch + max_numge) {       //计算σ，2σ和3σ区间的概率值，辅助判断是否可使用正态分布近似拟合
		judge1 = true;                                                                  //参考：标准正态分布1σ概率值为68.26%，2σ为95.44%，3σ为99.74%
		for (int i = int(exp - 1 * sqrt(var)); i <= int(exp + 1 * sqrt(var)); i++) {
			p1 += total_prob[i];
		}
	}
	else {
		printf("illegal 1sigma value.\n");
	}
	if (exp - 2 * sqrt(var) > 0 && exp + 2 * sqrt(var) < max_numch + max_numge) {
		judge2 = true;
		for (int i = int(exp - 2 * sqrt(var)); i <= int(exp + 2 * sqrt(var)); i++) {
			p2 += total_prob[i];
		}
	}
	else {
		printf("illegal 2sigma value.\n");
	}
	if (exp - 3 * sqrt(var) > 0 && exp + 3 * sqrt(var) < max_numch + max_numge) {
		judge3 = true;
		for (int i = int(exp - 3 * sqrt(var)); i <= int(exp + 3 * sqrt(var)); i++) {
			p3 += total_prob[i];
		}
	}
	else {
		printf("illegal 3sigma value.\n");
	}
	if (judge1 && judge2 && judge3) {
		printf("1sigma:%lf 2sigma:%lf 3sigma:%lf\n", p1, p2, p3);
		printf("reference value:1sigma,0.6826;2sigma,0.9544;3sigma,0.9974\n");
	}
	free(char_up_pos);
	free(gear_up_pos);
	for (int i = 0; i < num_chars + 1; i++) {
		free(dchar[i]);
	}
	free(dchar);
	for (int i = 0; i < num_gears + 1; i++) {
		free(dgear[i]);
	}
	free(dgear);
	free(char_prob_dist);
	free(gear_prob_dist);
	free(cum_char_prob);
	free(cum_gear_prob);
	free(total_prob);
	free(cum_total_prob);
	return 0;
}
