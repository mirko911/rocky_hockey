#include "RockyHockeyMain.h"

int main() {
	//Make sure we got a config object before we start with rockyHockey
	Config::get();

    RockyHockeyMain rockyhockey("../demo_4.mkv");

    rockyhockey.Init();
    rockyhockey.Run();
    rockyhockey.Fini();

#ifdef _WIN32
    getchar();
#endif
    return 0;
}