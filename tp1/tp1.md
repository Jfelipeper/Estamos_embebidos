# Ejercicios escritos del tp1

Este README tiene las respuestas escritas o la interpretacion de algunos ejercicios.

## Ejecucion de codigos

Si usa algun `IDE` puede copiar y ejecutar los codigos que aparezcan.
En caso de usar desde la terminal tener `gcc` de modo que ejecute:

```console
gcc <ejercicio>.c -o <nombre ejecutable>
./<nombre ejecutable>
```

## Ejercicios

- [x] Ejercicio 1.
- [x] Ejercicio 2.
- [ ] Ejercicio 3 falta simular.
- Ejercicio 4:
  - [x] Capacitores y resistencias calculadas.
  - [ ] Falta simular.
- Ejercicio 5:
  - [x] Punto 1.
  - [x] Punto 2.
  - [x] Punto 3.
  - [ ] Falta medir.
- [x] Ejercicio 6

### Ej 2

Da un loop que no para porque no incrementa t, fijarse si seria necesario.

### Ej 4

En la salida se esperan ver:

$$\frac{3.3 V}{2}=1.65 V$$

Para los **pasabajos** si se toma como referencia de resistencia $10k\Omega$ entonces se puede tomar a partir de la expresion:

$$C=\frac{1}{2\pi R f_{corte}}$$

De modo que seran:

- (i) $f_{corte}=1kHz$ es con $C \approx 16 nF$.
- (ii) $f_{corte}=16Hz$ es con $C \approx 1 \mu F$

### Ej 5

La configuracion del paso 1 fue hecha en clase siguiendo `New` -> `STM32 project` -> `Nombre del chip` y despues con el GUI para los pines se modifico:

- `SYS` -> `Debug` -> `Serial Wire`
- `RCC` -> `HSE` -> `Crystal/Ceramic Resonator`
- `PC13` -> `GPIO Output`

Para el paso 2 donde el pin 13 se mantiene alto se agrego la funcion:

```c
// Esto fue en la seccion de codigo fuera del loop
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
```

El `GPIO_PIN_RESET` es porque al ser alimentado con 3.3 V del USB y observando el esquematico:

![leds](/home/guido/facu/practicas/embebidos/assets/Foto_leds.png)

Por lo que si esta en `SET` serian 3.3 V y como consecuencia no se prende el led.

En cunato al toggle del tercer punto se puede utilizar parte de lo anterior haciendo lo siguiente:

```c
/* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_Delay(500);
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	  HAL_Delay(2000);
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```

Para medir el consumo del led encendido se puede volver al esquematico suponiendo que el reset es 0 V y la caida por el led de 2 V, entonces cosume aproximadamente:
$$I_{on} = \frac{3,3V-2V}{510 \Omega} \approx 2,55 mA$$
Apagado esta claro que deberia ser nulo el consumo del LED.

### Ej 6

Elijo el `PB6` y `PB7` para los actuadores tal que el 6 tenga el pull-up y el 7 el pull-down.
Cuando probe hacer lo del punto 3 me pasa lo siguiente:

- `PB6` da valor 1.
- `PB7` da valor 0.
  Esto es con las lineas:

```c
  int SW1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
  int SW2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
```

Para la conexion de los pulsadores puse el `PB6` con el boton a tierra pues al pulsarlo pasaria a cero y para el `PB7` hice lo opuesto.

![botones conectados](/home/guido/facu/practicas/embebidos/assets/Conexion_botones.jpg)

En cuanto a las macros, la verdad que no se si esta bien lo que hice pero puse esto que funciona (a mi gusto peor que la otra solucion):

```c
//Macro
#define PRESIONADO 1
#define NO_PRESIONADO 0

int main(){
  //Definiciones de las variables
  uint8_t SW1 = NO_PRESIONADO;
  uint8_t SW2 = NO_PRESIONADO;

  //Adentro del while
  if((SW1 = PRESIONADO) != (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) {
    SW1 = NO_PRESIONADO;
  }
  if((SW2 = PRESIONADO) != (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)){
    SW2 = NO_PRESIONADO;
  }
return 0;
}
```

Finalmente para la maquina de estados pedida hice lo siguiente:

```c
//Declaracion de los estados
enum state maquina;
maquina = inicio;

//Declaracion del while
while (1)
{
switch (maquina) {
  case inicio:
    maquina = led_on;
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    break;

  case led_on:
    if ((uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){
      maquina = led_off;
      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
    }
    break;

  case led_off:
    if ( (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 1){
      maquina = led_on;
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    }
    break;

  default:
    maquina = inicio;
    break;
}
```

Aca ilumine el led que viene con la placa para no tener que hacer la conexion, ademas de usar la config de los dos botones anteriores.
