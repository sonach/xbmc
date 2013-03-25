
#include "HISPlayer.h"
#include "DllLibhiplayer.h"

#include "system.h"

#include "Application.h"
#include "FileItem.h"
#include "GUIInfoManager.h"
#include "video/VideoThumbLoader.h"
#include "Util.h"
#include "cores/AudioEngine/AEFactory.h"
#include "cores/AudioEngine/AEFactory.h"
#include "cores/VideoRenderers/RenderFlags.h"
#include "cores/VideoRenderers/RenderFormats.h"
#include "cores/VideoRenderers/RenderManager.h"
#include "dialogs/GUIDialogKaiToast.h"
#include "filesystem/File.h"
#include "filesystem/SpecialProtocol.h"
#include "guilib/GUIWindowManager.h"
#include "guilib/LocalizeStrings.h"
#include "settings/AdvancedSettings.h"
#include "settings/GUISettings.h"
#include "settings/Settings.h"
#include "settings/VideoSettings.h"
#include "threads/SingleLock.h"
#include "utils/log.h"
#include "utils/TimeUtils.h"
#include "utils/URIUtils.h"
#include "utils/LangCodeExpander.h"
#include "utils/Variant.h"
#include "windowing/WindowingFactory.h"

#include "xbmc/cores/dvdplayer/DVDClock.h"
#include "xbmc/cores/dvdplayer/DVDPlayerSubtitle.h"
#include "xbmc/cores/dvdplayer/DVDDemuxers/DVDDemuxVobsub.h"
#include "settings/VideoSettings.h"

#include "hisplayer_type.h"

CHISPlayer::CHISPlayer(IPlayerCallback &callback)
	: IPlayer(callback),CThread("CHISPlayer"),m_ready(true)
{
  m_dll = new DllLibHiplayer; 
  m_dll->Load();

  m_pid = -1;
  m_speed = 0;
  m_paused = false;

  #if defined(_DEBUG)
    m_log_level = 5;
  #else
    m_log_level = 3;
  #endif

  m_bAbortRequest = false;

  m_file_info = new HI_FORMAT_FILE_INFO_S;

  if (!CAEFactory::Suspend())
  {
    CLog::Log(LOGNOTICE,"%s: Failed to suspend AudioEngine before launching external player", __FUNCTION__);
  }
}

CHISPlayer::~CHISPlayer()
{
  CloseFile();
  
  delete m_dll; 
  m_dll = NULL;

  delete m_file_info;
  m_file_info = NULL;

  if (!CAEFactory::Resume())
  {
    CLog::Log(LOGFATAL, "%s: Failed to restart AudioEngine after return from external player",__FUNCTION__);
  }
}

bool CHISPlayer::OpenFile(const CFileItem &file, const CPlayerOptions &options)
{
  try
  {
    CLog::Log(LOGNOTICE, "CHISPlayer: Opening: %s", file.GetPath().c_str());
    
    if (IsRunning())
      CloseFile();

    m_bAbortRequest = false;

    m_item = file;
    m_options = options;

    m_elapsed_ms  =  0;
    m_duration_ms =  0;
	m_starttime_ms = (int64_t)(options.starttime * 1000);

    m_audio_info  = "none";
    m_audio_delay = 0;
    m_audio_passthrough_ac3 = g_guiSettings.GetBool("audiooutput.ac3passthrough");
    m_audio_passthrough_dts = g_guiSettings.GetBool("audiooutput.dtspassthrough");

    m_video_info  = "none";
    m_video_width    =  0;
    m_video_height   =  0;
    m_video_fps_integer = 25;
	m_video_fps_decimal = 0;

    m_show_mainvideo = -1;
    m_dst_rect.SetRect(0, 0, 0, 0);
    m_zoom           = -1;
    m_contrast       = -1;
    m_brightness     = -1;

    ClearStreamInfos();

    if (m_item.IsDVDFile() || m_item.IsDVD())
      return false;

    m_ready.Reset();

    g_renderManager.PreInit();

    Create();
    if (!m_ready.WaitMSec(100))
    {
      CGUIDialogBusy *dialog = (CGUIDialogBusy*)g_windowManager.GetWindow(WINDOW_DIALOG_BUSY);
      dialog->Show();
      while (!m_ready.WaitMSec(1))
        g_windowManager.ProcessRenderLoop(false);
      dialog->Close();
    }

    if (m_bStop || m_bAbortRequest)
      return false;

    return true;
  }
  catch (...)
  {
    CLog::Log(LOGERROR, "%s - Exception thrown on open", __FUNCTION__);
    return false;
  }
}

