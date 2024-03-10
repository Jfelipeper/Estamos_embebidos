#include <stdio.h>

enum estado_def { inicio, reposo, disparo, pre_disparo, fin };
const int umbral1 = 30;
const int umbral2 = 25;
const float sig[] = {-0.1969, 0.7252, 1.3017, 0.6221, 1.5131,  0.6530,  2.2430,
                     0.1166,  6.9498, 2.3170, 3.1620, -7.9198, 18.5878, 64.9743,
                     0.7815,  7.3498, 2.5495, 9.0523, 13.3169, 26.7396, 19.9011,
                     6.1930,  3.6618, 2.1840, 2.7444, 1.6324,  0.4408,  -0.0509,
                     0.5014,  0.6186, 0.1427};
const int t_max = sizeof(sig) / sizeof(float);

int main(void) {
  enum estado_def maquina;
  maquina = inicio;
  int t = 0;

  while (maquina != fin) {
    switch (maquina) {
    case inicio:
      maquina = reposo;
      break;
    case reposo:
      if (t > t_max) {
        maquina = fin;
        break;
      }
      if (sig[t] > umbral1) {
        maquina = pre_disparo;
        break;
      }
      t++;
      break;
    case pre_disparo:
      if (t > t_max) {
        maquina = fin;
        break;
      }
      if (sig[t] > umbral2) {
        maquina = disparo;
        printf("Se produjo un disparo en tiempo t = %d con valor sig = %.2f\r\n", t, sig[t]);
        break;
      }
      t++;
      break;
      case disparo:
        maquina = reposo;
      break;
    case fin:
      puts("Fin de proceso");
    }
  }
  return 0;
}
