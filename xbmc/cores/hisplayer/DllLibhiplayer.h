#pragma once

#include "system.h"
#include "DynamicDll.h"

class DllLibHiplayerInterface
{
public:
  virtual ~DllLibHiplayerInterface() {};

  virtual int player_init(void)=0;
	virtual int player_setDataSource(const char * url) = 0; 
	virtual int player_start() = 0; 
	virtual int player_getStatus() = 0; 
	virtual int player_stop() = 0; 
};

class DllLibHiplayer : public DllDynamic, DllLibHiplayerInterface
{
  DECLARE_DLL_WRAPPER(DllLibHiplayer, "libhiplayer.so")

  DEFINE_METHOD0(int,            player_init)
  DEFINE_METHOD1(int,            player_setDataSource,          (const char * p1))
  DEFINE_METHOD0(int,            player_start)
  DEFINE_METHOD0(int,            player_getStatus)
  DEFINE_METHOD0(int,            player_stop)

  BEGIN_METHOD_RESOLVE()
    RESOLVE_METHOD(player_init)
    RESOLVE_METHOD(player_setDataSource)
    RESOLVE_METHOD(player_start)
    RESOLVE_METHOD(player_getStatus)
    RESOLVE_METHOD(player_stop)
  END_METHOD_RESOLVE()
};

