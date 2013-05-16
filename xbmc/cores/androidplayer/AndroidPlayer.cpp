
#include "AndroidPlayer.h"

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

#include "settings/VideoSettings.h"

CAndroidPlayer::CAndroidPlayer(IPlayerCallback &callback)
	: IPlayer(callback),CThread("CAndroidPlayer"), m_dll(0), m_ready(true)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::CAndroidPlayer()");
	if(m_dll == 0)
	{
		m_dll = new AndroidMediaDll; 
		m_dll->Load();
	}
	if (m_dll->IsLoaded())
	{
		CLog::Log(LOGDEBUG,"Load ^.^");
	}
	else
	{
		CLog::Log(LOGDEBUG,"Load - -b");
	}

	if (!CAEFactory::Suspend())
	{
		CLog::Log(LOGNOTICE,"%s: Failed to suspend AudioEngine before launching external player", __FUNCTION__);
	}
}

CAndroidPlayer::~CAndroidPlayer()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::~CAndroidPlayer()***************************************************");
	CloseFile();
	if (m_dll->IsLoaded())
	{
		m_dll->Unload();
	}
	//delete m_dll; 
	//m_dll = NULL;

	CLog::Log(LOGDEBUG, "AndroidPlayer::~CAndroidPlayer() Resume CAEFactory");
	if (!CAEFactory::Resume())
	{
		CLog::Log(LOGFATAL, "%s: Failed to restart AudioEngine after return from external player",__FUNCTION__);
	}
}


bool CAndroidPlayer::OpenFile(const CFileItem& file, const CPlayerOptions& options)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::OpenFile() - %s", file.GetPath().c_str());
	m_dll->reset();
	m_dll->setDataSource(file.GetPath().c_str());
	m_dll->prepare();
	g_renderManager.PreInit();

	m_ready.Reset();
	CLog::Log(LOGDEBUG, "AndroidPlayer::New Thread Coming In! ", file.GetPath().c_str());
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
	//return false;
}

bool CAndroidPlayer::CloseFile()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::CloseFile() stop");
	m_dll->stop();
	StopThread();
	return true;
}

bool CAndroidPlayer::IsPlaying() const
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::IsPlaying() - %d %d", m_dll->isPlaying(), !!m_dll->getCurrentPosition());
	return m_dll->isPlaying() || !!m_dll->getCurrentPosition();
}

bool CAndroidPlayer::CanPause()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::CanPause()");
	return true;
};

void CAndroidPlayer::Pause()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::Pause()");
	if(!IsPaused())
	{
		CLog::Log(LOGDEBUG, "AndroidPlayer::Pause() - pause");
		m_dll->pause();
	}
	else
	{
		CLog::Log(LOGDEBUG, "AndroidPlayer::Pause() - start");
		m_dll->start();
	}

}

bool CAndroidPlayer::IsPaused() const
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::IsPaused() - %d", (!m_dll->isPlaying() && !!m_dll->getCurrentPosition()));
	return (!m_dll->isPlaying() && !!m_dll->getCurrentPosition());
}

bool CAndroidPlayer::CanSeek()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::CanSeek()");
	return IsPlaying();
}

void CAndroidPlayer::Seek(bool bPlus, bool bLargeStep)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::Seek(%d, %d)", bPlus, bLargeStep);
	int step = bLargeStep ? 500 : 1000;
	m_dll->seekTo(m_dll->getCurrentPosition() + (2 * bPlus - 1) * step);
}

void CAndroidPlayer::SeekPercentage(float fPercent)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::SeekPercentage(%f)", fPercent);
	m_dll->seekTo(fPercent * GetTotalTime() / 100);
}

float CAndroidPlayer::GetPercentage()
{
	//CLog::Log(LOGDEBUG, "AndroidPlayer::GetPercentage() - %d / %d = %f", m_dll->getCurrentPosition(), m_dll->getDuration(), (float)m_dll->getCurrentPosition() / m_dll->getDuration());
	return 100.0 * m_dll->getCurrentPosition() / m_dll->getDuration();
}

