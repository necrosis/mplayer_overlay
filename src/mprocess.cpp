#include "mprocess.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

using namespace std;

bool MProcess::exec(const std::string& exe, const std::list<std::string>& arguments)
{
    stop();

    _exe_path = exe;

    _arguments.clear();
    _arguments.insert(_arguments.begin(), arguments.begin(), arguments.end());

    return create_process();
}

bool MProcess::exec(const std::list<std::string>& arguments)
{
    return exec(_exe_path, arguments);
}

void MProcess::stop()
{
    if (!_process_pid)
        return;

    stop_process();
}

void MProcess::stop_process()
{
    if (!_init_compite)
        return;

    kill(_process_pid, SIGKILL);

    _process_pid = 0;

    close(_in_pipe[PIPE_WRITE]);
    close(_out_pipe[PIPE_READ]);
}

bool MProcess::create_process()
{
    if (pipe(_in_pipe) < 0)
	{
		return false;
	}

	if (pipe(_out_pipe) < 0)
	{
		close(_in_pipe[PIPE_READ]);
		close(_in_pipe[PIPE_WRITE]);
		return false;
	}

    _process_pid = fork();

    if (!_process_pid)
	{
	    /// child
	    run();
	}
	else
        if (_process_pid > 0)
        {
            /// parent

            printf("PID: %d\n", _process_pid);

            // close unused file descriptors, these are for child only
			close(_in_pipe[PIPE_READ]);
			close(_out_pipe[PIPE_WRITE]);

			_init_compite = true;
            return true;
        }

    close(_in_pipe[PIPE_READ]);
    close(_in_pipe[PIPE_WRITE]);
    close(_out_pipe[PIPE_READ]);
    close(_out_pipe[PIPE_WRITE]);

    _process_pid = 0;
    return false;
}

void MProcess::run()
{
    /// redirect in|out|error

    printf("Going to call %s", _exe_path.c_str());

    if (dup2(_in_pipe[PIPE_READ], STDIN_FILENO) == -1)
        return;

    // redirecting stdout
    if (dup2(_out_pipe[PIPE_WRITE], STDOUT_FILENO) == -1)
        return;

    // redirecting stderr
    if (dup2(_out_pipe[PIPE_WRITE], STDERR_FILENO) == -1)
        return;

    // close pipes. For parent only
    close(_in_pipe[PIPE_READ]);
    close(_in_pipe[PIPE_WRITE]);
    close(_out_pipe[PIPE_READ]);
    close(_out_pipe[PIPE_WRITE]);

    /// start program
    string home = string("HOME=") + string(get_current_dir_name());
    char** args = new char*[_arguments.size()+1];

    int i = 0;
    for (auto str : _arguments)
    {
        args[i++] = strdup(str.c_str());
    }
    args[i] = NULL;


    char* env[] = {strdup(home.c_str()), (char*)NULL};
    int r = execve(_exe_path.c_str(), args, env);

    exit(r);
}

bool MProcess::active() const
{
    if (!_init_compite)
        return false;

    return !kill(_process_pid, 0);
}

MProcess& MProcess::operator << (const std::string& in)
{
    if (active())
    {
        write(_in_pipe[PIPE_WRITE], in.c_str(), in.size());
    }
    return *this;
}

MProcess& MProcess::operator >> (std::string& out)
{
    if (active())
    {
        int readed = read(_out_pipe[PIPE_READ], _block, BLOCK_SIZE);
        out = string(_block, _block + readed);
    }

    return *this;
}

