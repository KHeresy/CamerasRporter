#pragma once

#include <array>

#include <QComboBox>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QtWidgets/QMainWindow>

#include "ui_CamerasRporter.h"

#include "CameraProfile.h"

class CamerasRporter : public QMainWindow
{
	Q_OBJECT

public:
	using TTimeLine = QMap<QDateTime, QDateTime>;

public:
	CamerasRporter(QWidget *parent = Q_NULLPTR);

public slots:
	void slotOpenFolder();
	void slotDateChanged(int iIdx);
	void slotTimeChanged(int iIdx);
	void slotSetTimeline(int iTime);
	void slotCamera1FinishPlay(QDateTime timeNext);
	void slotCamera2FinishPlay(QDateTime timeNext);
	void slotCamera1PlayTime(QDateTime timePlay);
	void slotCamera2PlayTime(QDateTime timePlay);

private:
	Ui::CamerasRporterClass		ui;

	CameraProfile				m_mProfile;
	std::array<CameraUI*, 2>	m_aCameraUI;
	QMap<QDate, TTimeLine>		m_mapDateTimeline;
	TTimeLine::const_iterator	m_iterCurrentTimeLine;
};
