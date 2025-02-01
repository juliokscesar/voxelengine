#include <iostream>

#include "core/engine.h"

int main() {
    Engine engine(WindowProps(800, 600, "Hello World", true, false));
    if (!engine.run())
        return 1;
    return 0;
}
