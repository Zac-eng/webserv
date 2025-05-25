#include <csignal>

// volatieこの変数は外部（割り込みシグナル）から突然変更される可能性がある
// sig_atomic_t シグナルハンドラ内で安全に読み書きできる最小単位の型
extern volatile sig_atomic_t g_stop;

class Signal
{
	public:
		static void signal_handler(int signum);
};