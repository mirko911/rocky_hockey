#include "RockyHockeyMain.h"

int main() {
    RockyHockeyMain rockyhockey(0);

    rockyhockey.Init();
    rockyhockey.Run();
    rockyhockey.Fini();

#ifdef _WIN32
    getchar();
#endif
    return 0;
}