#pragma once

#include "system.h"
#include "DynamicDll.h"


class AndroidMediaDllInterface
{
public:
	virtual int setDataSource(const char* url) = 0;
	virtual  int prepare(void) = 0;
	virtual int start(void) = 0;
	virtual int stop(void) = 0;
	virtual int pause(void) = 0;
	virtual bool isPlaying(void) = 0;

	virtual int getVideoWidth(void) = 0;
	virtual int getVideoHeight(void) = 0;
	virtual int seekTo(int msec) = 0;
	virtual int getCurrentPosition(void) = 0;
	virtual int getDuration(void) = 0;
	virtual int reset(void) = 0;

	virtual int setLooping(int loop) = 0;
	virtual bool isLooping(void) = 0;
	virtual int setVolume(float leftVolume, float rightVolume) = 0;
};

class AndroidMediaDll : public DllDynamic, AndroidMediaDllInterface
{
	DECLARE_DLL_WRAPPER(AndroidMediaDll, "libmediawrapper.so")
	LOAD_SYMBOLS()
	DEFINE_METHOD1(int, setDataSource, (const char* p1))
	DEFINE_METHOD0(int, prepare)
	DEFINE_METHOD0(int, start)
	DEFINE_METHOD0(int, stop)
	DEFINE_METHOD0(int, pause)
	DEFINE_METHOD0(bool, isPlaying)
	DEFINE_METHOD0(int, getVideoWidth)
	DEFINE_METHOD0(int, getVideoHeight)
	DEFINE_METHOD1(int, seekTo, (int p1))
	DEFINE_METHOD0(int, getCurrentPosition)
	DEFINE_METHOD0(int, getDuration)
	DEFINE_METHOD0(int, reset)
	DEFINE_METHOD1(int, setLooping, (int p1))
	DEFINE_METHOD0(bool, isLooping)
	DEFINE_METHOD2(int, setVolume, (float p1, float p2))

	BEGIN_METHOD_RESOLVE()
	RESOLVE_METHOD(setDataSource)
	RESOLVE_METHOD(prepare)
	RESOLVE_METHOD(start)
	RESOLVE_METHOD(stop)
	RESOLVE_METHOD(pause)
	RESOLVE_METHOD(isPlaying)
	RESOLVE_METHOD(getVideoWidth)
	RESOLVE_METHOD(getVideoHeight)
	RESOLVE_METHOD(seekTo)
	RESOLVE_METHOD(getCurrentPosition)
	RESOLVE_METHOD(getDuration)
	RESOLVE_METHOD(reset)
	RESOLVE_METHOD(setLooping)
	RESOLVE_METHOD(isLooping)
	RESOLVE_METHOD(setVolume)
	END_METHOD_RESOLVE()
};

