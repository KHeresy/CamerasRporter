#include "CameraUI.h"

#include <QDir>

CameraUI::CameraUI(QWidget *parent) :QWidget(parent)
{
	ui.setupUi(this);

	m_mPlayer.setVideoOutput(ui.videoArea);
}

void CameraUI::playFile(int iIdx)
{
	m_mPlayer.setMedia(QUrl::fromLocalFile(m_sPath + m_vFileList[iIdx]));
	m_mPlayer.play();
}

bool CameraUI::setPath(QString sPath)
{
	m_sPath = sPath + m_sName;
	QDir dirFolder = m_sPath;
	if (dirFolder.exists())
	{
		m_vFileList = dirFolder.entryList({ "*.mp4" }, QDir::Files, QDir::Name);
		ui.comboFileList->clear();
		ui.comboFileList->addItems(m_vFileList);
		return true;
	}
	return false;
}
