#pragma once

#include "FileItem.h"
#include "cores/IPlayer.h"
#include "dialogs/GUIDialogBusy.h"
#include "threads/Thread.h"

typedef struct hiSVR_PLAYER_EVENT_S HI_SVR_PLAYER_EVENT_S;
typedef unsigned int HI_HANDLE;
typedef struct hiFORMAT_FILE_INFO_S HI_FORMAT_FILE_INFO_S;

class DllLibHiplayer;

class CHISPlayer : public IPlayer, public CThread
{
public:
  CHISPlayer(IPlayerCallback &callback);
  virtual ~CHISPlayer();

  virtual void  RegisterAudioCallback(IAudioCallback* pCallback) {}
  virtual void  UnRegisterAudioCallback()                        {}
  virtual bool  OpenFile(const CFileItem &file, const CPlayerOptions &options);
  virtual bool  QueueNextFile(const CFileItem &file)             {return false;}
  virtual void  OnNothingToQueueNotify()                         {}
  virtual bool  CloseFile();
  virtual bool  IsPlaying() const;
  virtual void  Pause();
  virtual bool  IsPaused() const;
  virtual bool  HasVideo() const;
  virtual bool  HasAudio() const;
  virtual void  ToggleFrameDrop();
  virtual bool  CanSeek();
  virtual void  Seek(bool bPlus = true, bool bLargeStep = false);
  virtual bool  SeekScene(bool bPlus = true);
  virtual void  SeekPercentage(float fPercent = 0.0f);
  virtual float GetPercentage();
  virtual void  SetVolume(float volume);
  virtual void  SetDynamicRangeCompression(long drc)              {}
  virtual void  GetAudioInfo(CStdString &strAudioInfo);
  virtual void  GetVideoInfo(CStdString &strVideoInfo);
  virtual void  GetGeneralInfo(CStdString &strVideoInfo) {};
  virtual void  Update(bool bPauseDrawing);
  virtual void  GetVideoRect(CRect& SrcRect, CRect& DestRect);
  virtual void  GetVideoAspectRatio(float &fAR);
  virtual bool  CanRecord()                                       {return false;};
  virtual bool  IsRecording()                                     {return false;};
  virtual bool  Record(bool bOnOff)                               {return false;};

  virtual void  SetAVDelay(float fValue = 0.0f);
  virtual float GetAVDelay();

  virtual int   GetAudioStreamCount();
  virtual int   GetAudioStream();
  virtual void  GetAudioStreamName(int iStream, CStdString &strStreamName);
  virtual void  SetAudioStream(int iStream);
  virtual void  GetAudioStreamLanguage(int iStream, CStdString &strLanguage) {};

  virtual TextCacheStruct_t* GetTeletextCache()                   {return NULL;};
  virtual void  LoadPage(int p, int sp, unsigned char* buffer)    {};

  virtual int   GetChapterCount();
  virtual int   GetChapter();
  virtual void  GetChapterName(CStdString& strChapterName);
  virtual int   SeekChapter(int iChapter);

  virtual float GetActualFPS();
  virtual void  SeekTime(int64_t iTime = 0);
  virtual int64_t GetTime();
  virtual int64_t GetTotalTime();
  virtual int   GetAudioBitrate();
  virtual int   GetVideoBitrate();
  virtual int   GetSourceBitrate();
  virtual int   GetChannels();
  virtual int   GetBitsPerSample();
  virtual int   GetSampleRate();
  virtual CStdString GetAudioCodecName();
  virtual CStdString GetVideoCodecName();
  virtual int   GetPictureWidth();
  virtual int   GetPictureHeight();
  virtual bool  GetStreamDetails(CStreamDetails &details);
  virtual void  ToFFRW(int iSpeed = 0);
  virtual bool  SkipNext()                                        {return false;}
  virtual bool  IsCaching() const                                 {return false;};
  virtual int   GetCacheLevel() const                             {return -1;};

  virtual bool  IsInMenu() const                                  {return false;};
  virtual bool  HasMenu()                                         {return false;};

  virtual void  DoAudioWork()                                     {};
  virtual bool  OnAction(const CAction &action)                   {return false;};

  virtual bool  GetCurrentSubtitle(CStdString& strSubtitle){return false;};
  virtual CStdString GetPlayerState()                             {return "";};
  virtual bool  SetPlayerState(CStdString state)                  {return false;};

  virtual CStdString GetPlayingTitle()                            {return "";};

  virtual void  GetRenderFeatures(std::vector<int> &renderFeatures);
  virtual void  GetDeinterlaceMethods(std::vector<int> &deinterlaceMethods);
  virtual void  GetDeinterlaceModes(std::vector<int> &deinterlaceModes);
  virtual void  GetScalingMethods(std::vector<int> &scalingMethods);
  virtual void  GetAudioCapabilities(std::vector<int> &audioCaps);
  virtual void  GetSubtitleCapabilities(std::vector<int> &subCaps);

protected:
  virtual void  OnStartup();
  virtual void  OnExit();
  virtual void  Process();

private:
  int           GetVideoStreamCount();
  bool          WaitForPausedThumbJobs(int timeout_ms);
  static int    HISPlayerEventCallback(HI_HANDLE hPlayer, HI_SVR_PLAYER_EVENT_S *pstruEvent);
  bool          InitFileInfos();
  void          ClearStreamInfos();
  bool          GetStatus();
  bool          WaitForPlaying(int timeout_ms);

  void          SetVideoRect(const CRect &SrcRect, const CRect &DestRect);
  static void   RenderUpdateCallBack(const void *ctx, const CRect &SrcRect, const CRect &DestRect);

  float         CalculateFPS(int fpsInteger, int fpsDecimal);

  
  DllLibHiplayer         *m_dll;

  int                     m_speed;
  bool                    m_paused;
  bool                    m_bAbortRequest;
  CEvent                  m_ready;
  CFileItem               m_item;
  CPlayerOptions          m_options;
  int                     m_log_level;

  int64_t                 m_elapsed_ms;
  int64_t                 m_duration_ms;
  int64_t                 m_starttime_ms;

  int                     m_program_index;
  int                     m_program_count;

  int                     m_audio_index;
  int                     m_audio_count;
  CStdString              m_audio_info;
  int                     m_audio_delay;
  bool                    m_audio_passthrough_ac3;
  bool                    m_audio_passthrough_dts;

  int                     m_video_index;
  int                     m_video_count;
  CStdString              m_video_info;
  int                     m_video_width;
  int                     m_video_height;
  int                     m_video_fps_decimal;
  int                     m_video_fps_integer;

  int                     m_show_mainvideo;
  CRect                   m_dst_rect;
  int                     m_view_mode;
  float                   m_zoom;
  int                     m_contrast;
  int                     m_brightness;

  CCriticalSection        m_his_csection;
  
  int                     m_pid;
  HI_FORMAT_FILE_INFO_S * m_file_info;

  HI_SVR_PLAYER_EVENT_S * m_player_event;
};