bool CHISPlayer::CloseFile(){
  CLog::Log(LOGDEBUG, "CHISPlayer::CloseFile");
  
  m_bAbortRequest = true;

  CLog::Log(LOGDEBUG, "CHISPlayer: waiting for threads to exit");
  
  StopThread();

  CLog::Log(LOGDEBUG, "CHISPlayer: finished waiting");

  g_renderManager.UnInit();

  return true;
}

bool CHISPlayer::IsPlaying() const
{
  return !m_bStop;
}

void CHISPlayer::Pause()
{
  CLog::Log(LOGDEBUG, "CHISPlayer::Pause");
  CSingleLock lock(m_his_csection);

  if ((m_pid < 0) && m_bAbortRequest)
    return;

  if (m_paused)
    m_dll->player_resume(m_pid);
  else
    m_dll->player_pause(m_pid);

  m_paused = !m_paused;
}

bool CHISPlayer::IsPaused() const
{
  return m_paused;
}

bool CHISPlayer::HasVideo() const
{
  return m_video_count > 0;
}

bool CHISPlayer::HasAudio() const
{
  return m_audio_count > 0;
}

void CHISPlayer::ToggleFrameDrop()
{
  CLog::Log(LOGDEBUG, "CAMLPlayer::ToggleFrameDrop");
}

bool CHISPlayer::CanSeek()
{
  return GetTotalTime() > 0;
}

void CHISPlayer::Seek(bool bPlus, bool bLargeStep)
{
  CLog::Log(LOGDEBUG, "CHISPlayer::Seek(%d, %d)", bPlus, bLargeStep);
}

bool CHISPlayer::SeekScene(bool bPlus)
{
  CLog::Log(LOGDEBUG, "CHISPlayer::SeekScene");
  return false;
}

void CHISPlayer::SeekPercentage(float fPercent)
{
  CLog::Log(LOGDEBUG, "CHISPlayer::SeekPercentage(%f)", fPercent);
  CSingleLock lock(m_his_csection);

  // force updated to m_elapsed_ms, m_duration_ms.
  GetStatus();

  if (m_duration_ms)
  {
    int64_t seek_ms = fPercent * m_duration_ms / 100.0;
    if (seek_ms <= 1000)
      seek_ms = 1000;

    // do seek here
    g_infoManager.SetDisplayAfterSeek(100000);
    SeekTime(seek_ms);
    m_callback.OnPlayBackSeek((int)seek_ms, (int)(seek_ms - m_elapsed_ms));
    g_infoManager.SetDisplayAfterSeek();
  }
}

float CHISPlayer::GetPercentage()
{
  //CLog::Log(LOGDEBUG, "CHISPlayer::GetPercentage");
  GetStatus();
  if (m_duration_ms)
    return 100.0f * (float)m_elapsed_ms/(float)m_duration_ms;
  else
    return 0.0f;
}

void CHISPlayer::SetVolume(float volume)
{
  CLog::Log(LOGDEBUG, "CHISPlayer::SetVolume(%f)", volume);
  CSingleLock lock(m_his_csection);
  // volume is a float percent from 0.0 to 1.0
  if(m_pid > 0)
  	m_dll->player_setVolume(m_pid, (int)(volume*100));
}

void CHISPlayer::GetAudioInfo(CStdString &strAudioInfo)
{
  CSingleLock lock(m_his_csection);

  int aformat = (int)(m_file_info->astProgramInfo[m_program_index].astAudStream[m_audio_index].u32Format);

  char codecname[32];

  m_dll->player_getAudioCodecName(aformat, codecname);

  strAudioInfo.Format("Audio stream (%s) [Kb/s:%.2f]",
    codecname,
    (double)m_file_info->astProgramInfo[0].astAudStream[m_audio_index].u32Bps / 1024.0);
}

void CHISPlayer::GetVideoInfo(CStdString &strVideoInfo)
{
  CSingleLock lock(m_his_csection);

  int vformat = (int)(m_file_info->astProgramInfo[m_program_index].stVidStream.u32Format);

  //CLog::Log(LOGDEBUG, "vformat:%d", vformat);

  char codecname[32];

  m_dll->player_getVideoCodecName(vformat, codecname);

  strVideoInfo.Format("Video stream (%s) [fr:%.3f Mb/s:%.2f]",
    codecname,
    CalculateFPS(m_video_fps_integer, m_video_fps_decimal),
    (double)m_file_info->astProgramInfo[m_program_index].stVidStream.u32Bps/(1024.0 * 1024.0));
}

