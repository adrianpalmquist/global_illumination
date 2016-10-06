#include <iostream>
#include <time.h>
#include "Renderer.h"

int main() {
    srand (time(NULL));

    Renderer r = Renderer();
    r.Render();
    //r.CreateImage();

    std::cout << "Render complete" << std::endl;
    return 0;
}