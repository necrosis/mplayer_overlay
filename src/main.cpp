#include <iostream>
#include "mplayerprocess.h"
#include <unistd.h>

using namespace std;

int main()
{
    /*
    MProcess proc;

    list<string> args = {"mplayer", "/home/necrosis/Music/From Games/Ayumi Hamasaki - Startin _pleer.com_.mp3"};

    proc.exec("/usr/bin/mplayer", args);

    proc << "]" << "]";

    string out;
    proc >> out;

    for (int i = 0; i < 5; i++)
    {
        do
        {
            proc >> out;
            cout << out;
        } while (out.size() == BLOCK_SIZE);

        sleep(3);
    }
    cout << endl;

    for (int i = 0; i < 10; i++)
    {
        proc << "/";
        usleep(100000);
    }
    */

    MPlayerProcess music("/usr/bin/mplayer");

    music.play("/home/necrosis/Music/From Games/Ayumi Hamasaki - Startin _pleer.com_.mp3");

    sleep(5);
    music.pause();
    sleep(2);
    music.play();

    sleep(5);

    return 0;
}
