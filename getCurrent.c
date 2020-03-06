#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getCurrent.h"
#include <math.h>

int main(int argc, char ** argv) {

    if (argc == 2) {
        if (strcmp(*(argv + 1), "Is") == 0) {
            fprintf(stdout, "Is is the saturation current. It is a constant. For a common diode it is about 10^(-14) A.\n");
            exit(0);
        } else if (strstr(*(argv + 1), "err")) {
            fprintf(stdout, "errThreshold is the value at which our program will quit processing.\n");
            printf("When the change in current from one iteration to the next is less than this value");
            printf(" the program will quit.\n");
            exit(0);
        } else if (strcmp(*(argv + 1), "R") == 0) {
            printf("R is the resistance in series with the diode.");
            exit(0);
        }
    }
    if (argc < 4 || argc > 10) {
        fprintf(stderr, "Correct usage: <%s Vs n R guessVd(optional) Is(optional) Vd(optional) Id(optional) errThreshold(optional) printCalcsOff(optional)>\n", *argv);
        fprintf(stderr, "Vs - (V) supplied voltage\n");
        fprintf(stderr, "n - experimental emission coefficient\n");
        fprintf(stderr, "R - (Ohms) resistance value in series with one diode\n");
        fprintf(stderr, "guessVd - (mv) different than Vd. Vd is the Vd at a certain Id. guessVd is our initial guess of what Vd is. If guessVd = Vs, we will change guessVd\n");
        fprintf(stderr, "Is - (A * 10^(-14)) saturation current or scale current");
        fprintf(stderr, ". default is 10e(-14)\n");
        fprintf(stderr, "Vd (mV) and Id (mA) - Diode has Vd at Id. Defualt is 70 mV and 1 mAi USED TO CALCULATE COEFF!\n");
        fprintf(stderr, "errThreshold - the precision of calculation must be within this value. There is a limit to precision.\n");
        fprintf(stderr, "printCalcsOff - If any value is inserted here, calcualtions will not be printed\n");

        exit(1);
    }

    const double Vs = atof(*(argv + 1));
    const int n = strtol(*(argv + 2), 0, 10);
    const double R = atof(*(argv + 3));
    double guessVd;
    double Is;
    double VdAtId;
    double Id;
    double thres;
    int print;

    if (argc > 4) {
        guessVd = atof(*(argv + 4));
    } else {
        guessVd = .7;
    }
    if (fabs(guessVd - Vs) < 0.01) {
        guessVd -= .06;
        guessVd = fabs(guessVd);
    }

    if (argc > 5) {
        Is = atof(*(argv + 5))/pow(10, 14);
    } else {
        Is = pow(10, -14);
    }

    if (argc > 6) {
        VdAtId = atof(*(argv + 6));
    } else {
        VdAtId = 700; //Diode's initial characteristic(mV) used to calc. coeff
    }

    if (argc > 7) {
        Id = atof(*(argv + 7));
    } else {
        Id = 1; //Diode's initial characteristic (mA) used to calc. coeff
    }
    if (argc > 8) {
        thres = atof(*(argv + 8));
        if (thres == 0)
            thres = 0.0001;
    } else {
        thres = 0.0001;
    }
    if (argc > 9) {
        print = 0;
    } else {
        print = 1;
    }

    //first calculate emission coeff
    if (print)
        printf("----------- Iteration Setup -----------\n");
    const double c = getCoeff(VdAtId, Id, Is, n, print); //based on the diode.
    float Vd = 0;
    float oldVd = -1000;
    double i = getCurrent(Vs, guessVd, R, print);
    printf("%s", print ? "\n" : "");
 

    int on = 1;
    while (fabs(Vd - oldVd) > thres) {
        if (print)
                printf("----------- Iteration %5d -----------\n", on++);
        oldVd = Vd;
        Vd = getVoltage(i, Is, n, c, print);
        i = getCurrent(Vs, Vd, R, print);
        printf("%s", print ? "\n" : "");
    }
    printf("Calculated current in circuit: %fA\n", i);
    return 0;
}

double getVoltage(double i, double Is, int n, double coeff, int print) {
    double Vd = log((i / Is) + 1) * n * coeff;
    if (print) {
        printf("\tCalculating the voltage drop on diode...\n");
        printf("\t   Calculation <Shockley Eqn>: New Vd = log((i/Is) + 1) * n * coeff = log((%0.4f/%0.15f) + 1) * %d %0.4f = %f\n\n", i, Is, n, coeff, Vd);
    }
    return Vd;
}

double getCurrent(double Vs, double Vd, double R, int print) {
    double i = (Vs - Vd) / R;
    if (print) {
        printf("\tCalculating the current...\n");
        printf("\t   Calculation <Ohm's Law> Current: (Vs - Vd) / R = (%0.4f - %0.4f) / %0.2f = %0.5f\n\n", Vs, Vd, R, i);
    }
   return i;
} 


double getCoeff(double Vd, double Id, double Is, int n,  int print) {
    double IdInmA = Id/pow(10, 3);
    double VdInmV = Vd/1000;
    double coeff = (VdInmV) / (log((IdInmA/Is) + 1) * n);
    if (print) {
        printf("\tCalculating the emission coefficient... Based on the diode's parameters...\n");
        printf("\t   Calculation <Shockley Eqn> Emission Coeff: (Vd) / (log((Id/Is) + 1) * n) = (%0.5f) / (log((%0.4f/%0.15f) + 1) * %d) = %0.4fmV (normal: 26 mV)\n\n", Vd, IdInmA, Is, n, (coeff * 1000));
    }
    return coeff;
}
 