int CHISPlayer::GetAudioStreamCount()
{
  //CLog::Log(LOGDEBUG, "CAMLPlayer::GetAudioStreamCount");
  return m_audio_count;
}

int CHISPlayer::GetAudioStream()
{
  //CLog::Log(LOGDEBUG, "CAMLPlayer::GetAudioStream");
  return m_audio_index;
}

void CHISPlayer::GetAudioStreamName(int iStream, CStdString &strStreamName)
{
  CSingleLock lock(m_his_csection);

  strStreamName.Format("Undefined");

  if((m_audio_index < 0)||(m_audio_index > (m_audio_count -1))){
    return;
  }

  //string temp(m_file_info->astProgramInfo[m_program_index].astAudStream[m_program_index].aszAudLang);
  //CLog::Log(LOGDEBUG, "language:%s", temp.c_str());

   CStdString name;
   g_LangCodeExpander.Lookup( name, m_file_info->astProgramInfo[m_program_index].astAudStream[iStream].aszAudLang);
   strStreamName = name;

   CLog::Log(LOGDEBUG, "g_langCodeExpander.lookup:%s", strStreamName.c_str());

}

void CHISPlayer::SetAudioStream(int SetAudioStream)
{
  CLog::Log(LOGDEBUG, "CHISPlayer::SetAudioStream=%d", SetAudioStream);

  CSingleLock lock(m_his_csection);

  if((m_audio_index < 0)||(m_audio_index > (m_audio_count -1))){
    return;
  }

  m_audio_index = SetAudioStream;

  if(m_dll->player_setAudioStream(m_pid, SetAudioStream)){
    CLog::Log(LOGDEBUG, "player_setAudioStream failed");
  }
}

void CHISPlayer::SetAVDelay(float fValue)
{

}

float CHISPlayer::GetAVDelay()
{
  return (float)m_audio_delay / 1000.0;
}

void CHISPlayer::Update(bool bPauseDrawing)
{
  g_renderManager.Update(bPauseDrawing);
}

void CHISPlayer::GetVideoRect(CRect& SrcRect, CRect& DestRect)
{
  g_renderManager.GetVideoRect(SrcRect, DestRect);
}

void CHISPlayer::GetVideoAspectRatio(float &fAR)
{
  fAR = g_renderManager.GetAspectRatio();
}

int CHISPlayer::GetChapterCount()
{
  return 0;
}

int CHISPlayer::GetChapter()
{
  
}

void CHISPlayer::GetChapterName(CStdString& strChapterName)
{
  
}

int CHISPlayer::SeekChapter(int chapter_index)
{
  return 0;
}

float CHISPlayer::GetActualFPS()
{
  float video_fps = CalculateFPS(m_video_fps_integer, m_video_fps_decimal);
  return video_fps;
}

void CHISPlayer::SeekTime(int64_t seek_ms)
{
  CLog::Log(LOGDEBUG, "SeekTime(%ld)", seek_ms);
  CSingleLock lock(m_his_csection);

  // we cannot seek if paused
  if (m_paused)
    return;

  if (seek_ms <= 0)
    seek_ms = 100;

  if(m_dll->player_seek(m_pid, seek_ms)){
    CLog::Log(LOGDEBUG, "player_seek failed");
  }
}

int64_t CHISPlayer::GetTime()
{
  return m_elapsed_ms;
}

int64_t CHISPlayer::GetTotalTime()
{
  return m_duration_ms;
}

int CHISPlayer::GetAudioBitrate()
{
  CSingleLock lock(m_his_csection);
  

  return (int)m_file_info->astProgramInfo[0].astAudStream[0].u32Bps;
}

int CHISPlayer::GetVideoBitrate()
{
  CSingleLock lock(m_his_csection);
  

  return (int)m_file_info->astProgramInfo[0].stVidStream.u32Bps;
}

int CHISPlayer::GetSourceBitrate()
{
  CLog::Log(LOGDEBUG, "CHISPlayer::GetSourceBitrate");
  return 0;
}

int CHISPlayer::GetChannels()
{
  CSingleLock lock(m_his_csection);
  
  return (int)m_file_info->astProgramInfo[0].astAudStream[0].u16Channels;
}

int CHISPlayer::GetBitsPerSample()
{
  CLog::Log(LOGDEBUG, "CHISPlayer::GetBitsPerSample");
  CSingleLock lock(m_his_csection);
  return (int)m_file_info->astProgramInfo[0].astAudStream[0].u16BitPerSample;
}

