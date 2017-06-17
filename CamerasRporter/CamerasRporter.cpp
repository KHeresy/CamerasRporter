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

	m_aCameraUI = { ui.camera1, ui.camera2 };
	m_aCameraUI[0]->setName("/VIDA/");
	m_aCameraUI[1]->setName("/VIDB/");
}

void CamerasRporter::slotOpenFolder()
{
	QString sFolderName = QFileDialog::getExistingDirectory(this, tr("Select the video folder"), "");
	if (sFolderName != "")
	{
		// Updat UI information
		ui.labelInfo->setText(tr("Path: ") + sFolderName);

		// set path to Camera UI
		m_setDate.clear();
		for (CameraUI* pCameraUI : m_aCameraUI)
		{
			if (pCameraUI->isEnabled())
			{
				if (pCameraUI->setPath(sFolderName))
				{
					m_setDate.unite(pCameraUI->getDaetSet());
				}
				else
				{

				}
			}
		}

		// Update combo box
		ui.comboDate->clear();
		for (const QDate& rDate : m_setDate)
			ui.comboDate->addItem(rDate.toString("yyyy/MM/dd"));
	}
}
