#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


double getThermalVoltageConst(double vd, double id, double t, int n, int print) {
    double Is = 0.00000000000001;
    double fi = vd / (log((id / Is) + 1) * n);
    double c = fi / t;
    if (print) {
        printf("\tCalculating thermal voltage from mesuared values...\n");
        printf("\t   Calculation <Shockley Eqn>: fi = vd / ((log((id / Is) + 1) * n) = %lf / (log((%lf / %lf) + 1) * %d = %f\n\n", vd, id, Is, n, fi);

        printf("Calculated constant: c = fi / temp = %lf / %lf = %lf\n", fi, t, c);
    }
    return c;
}

 

int main() {

    double measuredVd; //mV
    double  measuredId; //A
    double measuredTemp; //kelvin
    int n;

    double estAtId; //A
    double estAtTemp; //kelvin

    printf("Measured Vd (V, Double): ");
    scanf("%lf", &measuredVd);

    printf("Measured Id (A, Double): ");
    scanf("%lf", &measuredId);

    printf("Measured Temp. (kelvin, double) ");
    scanf("%lf", &measuredTemp);

    printf("n (Integer, if unsure, type 1): ");
    scanf("%d", &n);

    printf("Estimate at Id: ");
    scanf("%lf", &estAtId);

    printf("Estimate at Temp.: ");
    scanf("%lf", &estAtTemp);

    int print = 1;

    double f = getThermalVoltageConst(measuredVd, measuredId, measuredTemp, n, print);
    double realFi = estAtTemp * f;

    if (print) {
        printf("Calculated realFi: %f\n", realFi);
    }

    double Is = 0.00000000000001;
    double estVd = log ((estAtId / Is) + 1) * n * realFi;
    printf("Calculated estimate value from shockley eqn: %f\n", estVd);




    return 0;

}

