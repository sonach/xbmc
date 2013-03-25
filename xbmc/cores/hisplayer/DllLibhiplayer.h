#pragma once

#include "system.h"
#include "DynamicDll.h"

typedef __int64             HI_U64;
typedef __int64             HI_S64;
typedef int                 HI_S32;

typedef struct hiSVR_PLAYER_INFO_S HI_SVR_PLAYER_INFO_S;
typedef struct hiFORMAT_FILE_INFO_S HI_FORMAT_FILE_INFO_S;



class DllLibHiplayerInterface
{
public:
  virtual ~DllLibHiplayerInterface() {};

  int player_init(void);
  int player_create(HI_HANDLE * hplayer);
  int player_stop(HI_HANDLE hplayer);
  int player_destroy(HI_HANDLE hplayer);
  int player_release(HI_HANDLE hplayer);

  int player_play(HI_HANDLE hplayer);
  int player_pause(HI_HANDLE hplayer);
  int player_resume(HI_HANDLE hplayer);
  int player_seek(HI_HANDLE hplayer, HI_S64 s64TimeInMs);
  int player_trickplay(HI_HANDLE hplayer, HI_S32 speed);

  int player_get_playerinfo(HI_HANDLE hplayer, HI_SVR_PLAYER_INFO_S *pstruInfo);
  int player_get_mediainfo(HI_HANDLE hPlayer, HI_FORMAT_FILE_INFO_S *pstruInfo);

  int player_setDataSource(HI_HANDLE hplayer, const char* uri);

  int player_setVideoArea(HI_HANDLE hplayer, int x, int y , int width, int height);

  int player_setVolume(HI_HANDLE hplayer, int volume);

  int player_setAudioStream(HI_HANDLE hplayer, int streamid);

  int player_getAudioCodecName(int aformat, char* codecname);
  int player_getVideoCodecName(int vformat, char* codecname);
};

class DllLibHiplayer : public DllDynamic, DllLibHiplayerInterface
{
  DECLARE_DLL_WRAPPER(DllLibHiplayer, "libhi3716cplayer.so")

  DEFINE_METHOD0(int,            player_init)
  DEFINE_METHOD1(int,            player_create,              (HI_HANDLE *p1))
  DEFINE_METHOD1(int,            player_stop,			     (HI_HANDLE p1))
  DEFINE_METHOD1(int,            player_destroy,			 (HI_HANDLE p1))
  DEFINE_METHOD1(int,            player_release,             (HI_HANDLE p1))
  DEFINE_METHOD1(int,            player_play,                (HI_HANDLE p1))
  DEFINE_METHOD1(int,            player_pause,               (HI_HANDLE p1))
  DEFINE_METHOD1(int,            player_resume,              (HI_HANDLE p1))
  DEFINE_METHOD2(int,            player_seek,                (HI_HANDLE p1, HI_S64 p2))
  DEFINE_METHOD2(int,            player_trickplay,           (HI_HANDLE p1, HI_S32 p2))
  DEFINE_METHOD2(int,            player_get_playerinfo,      (HI_HANDLE p1, HI_SVR_PLAYER_INFO_S * p2))
  DEFINE_METHOD2(int,            player_get_mediainfo,       (HI_HANDLE p1, HI_FORMAT_FILE_INFO_S * p2))
  DEFINE_METHOD2(int,            player_setDataSource,       (HI_HANDLE p1, const char *p2))
  DEFINE_METHOD5(int,            player_setVideoArea,        (HI_HANDLE p1, int p2, int p3, int p4, int p5))
  DEFINE_METHOD2(int,            player_setVolume,           (HI_HANDLE p1, int p2))
  DEFINE_METHOD2(int,            player_setAudioStream,      (HI_HANDLE p1, int p2))
  DEFINE_METHOD2(int,            player_getAudioCodecName,   (int p1, char *p2))
  DEFINE_METHOD2(int,            player_getVideoCodecName,   (int p1, char *p2))

  BEGIN_METHOD_RESOLVE()
    RESOLVE_METHOD(player_init)
    RESOLVE_METHOD(player_create)
    RESOLVE_METHOD(player_stop)
    RESOLVE_METHOD(player_destroy)
    RESOLVE_METHOD(player_release)
    RESOLVE_METHOD(player_play)
    RESOLVE_METHOD(player_pause)
    RESOLVE_METHOD(player_resume)
    RESOLVE_METHOD(player_seek)
    RESOLVE_METHOD(player_trickplay)
    RESOLVE_METHOD(player_get_playerinfo)
    RESOLVE_METHOD(player_get_mediainfo)
    RESOLVE_METHOD(player_setDataSource)
    RESOLVE_METHOD(player_setVideoArea)
    RESOLVE_METHOD(player_setVolume)
    RESOLVE_METHOD(player_setAudioStream)
    RESOLVE_METHOD(player_getAudioCodecName)
    RESOLVE_METHOD(player_getVideoCodecName)
  END_METHOD_RESOLVE()
};

