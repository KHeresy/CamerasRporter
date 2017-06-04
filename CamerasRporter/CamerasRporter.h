#pragma once

#include <array>

#include <QComboBox>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QtWidgets/QMainWindow>

#include "ui_CamerasRporter.h"

class CCameraInfo
{
public:
	QString			m_sFolder;
	QString			m_sPath;
	QStringList		m_vFileList;
	QComboBox*		m_pComboBox;
	QMediaPlayer*	m_pPlayer;

public:
	CCameraInfo();
	bool setPath(const QString& sPath);
	void play(int iIdx);
};

class CamerasRporter : public QMainWindow
{
	Q_OBJECT

public:
	CamerasRporter(QWidget *parent = Q_NULLPTR);

public slots:
	void slotOpenFolder();

private:
	Ui::CamerasRporterClass		ui;
	std::array<CameraUI*, 2>	aCameraUI;
};
