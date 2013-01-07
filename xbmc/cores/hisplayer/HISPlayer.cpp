
#include "HISPlayer.h"
#include "DllLibhiplayer.h"

#include "system.h"

#include "Application.h"
#include "FileItem.h"
#include "GUIInfoManager.h"
#include "video/VideoThumbLoader.h"
#include "Util.h"
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

// for external subtitles
#include "xbmc/cores/dvdplayer/DVDClock.h"
#include "xbmc/cores/dvdplayer/DVDPlayerSubtitle.h"
#include "xbmc/cores/dvdplayer/DVDDemuxers/DVDDemuxVobsub.h"
#include "settings/VideoSettings.h"


CHISPlayer::CHISPlayer(IPlayerCallback &callback)
	: IPlayer(callback),CThread("CHISPlayer"),m_ready(true)
{
	m_dll = new DllLibHiplayer; 
	m_dll->Load(); 
}

CHISPlayer::~CHISPlayer()
{
	delete m_dll; 
	m_dll = NULL; 
}

bool CHISPlayer::OpenFile(const CFileItem &file, const CPlayerOptions &options)
{
  try
  {
    CLog::Log(LOGNOTICE, "CHISPlayer: Opening: %s", file.GetPath().c_str());

    m_item = file;
    m_options = options;

	  m_ready.Reset();
    g_renderManager.PreInit();

    // create the playing thread
    Create();
    if (!m_ready.WaitMSec(100))
    {
      CGUIDialogBusy *dialog = (CGUIDialogBusy*)g_windowManager.GetWindow(WINDOW_DIALOG_BUSY);
      dialog->Show();
      while (!m_ready.WaitMSec(1))
        g_windowManager.ProcessRenderLoop(false);
      dialog->Close();
    }

    return true;
  }
  catch (...)
  {
    CLog::Log(LOGERROR, "%s - Exception thrown on open", __FUNCTION__);
    return false;
  }
}

void CHISPlayer::Process()
{
  CLog::Log(LOGNOTICE, "CHISPlayer::Process");
	CStdString url = m_item.GetPath();
  try
  {
    CLog::Log(LOGDEBUG, "CHISPlayer::Process: URL=%s", url.c_str());

    if (m_dll->player_init() != 0)
    {
      CLog::Log(LOGDEBUG, "player init failed");
      throw "CHISPlayer::Process:player init failed";
    }
    CLog::Log(LOGDEBUG, "CHISPlayer::Process: player init......");
    usleep(250 * 1000);

		if(m_dll->player_setDataSource(url.c_str()) != 0)
		{
			CLog::Log(LOGDEBUG, "player_setDataSource failed");
      throw "CHISPlayer::Process:player_setDataSource failed";
		}

		int width  = 1280;
    int height = 720;
    double fFrameRate = 24.0;
		unsigned int flags = 0;
    flags |= CONF_FLAGS_FULLSCREEN;

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

		if(m_dll->player_start() != 0)
		{
			CLog::Log(LOGDEBUG, "player_start failed");
      throw "CHISPlayer::Process:player_start failed";
		}

		while(1)
		{
			if(m_dll->player_getStatus() == 0)
			{
				CLog::Log(LOGNOTICE, "play finished! will stop it"); 
			  m_dll->player_stop(); 	
				break; 
			}

			usleep(1000*1000); 
		}
		
		CLog::Log(LOGNOTICE, "exit Process()!"); 
  }
	catch(char* error)
  {
    CLog::Log(LOGERROR, "%s", error);
  }
  catch(...)
  {
    CLog::Log(LOGERROR, "CHISPlayer::Process Exception thrown");
  }
}

void CHISPlayer::Pause()
{
  CLog::Log(LOGDEBUG, "CHISPlayer::Pause");
  
}

bool CHISPlayer::IsPaused() const
{
  return true;
}

bool CHISPlayer::HasVideo() const
{
  return true;
}

bool CHISPlayer::HasAudio() const
{
  return true;
}

void CHISPlayer::Seek(bool bPlus, bool bLargeStep)
{
}

void CHISPlayer::GetAudioInfo(CStdString &strAudioInfo)
{
  
}

void CHISPlayer::GetVideoInfo(CStdString &strVideoInfo)
{
  
}

void CHISPlayer::Update(bool bPauseDrawing)
{
  g_renderManager.Update(bPauseDrawing);
}

void CHISPlayer::RenderUpdateCallBack(const void *ctx, const CRect &SrcRect, const CRect &DestRect)
{
  //CHISPlayer *player = (CHISPlayer*)ctx;
	CLog::Log(LOGDEBUG, "RenderUpdateCallBack");  
}

