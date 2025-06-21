#include "Signal.hpp"


volatile sig_atomic_t g_stop = 1;

void Signal::signal_handler(int signum)
{
	(void)signum;
	g_stop = 0;
}

void sigchld_handler(int signum) {
	if (signum != SIGCHLD)
		return ;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
