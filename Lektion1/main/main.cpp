#include <stdio.h>
#include <iostream>
#include "lektion1.h"

/* Allow resolution of undecorated (a.k.a. not mangled), C-style references.  */
extern "C" {
    void app_main(void);
}

void app_main(void) {
    Lektion1 my_L1;

    my_L1.Speak();
}