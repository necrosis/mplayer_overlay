#include "mplayerprocess.h"
#include <list>

using namespace std;

void MPlayerProcess::play(const std::string& path)
{
    if (path == "" && _music_start)
        _process << " ";

    _music_path = path;

    list<string> args = {"mplayer", string(path)};
    _process.exec(args);

    _music_start = true;
}

void MPlayerProcess::pause()
{
    if (_music_start)
        _process << " ";
}

void MPlayerProcess::stop()
{
    if (_music_start)
    {
        _process << "q";
        _music_start = false;
        _process.stop();
    }
}
