#include "CamerasRporter.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

CCameraInfo::CCameraInfo()
{
	m_pPlayer = new QMediaPlayer();
}

bool CCameraInfo::setPath(const QString & sPath)
{
	m_sPath = sPath + m_sFolder;
	QDir dirFolder = m_sPath;
	if (dirFolder.exists())
	{
		m_vFileList = dirFolder.entryList({ "*.mp4" }, QDir::Files, QDir::Name);
		m_pComboBox->clear();
		m_pComboBox->addItems(m_vFileList);
		return true;
	}
	return false;
}

void CCameraInfo::play(int iIdx)
{
	m_pPlayer->setMedia(QUrl::fromLocalFile(m_sPath + m_vFileList[iIdx]));
	m_pPlayer->play();
	auto s = m_pPlayer->errorString();
}

CamerasRporter::CamerasRporter(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	aCameraInfo[0].m_sFolder = "/VIDA/";
	aCameraInfo[0].m_pComboBox = ui.comboFileList1;
	aCameraInfo[0].m_pPlayer->setVideoOutput(ui.videoCamera1);

	aCameraInfo[1].m_sFolder = "/VIDB/";
	aCameraInfo[1].m_pComboBox = ui.comboFileList2;
	aCameraInfo[1].m_pPlayer->setVideoOutput(ui.videoCamera2);
}

void CamerasRporter::slotOpenFolder()
{
	QString sFolderName = QFileDialog::getExistingDirectory(this, tr("Select the video folder"), "");
	if (sFolderName != "")
	{
		ui.labelInfo->setText(tr("Path: ") + sFolderName);

		for (CCameraInfo& sCameraInfo : aCameraInfo)
		{
			if (sCameraInfo.setPath( sFolderName))
			{

			}
			else
			{

			}
		}
	}
}

void CamerasRporter::slotCamera1Changed(int iIdx)
{
	aCameraInfo[0].play(iIdx);
}

void CamerasRporter::slotCamera2Changed(int iIdx)
{
	aCameraInfo[1].play(iIdx);
}
