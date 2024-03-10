#include "stdio.h"
#include <stdint.h>

// Prototipos
int change_bit(int32_t posicion, int32_t valor, uint32_t *variable);

int main() {
  uint32_t variable = 0;

  for (int i = 0; i < 32; i++) {
    variable = 0;
    change_bit(i, 1, &variable);
    printf("0x%X\n", variable);
  }
  return 0;
}

// Funciones

/* Funcion que cambia un bit al valor indicado
 * @param posicion: posicion del bit a cambiar
 * @param valor: valor del bit
 * @param variable: variable a modificar
 */
int change_bit(int32_t posicion, int32_t valor, uint32_t *variable) {
  if (posicion >= 32 || posicion < 0 || (valor != 0 && valor != 1))
    return -1;
  *variable = (*variable && ~(1 << posicion)) | (valor << posicion);
  return 0;
}
