#include <iostream>
#include "Renderer.h"

int main() {
    Renderer r = Renderer();
    r.Render();
    r.CreateImage();

    std::cout << "Render complete" << std::endl;
    return 0;
}