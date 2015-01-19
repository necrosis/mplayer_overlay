#pragma once

#include <sys/types.h>
#include <string>
#include <list>

#define PIPE_READ 0
#define PIPE_WRITE 1

#define BLOCK_SIZE 512

class MProcess
{
public:
    MProcess(const std::string exepath = "")
        : _process_pid(0)
        , _exe_path(exepath)
        , _init_compite(false)
    { }

    virtual ~MProcess() { stop(); }

    virtual bool exec(const std::string& exe, const std::list<std::string>& arguments);
    virtual bool exec(const std::list<std::string>& arguments);
    virtual void stop();
    virtual MProcess& operator << (const std::string& in);
    virtual MProcess& operator >> (std::string& out);
    virtual bool active() const;

protected:
    bool create_process();
    void stop_process();

    void run();

    pid_t                   _process_pid;
    std::string             _exe_path;
    std::list<std::string>  _arguments;

    int                     _in_pipe[2];
	int                     _out_pipe[2];

	bool                    _init_compite;
	char                    _block[BLOCK_SIZE];
};
