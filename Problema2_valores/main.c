#include <stdio.h>
#include <math.h>

#define N 100 //gridpoints
#define L 1.0
#define h (L/N) //paso h=dx

double potencial(double x) {
    return 50.0*cos(-M_PI*x/L);
    //return 0;
}

double resolver(double psi[N+1], double E) {
    double k[N+1], numerador;

   //definir y rellenar k
    for (int i = 0; i < N+1; i++) {
        double x = i * h;
        k[i] = (E - potencial(x));
            //printf("%f\n", k[i]);
    }

    //valores de frontera funcion de onda estatica
    psi[0] = 0.0;
    psi[1] = 2.0/(double)N;
    double factor = 6.0*(double)N*(double)N;

    for (int i = 2; i < N+1; i++) {
        numerador = 2.0*(1.0 - 5.0 * k[i - 1]/factor)*psi[i - 1] - (1.0 + k[i - 2]/factor)*psi[i - 2];
        psi[i] = numerador / (1.0 + k[i]/factor);
    }
   return psi[N];
}    


int main() {
    double psi[N+1];
    double E1 = 17.0, E2 = 21.0; //valores entre los cuales se estima valor propio 1
    double E3; 
    double psi_1, psi_2, psi_3; 
    double etha = 1e-10; 
    int contador = 0;

    while (contador < 5) {
        psi_1 = resolver(psi, E1);
        psi_2 = resolver(psi, E2);

        if (psi_1 * psi_2 > 0) {
            E1 = E2;
            E2 += 5.0; //se va buscando si existen ceros de la funcion en intervalos de 5 en 5 
        } else if (psi_1 * psi_2 < 0) {
            E3 = (E1 + E2) / 2;
            psi_3 = resolver(psi, E3);

            if (psi_1 * psi_3 < 0) {
                E2 = E3;
            } else if (psi_3 * psi_2 < 0) {
                E1 = E3;
            }

            if (fabs(E1 - E2) < etha) {
                printf("valor propio %d: %lf\n", contador+1, (E1 + E2) / 2);

                contador++;
                
                E1 = E2;
                E2 += 5.0;
            }
        }
    }

    return 0;
}

