#pragma once

#include "FileItem.h"
#include "cores/IPlayer.h"
#include "dialogs/GUIDialogBusy.h"
#include "threads/Thread.h"
#include "AndroidMediaDll.h"

class AndroidMediaDll;

class CAndroidPlayer : public IPlayer, public CThread
{
public:
	CAndroidPlayer(IPlayerCallback &callback);
	virtual ~CAndroidPlayer();

	virtual bool OpenFile(const CFileItem& file, const CPlayerOptions& options);
	virtual bool CloseFile();
	virtual bool IsPlaying() const;
	virtual bool CanPause();
	virtual void Pause();
	virtual bool IsPaused() const;
	virtual bool HasVideo() const {return true;}
	virtual bool HasAudio() const {return true;}
	virtual bool CanSeek();
	virtual float GetActualFPS();
	virtual void Seek(bool bPlus = true, bool bLargeStep = false);
	virtual void SeekPercentage(float fPercent = 0);
	virtual float GetPercentage();
	virtual void SetMute(bool bOnOff);
	virtual void SetVolume(float volume);
	virtual void Update(bool bPauseDrawing);
	virtual bool ControlsVolume();
	virtual void GetAudioInfo( CStdString& strAudioInfo);
	virtual void GetVideoInfo( CStdString& strVideoInfo);
	virtual void GetGeneralInfo( CStdString& strVideoInfo);

	virtual int64_t GetTotalTime();
	virtual int GetPictureWidth();
	virtual int GetPictureHeight();

	float CalculateFPS(int fpsInteger,int fpsDecimal);
	static void   RenderUpdateCallBack(const void *ctx, const CRect &SrcRect, const CRect &DestRect);

protected:
	virtual void OnStartup();
	virtual void OnExit();
	virtual void Process();

private:
	static AndroidMediaDll *m_dll;
	CEvent m_ready;
};
