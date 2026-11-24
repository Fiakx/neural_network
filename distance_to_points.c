#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 8

int randint(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// f(a,b) = 1/(1+a^2) * sum (a*xi - yi + b)^2
double f(double a, double b, double x[], double y[]) {
    double s = 0;
    for (int i = 0; i < N; i++) {
        double e = a * x[i] - y[i] + b;
        s += e * e;
    }
    return s / (1 + a * a);
}

int main(void) {
    srand((unsigned)time(NULL));

    // points aléatoires    double x[N], y[N];
    for (int i = 0; i < N; i++) {
        x[i] = randint(0,20);
        y[i] = randint(0,20);
    }

    // minimum of f(a,b) by girll search 
    double a = 0, b = 0, best = 1e300;
    double amin = -10, amax = 10, bmin = -30, bmax = 30;
    for (int pass = 0; pass < 6; pass++) {
        double da = (amax - amin) / 200, db = (bmax - bmin) / 200;
        for (double aa = amin; aa <= amax; aa += da)
            for (double bb = bmin; bb <= bmax; bb += db) {
                double v = f(aa, bb, x, y);
                if (v < best) { best = v; a = aa; b = bb; }
            }
        amin = a - da; amax = a + da;   // we zoom around the best pair
        bmin = b - db; bmax = b + db;
    }
    printf("y = %.3f x + %.3f\n", a, b);

    // gnuplot
    FILE *gp = popen("gnuplot -persist", "w");
    if (!gp) { perror("popen"); return 1; }
    fprintf(gp, "set grid\nset xrange [0:22]\nset yrange [0:26]\n");
    fprintf(gp, "a = %f\nb = %f\n", a, b);
    fprintf(gp, "plot '-' with points pt 7 ps 1.5 lc rgb 'blue' title 'points', "
                "a*x+b with lines lw 2 lc rgb 'red' title 'y = %.3fx + %.3f'\n", a, b);
    for (int i = 0; i < N; i++) {
        fprintf(gp, "%f %f\n", x[i], y[i]);
    }
    fprintf(gp, "e\n");
    pclose(gp);
    return 0;
}