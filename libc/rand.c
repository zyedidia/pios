// TODO improve rand implementation, this one is from libpi

static unsigned short lfsr = 0xACE1u;
static unsigned bit;

unsigned short rand16() {
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    lfsr =  (lfsr >> 1) | (bit << 15);
    return lfsr;
}

unsigned long rand32() {
    return (rand16() << 16) | rand16();
}

void rand_seed(unsigned seed) {
    lfsr = seed;
}
