#ifndef _CTIMER_H_
#define _CTIMER_H_

//#include "core_system.h"

class cTimer  
{
private:
	bool	_bperfCnt;
	__int64 _perfFreq;

	__int64 _pcTimerStart;
	unsigned long _mmTimerStart;	

	unsigned long _pauseCnt;

	double _pauseTime;
	double _resolution;

public:
	cTimer();
	~cTimer();

	void _Reset(void);
	double _GetTime(void) const;
	double _GetAbsoluteTime(void) const;

	void _Pause(void);
	void _Unpause(void);
};

#endif	//_CTIMER_H_