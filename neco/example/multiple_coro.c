#include <stdio.h>
#include "../neco.h"

void ticker(int argc, void *argv[]) {
    while (1) {
        neco_sleep(NECO_SECOND);
        printf("tick\n");
    }
}

void tocker(int argc, void *argv[]) {
    while (1) {
        neco_sleep(NECO_SECOND*2);
        printf("tock\n");
    }
}

int neco_main(int argc, char *argv[]) {
    neco_start(ticker, 0);
    neco_start(tocker, 0);
    
    // Keep the program alive for an hour.
    neco_sleep(NECO_HOUR);
    return 0;
}
