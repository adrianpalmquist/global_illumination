#include <iostream>
#include <time.h>
#include "Renderer.h"

int main() {
    srand (time(NULL));

//    std::vector<int> vec1;
//    vec1.push_back(1);
//    vec1.push_back(2);
//    vec1.push_back(3);
//
//    std::vector<int> vec2;
//    vec2.push_back(4);
//    vec2.push_back(5);
//    vec2.push_back(6);
//
//    vec1.insert(vec1.end(), vec2.begin(), vec2.end());

//    for (int i = 0; i < vec1.size(); i++) {
//        std::cout << vec1.at(i) << std::endl;
//    }

    const clock_t begin_time = clock();

    Renderer r = Renderer();
    r.Render();

    std::cout << "Render complete, time elapsed: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << std::endl;
    return 0;
}