#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>
#include <math.h>

//sistema de 4 EDOs de primer orden
int func(double t, const double y[], double f[], void *params){
  (void)(t); /*avoid unused paramater warning */
  double *p = (double*)params; // cambio de puntero void a puntero double
  double G = p[0];
  double MT = p[1];

    f[0] = y[1]; // y[0]=r  y[1]=r'
    f[1] = y[0]*pow(y[3],2) - (G*MT)/pow(y[0],2); // f[1] = r''
    f[2] = y[3]; // y[2]=theta  y[3]=theta'
    f[3] = -2*y[1]*y[3]/y[0]; // f[3] = theta''

    return GSL_SUCCESS;
}

int solucion_pvi(int nf, double *x, double h,
                 gsl_odeiv2_driver *d, double *Y){
  double t_i = 0.0;
  int status = 0;
  for (int n=1; n<=nf ; ++n) {
      t_i = *x + h;
      status = gsl_odeiv2_driver_apply (d, x, t_i, Y);
      //printf("%f %f %f %f\n",Y[0], Y[1], Y[2], Y[3]);
      if (status != GSL_SUCCESS){
        printf ("error, return value=%d\n", status);
        break;
      }
   if (Y[0] >= 8371000.0){ 
            break;
        }

    }
  return 0;
}

int main() {
    // parametros dados, en SI
    double H = 1072000.0; // r(0) misil
    double G = 6.674e-11; 
    double MT = 5.972e24; 
    double RT = 6371000.0; 
    double v0 = 7300.0; // velocidad tangencial inicial misil
    double phi = 15.0*M_PI/180.0; //angulo de lazamiento misil

    double params[2] = {G, MT};


    // Trayectoria Misil:
    // en primer lugar se busca conocer el tiempo t en que se encontrará a 2000 km (en descenso) sobre la tierra
    // PVI. Estimación numérica usando RK45:
    FILE *archivo = fopen("misil.dat", "w");
    gsl_odeiv2_system sys = {func, NULL, 4, params}; //sist 4 ecuaciones

    double h = 1.0;
    int i;
    gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rkf45,
                                  h, 1e-6, 0.0);
                                   //paso h //error abs e-6 // error relativo 0

    double t = 0.0; // t inicial
    double t1 = 999999.9; // max tiempo a considerar. valor alto arbitrario.
    double nn = (t1-t)/h;
    int N = (int)nn;
    double ti = 0.0;
    
    double tol=1000.0; //se define tolerancia en error de estimacion de posicion
    
    double y[4] = {H+RT, v0*sin(phi), 0.0, v0*cos(phi)/(H+RT)}; //condiciones iniciales misil
                                            //v=r*theta'

    double tf = 0.0; //variables para guardar tiempo y posicion en el cual proyectil debe chocarlo
    double rf = 0.0;
    double thetaf = 0.0;
    
    for (i = 1; i <= N; i++)
     {
      fprintf(archivo, "%.6f %.6f %.6f %.6f %.6f\n", ti, y[0], y[1], y[2], y[3]); //guardar datos para graficar trayectoria
      ti = i * h;
      
      double rfinal = RT + 2000000.0;
      double error = fabs(rfinal - y[0]);

      int status = gsl_odeiv2_driver_apply (d, &t, ti, y);
      if (status != GSL_SUCCESS)
        {
          printf ("error, return value=%d\n", status);
          break;
        }
      // terminar de guardar datos cuando se llege a 2000 km sobre la tierra, en descenso
      if (error <= tol && y[1] < 0.0){ 
            tf = ti;
            rf = y[0];
            thetaf = y[2];
            break;
        }
     }
    fclose(archivo);
    gsl_odeiv2_driver_free (d);
    
    

    //Trayectoria Proyectil:
    //PVF. estimación usando shooting no lineal.
    double h2 = 1.0;
    double t_0 = 0.0;
    double t_f = tf;
    double nnf = (tf - t_0)/h2;
    int nf = (int)nnf;

    gsl_odeiv2_system sys2 = {func, NULL, 4, params};

    gsl_odeiv2_driver * d2 = gsl_odeiv2_driver_alloc_y_new (&sys2, gsl_odeiv2_step_rk4,
                                  h2, 1e-6, 0.0);
  
    double cond_final = rf;
    double v_i = 2000.0;

    double tol2 = 1e-9;
    double error2 = tol2 + 1;
    double  y0_0 = -0.1; 
    double  y0_1 = -v_i*cos(0)/RT; 
    double  y0_2 = -0.1;
    
    
    // Primera ejecucion
    double x = 0.0; //reiniciar t
    double Y[4] = {RT, 0.0, M_PI, -v_i*cos(0)/RT}; //condiciones iniciales proyectil
    Y[3]=y0_0;
    solucion_pvi(nf, &x, h2, d2, Y);
    double error_0 = fabs(Y[0] - cond_final);
    
    // Segunda ejecucion
    FILE *archivo2 = fopen("proyectil.dat", "w");
    int nmax = 100;
    int n = 0;
   while (error2 > tol2 && n<nmax){
       t = 0.0;
       Y[0] = RT;
       Y[1] = 0.0;
       Y[2] = M_PI;
       Y[3] = y0_1;
            
       solucion_pvi(nf, &t, h2, d2, Y);
       error2 = fabs(Y[0] - cond_final);

       y0_2 = y0_1 - (y0_1 - y0_0) / (error2 - error_0)*error2; //metodo secante
       y0_0 = y0_1;
       y0_1 = y0_2;
       error_0 = error2;
        
        ++n;
    }

    t= 0.0;
    Y[0] = RT;
    Y[1] = 0.0;
    Y[2] = M_PI;
    Y[3] = y0_1;


   printf("Velocidad en [m/s²] %f/n", Y[3]*RT);
   printf("/n");
    for(int i = 1; i <= nf; i++){
        double ti = t + h2;
        int status = gsl_odeiv2_driver_apply(d2, &t, ti, Y);
        fprintf(archivo2, "%.6f %.6f %.6f %.6f %.6f\n", ti, Y[0], Y[1], Y[2], Y[3]);
        t = ti;
        if (Y[0] >= 10000000.0){ 
            break;
        }
    }
    fclose(archivo2);
    gsl_odeiv2_driver_free(d2);
    
    return 0;
}



