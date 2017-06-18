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
	CamerasRporter(QWidget *parent = Q_NULLPTR);

public slots:
	void slotOpenFolder();

private:
	Ui::CamerasRporterClass		ui;

	CameraProfile				m_mProfile;
	std::array<CameraUI*, 2>	m_aCameraUI;
	QSet<QDate>					m_setDate;
};
