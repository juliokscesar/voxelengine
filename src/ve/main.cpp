#include "core/engine.h"
#include "core/logging.h"

int main() {
    grflog::info("Hello World!");

    WindowProps winProps(1280, 720, "Hello World", true, false);
    Engine engine(winProps);
    if (!engine.run()) {
        grflog::fatal("Failed to run engine");
        return 1;
    }
    engine.shutdown();
    return 0;
}
