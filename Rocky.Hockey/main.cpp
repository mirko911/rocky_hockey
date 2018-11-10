#include "RockyHockeyMain.h"

int main() {
    RockyHockeyMain rockyhockey("demo_low.mp4");

    rockyhockey.Init();
    rockyhockey.Run();
    rockyhockey.Fini();

#ifdef _WIN32
    getchar();
#endif
    return 0;
}