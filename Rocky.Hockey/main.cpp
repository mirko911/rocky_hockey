#include "RockyHockeyMain.h"

int main() {
    RockyHockeyMain rockyhockey("./../../demos/demo_4.mkv");

    rockyhockey.Init();
    rockyhockey.Run();
    rockyhockey.Fini();

#ifdef _WIN32
    getchar();
#endif
    return 0;
}