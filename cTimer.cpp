#include "core_global.h"
//#include "cTimer.h"

cTimer::cTimer()
{
	if(QueryPerformanceFrequency( (LARGE_INTEGER *) &_perfFreq) )
	{
		_bperfCnt = true;

		QueryPerformanceCounter( (LARGE_INTEGER *) &_pcTimerStart);

		_resolution = (1.0 /(double) _perfFreq) * 1000.0;
	}
	else
	{
		_bperfCnt = false;
		_mmTimerStart = timeGetTime();
	}

	_pauseTime = 0.0;
	_pauseCnt = 0;
}

cTimer::~cTimer()
{	}
void cTimer::_Reset(void)
{
	if (_bperfCnt)
		QueryPerformanceCounter( (LARGE_INTEGER *) &_pcTimerStart);
	else
		_mmTimerStart = timeGetTime();

	_pauseTime = _GetTime();

	return;
}

double cTimer::_GetTime(void) const
{
	if (_pauseCnt > 0)
		return _pauseTime;

	__int64 timeElapsed;

	if (_bperfCnt)
	{
		QueryPerformanceCounter( (LARGE_INTEGER *) &timeElapsed);
		timeElapsed -= _pcTimerStart;

		return (double)timeElapsed * _resolution;
	}
	else
	{
		timeElapsed = timeGetTime() - _mmTimerStart;
		return (double) timeElapsed;
	}
}

double cTimer::_GetAbsoluteTime(void) const
{
	
	if (_bperfCnt)
	{
		__int64 timeElapsed;
		QueryPerformanceCounter( (LARGE_INTEGER *) &timeElapsed);
		return timeElapsed * _resolution;
	}
	else
		return (double) timeGetTime();
}

void cTimer::_Pause(void)
{
	if (_pauseCnt == 0)
		_pauseTime = _GetTime();

	_pauseCnt++;

	return;
}

void cTimer::_Unpause(void)
{
	if (_pauseCnt > 0)
		_pauseCnt--;

	if (_pauseCnt == 0)
	{
		if (_bperfCnt)
		{
			__int64 time;
			QueryPerformanceCounter(( LARGE_INTEGER *) &time);
			_pcTimerStart = time - (__int64)(_pauseTime/_resolution);
		}
		else
			_mmTimerStart = (unsigned long)(timeGetTime() - _pauseTime);
	}

	return;
}