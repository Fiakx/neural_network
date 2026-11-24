#include <stdio.h>

double f(double x, double y) {
    return y*y*y + 2*y*y - x*x;
}

// send the grid of points (x, y, f(x,y)) to gnuplot
void envoyer_donnees(FILE *gp) {
    int n = 40;
    double xmin = -3, xmax = 3, ymin = -3, ymax = 2;
    for (int i = 0; i <= n; i++) {
        double x = xmin + (xmax - xmin) * i / n;
        for (int j = 0; j <= n; j++) {
            double y = ymin + (ymax - ymin) * j / n;
            fprintf(gp, "%f %f %f\n", x, y, f(x, y));
        }
        fprintf(gp, "\n");   // gap
    }
    fprintf(gp, "e\n");
}

// 3d view 
int view_3d(void) {
    FILE *gp = popen("gnuplot -persist", "w");
    if (!gp) { perror("popen"); return 1; }

    fprintf(gp, "set xlabel 'x'\nset ylabel 'y'\nset zlabel 'f(x,y)'\n");
    fprintf(gp, "set hidden3d\nset pm3d\n");
    fprintf(gp, "splot '-' with lines title 'f(x,y) = y^3 + 2y^2 - x^2'\n");
    envoyer_donnees(gp);

    pclose(gp);
    return 0;
}

// top view (view level like in a map)
int view_top(void) {
    FILE *gp = popen("gnuplot -persist", "w");
    if (!gp) { perror("popen"); return 1; }

    fprintf(gp, "set xlabel 'x'\nset ylabel 'y'\n");
    fprintf(gp, "set view map\nunset surface\n");
    fprintf(gp, "set contour base\nset cntrparam levels 25\n");
    fprintf(gp, "set title 'Lignes de niveau de f(x,y) = y^3 + 2y^2 - x^2'\n");
    fprintf(gp, "unset key\n");
    fprintf(gp, "splot '-' with lines\n");
    envoyer_donnees(gp);

    pclose(gp);
    return 0;
}

int main(void) {
    view_3d();
    view_top();
    return 0;
}