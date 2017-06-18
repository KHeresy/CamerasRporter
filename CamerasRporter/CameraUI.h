#pragma once

#include <QDateTime>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
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
	QMap<QTime, QTime> getTimeSet(const QDate& mDate);

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
	void mediaStatusChanged(QMediaPlayer::MediaStatus status);
	void positionChanged(qint64 position);
	void stateChanged(QMediaPlayer::State eState);

protected:
	struct SFileTimeInfo
	{
		QString		sFilename;
		QDateTime	timeBegin;
		QTime		timeLength;
	};

protected:
	static QTime getTimeEnd(const SFileTimeInfo& sVideo)
	{
		static QTime timeZero(0, 0, 0, 0);
		return (sVideo.timeBegin.addMSecs(timeZero.msecsTo(sVideo.timeLength))).time();
	}

private:
	Ui::Form		ui;

	int				m_iFrameInterval;
	int				m_iTimeScaleInUI;

	QMediaPlayer	m_mPlayer;
	QString			m_sName;
	QString			m_sPath;

	QVector<SFileTimeInfo>	m_vFileList;
	QSet<QDate>			m_setDate;

	QGraphicsVideoItem*	m_pVideoItem;
};
