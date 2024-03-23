uint32_t set_on_off(uint32_t variable32bit, int nbit, int set);

int main() {
    uint32_t newvar = 0;
    uint32_t var = 0b00000000000000000000000000000101;
    newvar = set_on_off(var, 0 , 0);
    printf("%u", newvar);
    return 0;
}


uint32_t set_on_off(uint32_t variable32bit, int nbit, int set) {
    if (set == 1) {
        variable32bit |= (1 << nbit);                   // |= me hace la operación or con 1 en el bit que le pedi, por defecto me da 1. 
        return variable32bit;
    } else if (set == 0) {
        variable32bit &= ~(1 << nbit);                  // &= ~ me hace la negacion del and con un 1, por defecto me da un 0. 
        return variable32bit;
    } else {
        printf("mal seteado");
        return variable32bit;
    }
}


