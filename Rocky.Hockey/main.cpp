#include "RockyHockeyMain.h"
#define _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING

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