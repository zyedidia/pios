#include "pios.h"
#include "dev.h"

extern void irq_vec_asm();

struct thread {
    volatile uint32_t registers[16];
};

volatile struct thread threads[100];
volatile unsigned n_threads = 1;
volatile unsigned curr_thread_id = 0;

void swippityswap(volatile uint32_t *live_state, volatile struct thread *new_thread, volatile struct thread *old_thread) {
    for (size_t i = 0; i < 16; i++) {
        old_thread->registers[i] = live_state[i];
        live_state[i] = new_thread->registers[i];
    }
}

void irq(uint32_t *state_ptr) {
    dev_barrier();
    if (!timer_has_irq()) return;

    unsigned next_thread_id = (curr_thread_id + 1) % n_threads;
    if (next_thread_id == curr_thread_id) goto end;

    swippityswap(state_ptr, &(threads[next_thread_id]), &(threads[curr_thread_id]));
    curr_thread_id = next_thread_id;

end:
    dev_barrier();
    timer_clear_irq();
    dev_barrier();
}

void spawn(void (*foo)(unsigned), unsigned arg) {
    uint32_t *stack = kmalloc(1024 * 1024 * 4);
    unsigned thread_id = n_threads++;

    for (size_t i = 0; i < 16; i++)
        threads[thread_id].registers[i] = 0;

    threads[thread_id].registers[0] = arg;
    threads[thread_id].registers[13] = (uint32_t)stack;
    threads[thread_id].registers[15] = (uint32_t)foo;
}

void worker(unsigned which) {
    bool lit = false;
    while (true) {
        gpio_write(which, lit);
        lit = !lit;
        printf("Hello from worker on gpio %u\n", which);
        delay_ms(1000);
    }
}

int main() {
    gpio_set_output(21);
    gpio_set_output(20);

    irq_init_table();
    irq_enable(BASIC_TIMER_IRQ);
    timer_irq_load(0x1000);
    register_irq_vec(IRQ_VEC_IRQ, (uintptr_t) irq_vec_asm);

    enable_interrupts();

    spawn(worker, 21);
    spawn(worker, 20);

    while (true);

    return 0;
}
