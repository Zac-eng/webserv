#include "Signal.hpp"


volatile sig_atomic_t g_stop = 1;

void Signal::signal_handler(int signum)
{
	(void)signum;
	g_stop = 0;
}