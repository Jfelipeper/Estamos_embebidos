#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <stdio.h>

int main() {
    enum state{
        rest,
        Pre_trigger,
        trigger,
        finalprocess
    };
    enum state current_state = rest;
    int t = 0, flag = 0;
    int umbral1 = 30;
    int umbral2 = 25;
    float tmax = 0;
    float sig[] = {-0.1969, 0.7252, 1.3017, 0.6221, 1.5131, 0.6530, 2.2430,
                    0.1166, 6.9498, 2.3170, 3.1620, -7.9198, 18.5878,64.9743,
                    0.7815, 7.3498, 2.5495, 9.0523, 13.3169, 26.7396,19.9011,
                    6.1930, 3.6618, 2.1840, 2.7444, 1.6324, 0.4408,-0.0509,
                    0.5014, 0.6186, 0.1427};

   while(flag == 0){
    tmax = sig[t];
    printf("%d y %f\n", current_state, tmax);
    switch(current_state){
        case rest:
            if(tmax > umbral1){
                current_state = Pre_trigger;
                break;
            }
            t++;
            break;
        case Pre_trigger:
            printf("etapa de pre-disparo.. %f\n", tmax);
            if(tmax<umbral2){
                current_state = trigger;
                break;
            }
            t++;
            break;
        case trigger:
            printf("etapa de disparo..%f\n", tmax);
            t = 0;
            current_state = finalprocess;
            break;
        case finalprocess:
            flag = 1;
            break;
    }
}

    return 0;
}

