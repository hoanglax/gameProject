#ifndef IMP_TIMER_H
#define IMP_TIMER_H
#include "CommonLib.h"

class ImpTimer
{
public:
	ImpTimer();
	~ImpTimer();

	void start();
	void stop();
	void paused();
	void unpaused();

	int get_ticks();

	bool is_started();
	bool is_paused();

private:
	int start_tick_;
	int pause_tick_;

	bool is_paused_;
	bool is_started_;
};

#endif
