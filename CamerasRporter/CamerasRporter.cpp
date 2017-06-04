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

	aCameraUI = { ui.camera1, ui.camera2 };
	aCameraUI[0]->setName("/VIDA/");
	aCameraUI[1]->setName("/VIDB/");
}

void CamerasRporter::slotOpenFolder()
{
	QString sFolderName = QFileDialog::getExistingDirectory(this, tr("Select the video folder"), "");
	if (sFolderName != "")
	{
		ui.labelInfo->setText(tr("Path: ") + sFolderName);

		for (CameraUI* pCameraUI : aCameraUI)
		{
			if (pCameraUI->setPath( sFolderName))
			{

			}
			else
			{

			}
		}
	}
}
