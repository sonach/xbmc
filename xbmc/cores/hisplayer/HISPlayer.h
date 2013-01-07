#pragma once

#include "FileItem.h"
#include "cores/IPlayer.h"
#include "dialogs/GUIDialogBusy.h"
#include "threads/Thread.h"

class DllLibHiplayer;

class CHISPlayer : public IPlayer, public CThread
{
public:
  CHISPlayer(IPlayerCallback &callback);
  virtual ~CHISPlayer();

  virtual bool  OpenFile(const CFileItem &file, const CPlayerOptions &options);
	virtual void  Pause();
  virtual bool  IsPaused() const;
  virtual bool  HasVideo() const;
  virtual bool  HasAudio() const;
	virtual void  Seek(bool bPlus = true, bool bLargeStep = false);
	virtual void  GetAudioInfo(CStdString &strAudioInfo);
  virtual void  GetVideoInfo(CStdString &strVideoInfo);
	virtual void  GetGeneralInfo(CStdString &strVideoInfo) {};
  virtual void  Update(bool bPauseDrawing);

protected: 
	virtual void  Process();
	static void   RenderUpdateCallBack(const void *ctx, const CRect &SrcRect, const CRect &DestRect);
	
private:
  DllLibHiplayer         *m_dll;
	CFileItem               m_item;
  CPlayerOptions          m_options;
	CEvent                  m_ready;
};