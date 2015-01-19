#pragma once

#include "mprocess.h"

class MPlayerProcess
{
public:
    MPlayerProcess(const std::string exepath = "")
        : _process(exepath)
        , _sound_level(0)
        , _time(0.0f)
        , _max_time(0.0f)
        , _music_start(false)
    {}

    virtual ~MPlayerProcess() { stop(); }

    void play(const std::string& path = "");
    void pause();
    void stop();

protected:
    MProcess        _process;
    std::string     _music_path;
    int             _sound_level;
    float           _time;
    float           _max_time;

    bool            _music_start;
};
