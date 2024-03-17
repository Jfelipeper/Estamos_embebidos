# TP2

## Ejercicio 1

Yendo a la seccion `9.2.2` donde se ve como funciona el registro `GPIOx_CRH` para configurarlo. En este hay dos variables que controlan los pines:

- `CNF13` : Controla el modo en que funcion el pin.
- `MODE13` : Controla la frecuencia de swichting del pin.

Podemos poner `CNF13` en 01 para que este en open drain y el `MODE13` en 11 para que funcione en la maxima frecuencia.

El b era medio complicado porque no entendi muy bien como hacerlo.

## Ejercicio 2

El HSI tiene un 1% de error de fabrica a 25°C por lo que en 300 segundos puede tener hasta 3 segundos de error, en cambio con el oscilaor a cristal tenes un error de 0,001% que equivale a 3 ms de error en el plazo a medir de tiempo.

## Ejercicio 3

Aca usamos un par de datasheets distintos para ver las cosas, de modo que:

- `BASEPRI`: Controla que numero de prioridad + subprioridad modifica a las interrupciones.
- `AIRCR`: Controla el `PRIGROUP` que es de 3 bits donde se determina cuantos bits se toman como prioridad y cuantos como subprioridad.

Como cuanto menor sea el `PRI_n` mayor la prioridad, desde el grafico se observa que el 0101 que comienza deberia tener menor prioridad que el 0100, pero como espera a que termine entonces ambos deben pertenecer a la misma prioridad. Esto significa que como mucho los primeros 3 bits son de prioridad.

Sin embargo el 0110 deberia tener menor prioridad pero se ejecuta cortando a 0100 lo cual indica mayor prioridad lo que no tiene sentido.

## Ejercicio 4

El codigo que aparece al hacer la funcion es:

```arm
...

El llamado a la funcion es:

  variable = fun1(1,1);
 800015c:	2101      	movs	r1, #1
 800015e:	2001      	movs	r0, #1
 8000160:	f000 f864 	bl	800022c <fun1>
 8000164:	4603      	mov	r3, r0
 8000166:	4a01      	ldr	r2, [pc, #4]	; (800016c <main+0x20>)
 8000168:	6013      	str	r3, [r2, #0]

 ...

int fun1(int a, int b){
 800022c:	b480      	push	{r7}
 800022e:	b087      	sub	sp, #28
 8000230:	af00      	add	r7, sp, #0
 8000232:	6078      	str	r0, [r7, #4]
 8000234:	6039      	str	r1, [r7, #0]
int x = 13;
 8000236:	230d      	movs	r3, #13
 8000238:	617b      	str	r3, [r7, #20]
int c = 1;
 800023a:	2301      	movs	r3, #1
 800023c:	613b      	str	r3, [r7, #16]
int y = 8;
 800023e:	2308      	movs	r3, #8
 8000240:	60fb      	str	r3, [r7, #12]
int z = x+c+y+a+b;
 8000242:	697a      	ldr	r2, [r7, #20]
 8000244:	693b      	ldr	r3, [r7, #16]
 8000246:	441a      	add	r2, r3
 8000248:	68fb      	ldr	r3, [r7, #12]
 800024a:	441a      	add	r2, r3
 800024c:	687b      	ldr	r3, [r7, #4]
 800024e:	4413      	add	r3, r2
 8000250:	683a      	ldr	r2, [r7, #0]
 8000252:	4413      	add	r3, r2
 8000254:	60bb      	str	r3, [r7, #8]
return z;
 8000256:	68bb      	ldr	r3, [r7, #8]
}
 8000258:	4618      	mov	r0, r3
 800025a:	371c      	adds	r7, #28
 800025c:	46bd      	mov	sp, r7
 800025e:	bc80      	pop	{r7}
 8000260:	4770      	bx	lr
```

La declaracion de la funcion comenzo en la direccion 800022c, que llama desde el main con la linea 8000160 donde va a ese lugar de memoria y hace las operaciones.

Por otro lado los valores de los elementos los pasa con la funcion `movs` que pasa los valores a los registros r1 y r0 para poder tener guardados los valores en la funcion.

La suma se lleva a cabo por esta secuencia:

```arm
 8000242:	697a      	ldr	r2, [r7, #20]
 8000244:	693b      	ldr	r3, [r7, #16]
 8000246:	441a      	add	r2, r3
```

donde `ldr` carga en los registros lo que estaba en `[r7, #20]` y `[r7, #16]`, que antes habian guardado las variables c y x, para despues con `add` sumarlos.

El `pop` o `push` lo usa para guardar (push) o sacar (pop) lo que esta en `r7` que despues saca al terminar la funcion. No entendimos muy bien que hace con todas las funciones en particular pero bueno.

En cuanto a lo que implementa el `while(1)`, ocurre lo siguiente:

```arm
800016a:	e7fe      	b.n	800016a <main+0x1e>
 800016c:	20000028 	.word	0x20000028
```
