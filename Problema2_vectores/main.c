#include <stdio.h>
#include <math.h>

#define N 100 //gridpoints
#define L 1.0
#define h (L/N) //paso h=dx

int main() {
    double psi[N+1];
    double k[N+1];
    double E[5] = {18.978316, 50.170586, 83.076227, 125.964973, 179.434990};
    
   for (int l = 0; l < 5; l++){
    for (int i = 0; i < N+1; i++) {
        double x = i * h;
        k[i] = (E[l] - 50.0*cos(-M_PI*x/L));
    }
    
    psi[0] = 0.0;
    psi[1] = 2.0/(double)N;
    printf("%f %f\n",0.0, psi[0]);
    printf("%f %f\n",1/(double)N, psi[1]);
    double factor = 6.0*(double)N*(double)N;

    for (int i = 2; i < N+1; i++) {
        double numerador = 2.0*(1.0 - 5.0 * k[i - 1]/factor)*psi[i - 1] - (1.0 + k[i - 2]/factor)*psi[i - 2];
        psi[i] = numerador / (1.0 + k[i]/factor);
        printf("%f %f\n",(double)i/(double)N, psi[i]);
    }
  }
 }