int CHISPlayer::GetSampleRate()
{
  CSingleLock lock(m_his_csection);
  return (int)m_file_info->astProgramInfo[0].astAudStream[0].u32SampleRate;
}

CStdString CHISPlayer::GetAudioCodecName()
{
  //CLog::Log(LOGDEBUG, "CHISPlayer::GetAudioCodecName");
  CStdString strAudioCodec = "";
#if 1
  if ((m_audio_index < 0) || (m_audio_index > (m_audio_count - 1)))
    return strAudioCodec;

  int aformat = (int)(m_file_info->astProgramInfo[m_program_index].astAudStream[m_audio_index].u32Format);

  //CLog::Log(LOGDEBUG, "aformat:%d", aformat);

  char codecname[32];

  if(m_dll)
    m_dll->player_getAudioCodecName(aformat, codecname);

  //CLog::Log(LOGDEBUG, "after player_getAudioCodecName, codecname=%s", codecname);

  strAudioCodec = codecname;
#endif

  return strAudioCodec;
}

CStdString CHISPlayer::GetVideoCodecName()
{
  //CLog::Log(LOGDEBUG, "CHISPlayer::GetVideoCodecName");
  CStdString strVideoCodec = "";
#if 1
  if ((m_video_index < 0) || (m_video_index > (m_video_count - 1)))
    return strVideoCodec;

  int vformat = (int)(m_file_info->astProgramInfo[m_program_index].stVidStream.u32Format);

  //CLog::Log(LOGDEBUG, "vformat:%d", vformat);

  char codecname[32];

  m_dll->player_getVideoCodecName(vformat, codecname);

  //CLog::Log(LOGDEBUG, "after player_getVideoCodecName, codecname=%s", codecname);

  strVideoCodec = codecname;
#endif

  return strVideoCodec;
}

int CHISPlayer::GetPictureWidth()
{
  //CLog::Log(LOGDEBUG, "CAMLPlayer::GetPictureWidth(%d)", m_video_width);
  return m_video_width;
}

int CHISPlayer::GetPictureHeight()
{
  //CLog::Log(LOGDEBUG, "CAMLPlayer::GetPictureHeight(%)", m_video_height);
  return m_video_height;
}

bool CHISPlayer::GetStreamDetails(CStreamDetails &details)
{
  //CLog::Log(LOGDEBUG, "CAMLPlayer::GetStreamDetails");
  return false;
}

void CHISPlayer::ToFFRW(int iSpeed)
{
  CLog::Log(LOGDEBUG, "CHISPlayer::ToFFRW: iSpeed(%d), m_speed(%d)", iSpeed, m_speed);
  CSingleLock lock(m_his_csection);

  if (m_bAbortRequest)
    return;

  if (m_speed != iSpeed)
  {
    if(m_dll->player_trickplay(m_pid, iSpeed)){
      CLog::Log(LOGERROR, "call player_trickplay failed");
	  return;
	}

    m_speed = iSpeed;
  }
}

void CHISPlayer::OnStartup()
{
  //m_CurrentVideo.Clear();
  //m_CurrentAudio.Clear();
  //m_CurrentSubtitle.Clear();

  //CThread::SetName("CAMLPlayer");
}

void CHISPlayer::OnExit()
{
  //CLog::Log(LOGNOTICE, "CAMLPlayer::OnExit()");

  m_bStop = true;
  // if we didn't stop playing, advance to the next item in xbmc's playlist
  if (m_options.identify == false)
  {
    if (m_bAbortRequest)
      m_callback.OnPlayBackStopped();
    else
      m_callback.OnPlayBackEnded();
  }
  // set event to inform openfile something went wrong
  // in case openfile is still waiting for this event
  m_ready.Set();
}

