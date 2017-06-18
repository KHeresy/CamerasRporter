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
		m_mProfile.m_tVideoLength = QTime(0, 1, 0, 0);
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
		m_mapDateTimeline.clear();
		for (CameraUI* pCameraUI : m_aCameraUI)
		{
			if (pCameraUI->isEnabled())
			{
				if (pCameraUI->setPath(sFolderName))
				{
					const QSet<QDate>& setDate = pCameraUI->getDaetSet();
					for (const QDate& rDate : setDate)
						m_mapDateTimeline.insert(rDate, QMap<QDateTime, QDateTime>());
				}
				else
				{

				}
			}
		}

		// Update combo box
		ui.comboDate->clear();
		for (auto iter = m_mapDateTimeline.begin(); iter != m_mapDateTimeline.end(); ++iter)
			ui.comboDate->addItem(iter.key().toString("yyyy/MM/dd"));
	}
}

void CamerasRporter::slotDateChanged(int iIdx)
{
	auto iter = m_mapDateTimeline.begin() + iIdx;
	const QDate& rDate = iter.key();
	
	for (CameraUI* pCameraUI : m_aCameraUI)
	{
		if (pCameraUI->isEnabled())
		{
			QMap<QTime, QTime> mapList = pCameraUI->getTimeSet(rDate);
			ui.comboTime->clear();
			for (auto iter = mapList.begin(); iter != mapList.end(); ++iter)
			{
				ui.comboTime->addItem(iter.key().toString("HH:ss") + "-" + iter->toString("HH:ss"));
			}
		}
	}
}
