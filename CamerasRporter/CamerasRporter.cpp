#include "CamerasRporter.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

CamerasRporter::CamerasRporter(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//TODO: load frome file
	{
		m_mProfile.m_sName = "SGH D720";
		m_mProfile.m_aCameraFolder << "/VIDA/" << "/VIDB/";
		m_mProfile.m_sViedoFilenameFilter = "*.mp4";
		m_mProfile.m_sFilenameToDateTime = "yyyyMMdd_HHmmss";
	}

	m_aCameraUI = { ui.camera1, ui.camera2 };
	m_aCameraUI[0]->setName(m_mProfile.m_aCameraFolder[0]);
	m_aCameraUI[1]->setName(m_mProfile.m_aCameraFolder[1]);
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