void CHISPlayer::Process()
{
  CLog::Log(LOGNOTICE, "CHISPlayer::Process");
  try
  {
    CJobManager::GetInstance().Pause(kJobTypeMediaFlags);

    if (CJobManager::GetInstance().IsProcessing(kJobTypeMediaFlags) > 0)
    {
      if (!WaitForPausedThumbJobs(20000))
      {
        CJobManager::GetInstance().UnPause(kJobTypeMediaFlags);
        throw "CHISPlayer::Process:thumbgen jobs still running !!!";
      }
    }

    CStdString url = m_item.GetPath();
    
    CLog::Log(LOGDEBUG, "CHISPlayer::Process: URL=%s", url.c_str());

    if (m_dll->player_init() != 0)
    {
      CLog::Log(LOGDEBUG, "player init failed");
      throw "CHISPlayer::Process:player init failed";
    }
    CLog::Log(LOGDEBUG, "player init......");
    usleep(50 * 1000);
	
    if(m_dll->player_create((HI_HANDLE *)&m_pid) != 0){
      CLog::Log(LOGDEBUG, "player create failed");
      throw "CHISPlayer::Process:player create failed";
	}

	CLog::Log(LOGDEBUG, "m_pid=%d", m_pid);
	
    if (m_pid < 0)
    {
      if (m_log_level > 5)
        CLog::Log(LOGDEBUG, "player create failed! error = %d", m_pid);
      throw "CAMLPlayer::Process:player create failed";
    }


    if(0 != m_dll->player_setDataSource(m_pid, url.c_str())){
      throw "CHISPlayer::Process:player_setDataSource() failed";
    }else{
      if(m_starttime_ms){
        SeekTime(m_starttime_ms);
	  }
	}
	
    InitFileInfos();

	if(0 != m_dll->player_play(m_pid)){
        throw "CHISPlayer::Process:player_play failed";
	}

    // wait for playback to start with 20 second timeout
    if (WaitForPlaying(10000))
    {
      m_speed = 1;
      m_callback.OnPlayBackSpeedChanged(m_speed);

      // get our initial status.
      GetStatus();

      // restore system volume setting.
      SetVolume(g_settings.m_fVolumeLevel);

      // drop CGUIDialogBusy dialog and release the hold in OpenFile.
      m_ready.Set();

      // we are playing but hidden and all stream fields are valid.
      // check for video in media content
      if (GetVideoStreamCount() > 0)
      {
        SetAVDelay(g_settings.m_currentVideoSettings.m_AudioDelay);

        // setup renderer for bypass. This tell renderer to get out of the way as
        // hw decoder will be doing the actual video rendering in a video plane
        // that is under the GUI layer.
        int width  = GetPictureWidth();
        int height = GetPictureHeight();
        double fFrameRate = GetActualFPS();
        unsigned int flags = 0;

        flags |= CONF_FLAGS_FULLSCREEN;
        CStdString formatstr = "BYPASS";
        CLog::Log(LOGDEBUG,"%s - change configuration. %dx%d. framerate: %4.2f. format: %s",
          __FUNCTION__, width, height, fFrameRate, formatstr.c_str());
        g_renderManager.IsConfigured();
        if (!g_renderManager.Configure(width, height, width, height, fFrameRate, flags, RENDER_FMT_BYPASS, 0, 0))
        {
          CLog::Log(LOGERROR, "%s - failed to configure renderer", __FUNCTION__);
        }
        if (!g_renderManager.IsStarted())
        {
          CLog::Log(LOGERROR, "%s - renderer not started", __FUNCTION__);
        }

        g_renderManager.RegisterRenderUpdateCallBack((const void*)this, RenderUpdateCallBack);
      }

      if (m_options.identify == false)
        m_callback.OnPlayBackStarted();

      bool stopPlaying = false;
      while (!m_bAbortRequest && !stopPlaying)
      {
        HI_SVR_PLAYER_INFO_S player_info;
        int res = m_dll->player_get_playerinfo(m_pid, &player_info);
        if (res != 0)
          throw "player_get_playerinfo return error";
		
        switch(player_info.eStatus)
        {
          case HI_SVR_PLAYER_STATE_PLAY:
		  	//CLog::Log(LOGDEBUG, "player_info.eStatus:HI_SVR_PLAYER_STATE_PLAY");
		  	GetStatus();
			break;
		  case HI_SVR_PLAYER_STATE_STOP:
		  	//CLog::Log(LOGDEBUG, "player_info.eStatus:HI_SVR_PLAYER_STOP");
		  	stopPlaying = true;
			break;
          default:
		  	//CLog::Log(LOGDEBUG, "player_info.eStatus:%d", player_info.eStatus);
		  	break;
        }
        if (!stopPlaying)
          usleep(250 * 1000);
      }
    }
  }
  catch(char* error)
  {
    CLog::Log(LOGERROR, "%s", error);
  }
  catch(...)
  {
    CLog::Log(LOGERROR, "CHISPlayer::Process Exception thrown");
  }

  if (m_log_level > 5)
    CLog::Log(LOGDEBUG, "CHISPlayer::Process stopped");
  if (m_pid > 0)
  {
    //m_dll->player_stop(m_pid);
	if(m_dll->player_destroy(m_pid)){
      CLog::Log(LOGDEBUG, "call player_destory failed");
	}else{
      //CLog::Log(LOGDEBUG, "call player_destory success");
	}
    if(m_dll->player_release(m_pid)){
      CLog::Log(LOGDEBUG, "call player_release failed");
	}else{
      //CLog::Log(LOGDEBUG, "call player_release success");
	}
    m_pid = -1;
  }

  // we are done, hide the mainvideo layer.
  //ShowMainVideo(false);
  m_ready.Set();

  ClearStreamInfos();

  // reset ac3/dts passthough
  //SetAudioPassThrough(AFORMAT_UNKNOWN);
  // let thumbgen jobs resume.
  CJobManager::GetInstance().UnPause(kJobTypeMediaFlags);

  if (m_log_level > 5)
    CLog::Log(LOGDEBUG, "CHISPlayer::Process exit");
}