void CAndroidPlayer::SetMute(bool bOnOff)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::SetMute(%d)", bOnOff);

}

void CAndroidPlayer::SetVolume(float volume)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::SetVolume(%f)", volume);
	m_dll->setVolume(volume, volume);
}

void CAndroidPlayer::Update(bool bPauseDrawing)
{
	g_renderManager.Update(bPauseDrawing);
	CLog::Log(LOGDEBUG, "AndroidPlayer::Update()");
}

bool CAndroidPlayer::ControlsVolume()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::ControlsVolume()");
	return true;
}

void CAndroidPlayer::GetAudioInfo( CStdString& strAudioInfo)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::GetAudioInfo()");
}

void CAndroidPlayer::GetVideoInfo( CStdString& strVideoInfo)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::GetVideoInfo()");
}
void CAndroidPlayer::GetGeneralInfo( CStdString& strVideoInfo)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::GetGeneralInfo()");

}

int64_t CAndroidPlayer::GetTotalTime()
{
	//CLog::Log(LOGDEBUG, "AndroidPlayer::GetTotalTime() - %d", m_dll->getDuration());
	return m_dll->getDuration();
}

int CAndroidPlayer::GetPictureWidth()
{
	//CLog::Log(LOGDEBUG, "AndroidPlayer::GetPictureWidth() - %d ", m_dll->getVideoWidth());
	return m_dll->getVideoWidth();
}

int CAndroidPlayer::GetPictureHeight()
{
	//CLog::Log(LOGDEBUG, "AndroidPlayer::GetPictureHeight() - %d", m_dll->getVideoHeight());
	return m_dll->getVideoHeight();
}


void CAndroidPlayer::OnStartup()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::OnStartup()");
	int width  = GetPictureWidth();
	int height = GetPictureHeight();
	double fFrameRate = GetActualFPS();
	unsigned int flags = 0;
	flags |= CONF_FLAGS_FULLSCREEN;
	CStdString formatstr = "BYPASS";
	CLog::Log(LOGDEBUG,"%s - change configuration. %dx%d. framerate: %4.2f. format: %s",
		__FUNCTION__, width, height, fFrameRate, formatstr.c_str());
	m_dll->start();

	g_renderManager.IsConfigured();
	if (!g_renderManager.Configure(width, height, width, height, fFrameRate, flags, RENDER_FMT_BYPASS, 0, 0))
	{
		CLog::Log(LOGERROR, "%s - failed to configure renderer", __FUNCTION__);
	}
	if (!g_renderManager.IsStarted())
	{
		CLog::Log(LOGERROR, "%s - renderer not started", __FUNCTION__);
	}

	//g_renderManager.RegisterRenderUpdateCallBack((const void*)this, RenderUpdateCallBack);
	m_ready.Set();

}

void CAndroidPlayer::OnExit()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::OnExit()");
	g_renderManager.UnInit();
}

void CAndroidPlayer::Process()
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::Process()");

	while (IsPlaying())
	{
		CLog::Log(LOGDEBUG, "AndroidPlayer::Process() Sec");
		usleep(250*1000);
	}
	CLog::Log(LOGDEBUG, "AndroidPlayer::Process() died.######################################################");
}

float CAndroidPlayer::GetActualFPS()
{
	float video_fps = 25;//CalculateFPS(m_video_fps_integer, m_video_fps_decimal);
	return video_fps;
}

//float CAndroidPlayer::CalculateFPS(int fpsInteger,int fpsDecimal)
//{
//	float decimal_part = (float)fpsDecimal;
//
//	while(fpsDecimal != 0){
//		fpsDecimal = fpsDecimal / 10;
//		decimal_part /= 10.0; 
//	}
//
//	return (fpsInteger + decimal_part);
//}

void CAndroidPlayer::RenderUpdateCallBack(const void *ctx, const CRect &SrcRect, const CRect &DestRect)
{
	CLog::Log(LOGDEBUG, "AndroidPlayer::RenderUpdateCallBack()");
	//CAndroidPlayer *player = (CAndroidPlayer*)ctx;
	//player->SetVideoRect(SrcRect, DestRect);
}


