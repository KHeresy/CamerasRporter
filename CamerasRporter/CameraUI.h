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

public slots:
	void playFile(int);
	void slotAudio(bool);
	void slotPlay(bool);
	void slotPrevious();
	void slotNext();
	void slotSetTime(int);
	void slotSaveImage();
	void durationChanged(qint64 duration);
	void mediaStatusChanged(QMediaPlayer::MediaStatus status);
	void positionChanged(qint64 position);
	void stateChanged(QMediaPlayer::State eState);

private:
	Ui::Form		ui;

	int		m_iFrameInterval;
	QMediaPlayer	m_mPlayer;
	QString			m_sName;
	QString			m_sPath;
	QStringList		m_vFileList;

	QVector<QDateTime>	m_vDateTimeList;
	QSet<QDate>			m_setDate;

	QGraphicsVideoItem*	m_pVideoItem;
};
