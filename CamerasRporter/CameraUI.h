#pragma once

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

public slots:
	void playFile(int);

private:
	Ui::Form		ui;

	QMediaPlayer	m_mPlayer;
	QString			m_sName;
	QString			m_sPath;
	QStringList		m_vFileList;
};
