#include "pios.h"

#include <string.h>

#define PIN 21

typedef struct {
    unsigned ncycles;
    unsigned v;
} event_t;

unsigned analyze(event_t* events, size_t N) {
    unsigned n = 0;
    while (n < N) {
        unsigned v1, v0 = gpio_read_fast(PIN);
        while ((v1 = gpio_read_fast(PIN)) == v0) {}
        unsigned time = timer_cycles();
        events[n] = (event_t){
            .ncycles = time,
            .v = v1,
        };
        n++;
        v0 = v1;
    }
    return n;
}

#define N 11

static int abs(int i) {
    if (i < 0) {
        return -i;
    }
    return i;
}

static unsigned ns_to_cyc(unsigned ns) {
    return ns * CPU_FREQ_MHZ / 1000;
}

static unsigned cyc_to_ns(unsigned cyc) {
    return cyc * 1000 / CPU_FREQ_MHZ;
}

int main() {
    gpio_set_input(PIN);
    event_t events[N];

    while (1) {
        unsigned n = analyze(events, N);
        unsigned start = events[0].ncycles;
        for (int i = 0; i < n; i++) {
            unsigned got = cyc_to_ns(events[i].ncycles - start);
            printf("pin-%d: %d %d\n", PIN, got, events[i].v);
        }
        delay_ms(100);
        memset(events, 0, sizeof(event_t) * N);
    }

    return 0;
}

int test() {
    gpio_set_input(PIN);
    event_t events[N];

    printf("Waiting for change on pin %d\n", PIN);

    for (int l = 0; l < 4; l++) {
        unsigned n = analyze(events, N);

        printf("%d results\n", n);

        unsigned start = events[0].ncycles;
        int toterr = 0;
        for (int i = 0; i < n; i++) {
            unsigned got = cyc_to_ns(events[i].ncycles - start);
            unsigned want = i * 6000 * 1000 / 700;
            int err = abs((int) (want - got));
            toterr += err;
            printf("%d: got: %d ns, (err %d ns, toterr %d ns) (%d pi cycles)\n", events[i].v, got, err, toterr, ns_to_cyc(got));
        }

        printf("toterr: %d pi cycles\n", ns_to_cyc(toterr));

        delay_ms(100);

        memset(events, 0, sizeof(event_t) * N);
    }

    return 0;
}