void CHISPlayer::GetRenderFeatures(std::vector<int> &renderFeatures)
{
  renderFeatures.push_back(RENDERFEATURE_ZOOM);
  //renderFeatures.push_back(RENDERFEATURE_CONTRAST);
  //renderFeatures.push_back(RENDERFEATURE_BRIGHTNESS);
  renderFeatures.push_back(RENDERFEATURE_STRETCH);
}

void CHISPlayer::GetDeinterlaceMethods(std::vector<int> &deinterlaceMethods)
{
  deinterlaceMethods.push_back(VS_INTERLACEMETHOD_DEINTERLACE);
}

void CHISPlayer::GetDeinterlaceModes(std::vector<int> &deinterlaceModes)
{
  deinterlaceModes.push_back(VS_DEINTERLACEMODE_AUTO);
}

void CHISPlayer::GetScalingMethods(std::vector<int> &scalingMethods)
{
}

void CHISPlayer::GetAudioCapabilities(std::vector<int> &audioCaps)
{
  audioCaps.push_back(IPC_AUD_SELECT_STREAM);
  audioCaps.push_back(IPC_AUD_SELECT_OUTPUT);
  audioCaps.push_back(IPC_AUD_OFFSET);
}

void CHISPlayer::GetSubtitleCapabilities(std::vector<int> &subCaps)
{
#if 0
  subCaps.push_back(IPC_SUBS_EXTERNAL);
  subCaps.push_back(IPC_SUBS_SELECT);
  subCaps.push_back(IPC_SUBS_OFFSET);
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
int CHISPlayer::GetVideoStreamCount()
{
  //CLog::Log(LOGDEBUG, "CAMLPlayer::GetVideoStreamCount(%d)", m_video_count);
  return m_video_count;
}

bool CHISPlayer::WaitForPausedThumbJobs(int timeout_ms)
{
  // use m_bStop and Sleep so we can get canceled.
  while (!m_bStop && (timeout_ms > 0))
  {
    if (CJobManager::GetInstance().IsProcessing(kJobTypeMediaFlags) > 0)
    {
      Sleep(100);
      timeout_ms -= 100;
    }
    else
      return true;
  }

  return false;
}

int CHISPlayer::HISPlayerEventCallback(HI_HANDLE hPlayer,HI_SVR_PLAYER_EVENT_S * pstruEvent)
{
  CHISPlayer *hisplayer = dynamic_cast<CHISPlayer*>(g_application.m_pPlayer);
  if(hisplayer){
    //CSingleLock lock(hisplayer->m_hisevent_csection);
	//hisplayer->m_player_event->eEvent = pstruEvent->eEvent;
    //hisplayer->m_player_event->pu8Data = (HI_U8*)malloc(pstruEvent->u32Len);
    //memcpy(m_player_event, pstruEvent->pu8Data, pstruEvent->u32Len);
    //hisplayer->m_player_event->u32Len = pstruEvent->u32Len;
  }

  return 0;
}


bool CHISPlayer::InitFileInfos()
{
  ClearStreamInfos();
  HI_FORMAT_FILE_INFO_S media_info;
  int res = m_dll->player_get_mediainfo(m_pid, &media_info);
  if (res != 0)
    return false;

  m_program_count = media_info.u32ProgramNum;//节目数目

  m_file_info->u32ProgramNum = media_info.u32ProgramNum;
  m_file_info->eStreamType = media_info.eStreamType;
  strcpy(m_file_info->aszFileName, media_info.aszFileName);
  m_file_info->s64Duration = media_info.s64Duration;
  m_file_info->s64StartTime = media_info.s64StartTime;

  m_program_count = media_info.u32ProgramNum;

  for(int i=0; i< media_info.u32ProgramNum; i++){

	m_program_index = i;
    if (HI_FORMAT_INVALID_STREAM_ID != media_info.astProgramInfo[i].stVidStream.s32StreamIndex)
    {
      m_file_info->astProgramInfo[i].stVidStream.s32StreamIndex = media_info.astProgramInfo[i].stVidStream.s32StreamIndex;
	  m_file_info->astProgramInfo[i].stVidStream.u32Format = media_info.astProgramInfo[i].stVidStream.u32Format;
	  m_file_info->astProgramInfo[i].stVidStream.u16Width = media_info.astProgramInfo[i].stVidStream.u16Width;
	  m_file_info->astProgramInfo[i].stVidStream.u16Height = media_info.astProgramInfo[i].stVidStream.u16Height;
	  m_file_info->astProgramInfo[i].stVidStream.u16FpsInteger = media_info.astProgramInfo[i].stVidStream.u16FpsInteger;
	  m_file_info->astProgramInfo[i].stVidStream.u16FpsDecimal = media_info.astProgramInfo[i].stVidStream.u16FpsDecimal;
	  m_file_info->astProgramInfo[i].stVidStream.u32Bps = media_info.astProgramInfo[i].stVidStream.u32Bps;

	  m_video_count = 1;//视频数目，目前HIS默认视频最多为一个
	  m_video_width = (int)m_file_info->astProgramInfo[i].stVidStream.u16Width;
	  m_video_height = (int)m_file_info->astProgramInfo[i].stVidStream.u16Height;
	  m_video_fps_integer = (int)m_file_info->astProgramInfo[i].stVidStream.u16FpsInteger;
	  m_video_fps_decimal = (int)m_file_info->astProgramInfo[i].stVidStream.u16FpsDecimal;
	  m_video_index = 0;
			  
    }
    else
    {
      m_video_count = 0;//视频数目为0
    }

    m_audio_count = (int)media_info.astProgramInfo[i].u32AudStreamNum;//音频数目
    m_audio_index = 0;

    for (int j = 0; j < (int)media_info.astProgramInfo[i].u32AudStreamNum; j++)
    {
	  m_file_info->astProgramInfo[i].astAudStream[j].s32StreamIndex = media_info.astProgramInfo[i].astAudStream[j].s32StreamIndex;
	  m_file_info->astProgramInfo[i].astAudStream[j].u32Format = media_info.astProgramInfo[i].astAudStream[j].u32Format;
	  m_file_info->astProgramInfo[i].astAudStream[j].u32Version = media_info.astProgramInfo[i].astAudStream[j].u32Version;
	  m_file_info->astProgramInfo[i].astAudStream[j].u32SampleRate = media_info.astProgramInfo[i].astAudStream[j].u32SampleRate;
	  m_file_info->astProgramInfo[i].astAudStream[j].u16BitPerSample = media_info.astProgramInfo[i].astAudStream[j].u16BitPerSample;
	  m_file_info->astProgramInfo[i].astAudStream[j].u16Channels = media_info.astProgramInfo[i].astAudStream[j].u16Channels;
	  m_file_info->astProgramInfo[i].astAudStream[j].u32Bps = media_info.astProgramInfo[i].astAudStream[j].u32Bps;

	  for(int kk=0; kk<4; kk++){
        CLog::Log(LOGDEBUG, "lauguage[%d]=%c", kk, media_info.astProgramInfo[i].astAudStream[j].aszAudLang[kk]);
	  }

	  strcpy(m_file_info->astProgramInfo[i].astAudStream[j].aszAudLang, media_info.astProgramInfo[i].astAudStream[j].aszAudLang);
    }
  }

        return true;
}

void CHISPlayer::ClearStreamInfos()
{
  CSingleLock lock(m_his_csection);

  m_program_count = 0;
  m_program_index = -1;

  m_audio_count = 0;
  m_audio_index = -1;

  m_video_count = 0;
  m_video_index = -1;
}

bool CHISPlayer::GetStatus()
{
  //CLog::Log(LOGDEBUG, "CHISPlayer::GetStatus");
  CSingleLock lock(m_his_csection);

  if (m_pid < 0)
    return false;

  HI_SVR_PLAYER_INFO_S player_info;
  int res = m_dll->player_get_playerinfo(m_pid, &player_info);
  if (res != 0)
    return false;
  m_elapsed_ms  = player_info.u64TimePlayed;

  HI_FORMAT_FILE_INFO_S file_info;
  res = m_dll->player_get_mediainfo(m_pid, &file_info);
  m_duration_ms = file_info.s64Duration;

  return true;
}

bool CHISPlayer::WaitForPlaying(int timeout_ms)
{
  CLog::Log(LOGDEBUG, "CHISPlayer::WaitForPlaying timeout_ms=%d", timeout_ms);
  while (!m_bAbortRequest && (timeout_ms > 0))
  {
    HI_SVR_PLAYER_INFO_S player_info;
    int res = m_dll->player_get_playerinfo(m_pid, &player_info);
    if (res != 0)
      return false;

	if(player_info.eStatus == HI_SVR_PLAYER_STATE_PLAY){
	  CLog::Log(LOGDEBUG, "player_info.eStatus == HI_SVR_PLAYER_STATE_PLAY");
      return true;
	}else{
      usleep(100 * 1000);
      timeout_ms -= 100;
	}
  }

  return false;
}


void CHISPlayer::SetVideoRect(const CRect &SrcRect, const CRect &DestRect)
{
  // check if destination rect or video view mode has changed
  if ((m_dst_rect != DestRect) || (m_view_mode != g_settings.m_currentVideoSettings.m_ViewMode))
  {
    m_dst_rect  = DestRect;
    m_view_mode = g_settings.m_currentVideoSettings.m_ViewMode;
  }
  else
  {
    return;
  }

  CRect gui, display, dst_rect;
  gui = g_graphicsContext.GetViewWindow();
  CStdString rectangle_gui;
  rectangle_gui.Format("%i,%i,%i,%i",
    (int)gui.x1, (int)gui.y1,
    (int)gui.Width(), (int)gui.Height());
  CLog::Log(LOGDEBUG, "CHISPlayer::gui-gui(%s)", rectangle_gui.c_str());
  
  display = g_graphicsContext.GetViewWindow();

  CRect res_rect;
  RESOLUTION res = g_graphicsContext.GetVideoResolution();
  res_rect.SetRect(0, 0, g_settings.m_ResInfo[res].iScreenWidth, g_settings.m_ResInfo[res].iScreenHeight);
  CStdString rectangle_res;
  rectangle_res.Format("%i,%i,%i,%i",
    (int)res_rect.x1, (int)res_rect.y1,
    (int)res_rect.Width(), (int)res_rect.Height());
  //CLog::Log(LOGDEBUG, "************************TEST**********************");
  //CLog::Log(LOGDEBUG, "CHISPlayer::res-res(%s)", rectangle_res.c_str());
  
  dst_rect = m_dst_rect;
  
  if (gui != display)
  {
    float xscale = display.Width()  / gui.Width();
    float yscale = display.Height() / gui.Height();
    dst_rect.x1 *= xscale;
    dst_rect.x2 *= xscale;
    dst_rect.y1 *= yscale;
    dst_rect.y2 *= yscale;
  }
  
  dst_rect.x2--;
  dst_rect.y2--;

  dst_rect.x1 = dst_rect.x1*1920/gui.Width();
  dst_rect.x2 = dst_rect.x2*1920/gui.Width();

  dst_rect.y1 = dst_rect.y1*1080/gui.Height();
  dst_rect.y2 = dst_rect.y2*1080/gui.Height();

  if(m_dll->player_setVideoArea(m_pid, dst_rect.x1, dst_rect.y1, dst_rect.x2-dst_rect.x1, dst_rect.y2-dst_rect.y1)){
    CLog::Log(LOGERROR, "CHISPlayer:: player_setVideoArea failed");
  }

  CStdString rectangle;
  rectangle.Format("%i,%i,%i,%i",
    (int)dst_rect.x1, (int)dst_rect.y1,
    (int)dst_rect.Width(), (int)dst_rect.Height());
  CLog::Log(LOGDEBUG, "CHISPlayer::SetVideoRect:dst_rect(%s)", rectangle.c_str());
}

void CHISPlayer::RenderUpdateCallBack(const void *ctx, const CRect &SrcRect, const CRect &DestRect)
{
  //CLog::Log(LOGDEBUG, "RenderUpdateCallBack is called");
  CHISPlayer *player = (CHISPlayer*)ctx;
  player->SetVideoRect(SrcRect, DestRect);
}

float CHISPlayer::CalculateFPS(int fpsInteger,int fpsDecimal)
{
  float decimal_part = (float)fpsDecimal;

   while(fpsDecimal != 0){
        fpsDecimal = fpsDecimal / 10;
        decimal_part /= 10.0; 
    }

   return (fpsInteger + decimal_part);
}

