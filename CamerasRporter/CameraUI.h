#pragma once

#include <QDateTime>
#include <QtAV/AVPlayer.h>
#include <QtAVWidgets/GraphicsItemRenderer.h>

#include <QtWidgets/QWidget>

#include "ui_CameraUI.h"

class CameraUI : public QWidget
{
	Q_OBJECT

public:
	CameraUI(QWidget *parent = Q_NULLPTR);
	void setName(QString sName)
	{
		m_sName = sName;
	}

	bool setPath(QString sPath);
	void resizeEvent(QResizeEvent* pEvent);
	const QSet<QDate>& getDaetSet() const
	{
		return m_setDate;
	}
	QMap<QDateTime, QDateTime> getTimeSet(const QDate& mDate);
	bool playTime(const QDateTime& timeToPlay);

public slots:
	void playFile(int);
	void slotAudio(bool);
	void slotPlay(bool);
	void slotPrevious();
	void slotNext();
	void slotSetTime(int);
	void slotSliderAction(int);
	void slotSaveImage();
	void durationChanged(qint64 duration);
	void mediaStatusChanged(QtAV::MediaStatus status);
	void positionChanged(qint64 position);
	void stateChanged(QtAV::AVPlayer::State eState);

signals:
	void beginPlay(QDateTime timeCurrent);
	void endPlay(QDateTime timeNext);
	void timeChanged(QDateTime timeNext);

protected:
	struct SFileTimeInfo
	{
		QString		sFilename;
		QDateTime	timeBegin;
		QDateTime	timeEnd;
	};

private:
	Ui::Form	ui;

	int			m_iFrameInterval;
	int			m_iTimeScaleInUI;

	QString		m_sName;
	QString		m_sPath;

	QVector<SFileTimeInfo>	m_vFileList;
	int						m_iCurrentIndx;	//TODO: should use iterator?
	QSet<QDate>				m_setDate;

	QtAV::AVPlayer				m_mPlayer;
	QtAV::GraphicsItemRenderer*	m_pVideoItem;

};
