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

	connect(ui.camera1, &CameraUI::endPlay, this, &CamerasRporter::slotCamera1FinishPlay);
	connect(ui.camera2, &CameraUI::endPlay, this, &CamerasRporter::slotCamera2FinishPlay);
	connect(ui.camera1, &CameraUI::timeChanged, this, &CamerasRporter::slotCamera1PlayTime);
	connect(ui.camera2, &CameraUI::timeChanged, this, &CamerasRporter::slotCamera2PlayTime);
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
						m_mapDateTimeline.insert(rDate, TTimeLine());
				}
				else
				{
					//TODO: can't load path
				}
			}
		}

		// Update combo box
		ui.comboDate->clear();
		for (auto iter = m_mapDateTimeline.begin(); iter != m_mapDateTimeline.end(); ++iter)
			ui.comboDate->addItem(iter.key().toString("yyyy/MM/dd"));
		slotDateChanged(0);
	}
}

void CamerasRporter::slotDateChanged(int iIdx)
{
	if (iIdx < 0)
		return;

	auto iter = m_mapDateTimeline.begin() + iIdx;
	const QDate& rDate = iter.key();
	
	for (CameraUI* pCameraUI : m_aCameraUI)
	{
		if (pCameraUI->isEnabled())
		{
			//TODO: should merge the timeline of two camera
			TTimeLine mapList = pCameraUI->getTimeSet(rDate);
			iter.value() = mapList;
			ui.comboTime->clear();
			for (auto iter = mapList.begin(); iter != mapList.end(); ++iter)
			{
				ui.comboTime->addItem(iter.key().toString("HH:mm:ss") + " - " + iter->toString("HH:mm:ss"));
			}
		}
	}

	slotTimeChanged(0);
}

void CamerasRporter::slotTimeChanged(int iIdx)
{
	if (iIdx < 0)
		return;

	const TTimeLine& mapTime = (m_mapDateTimeline.begin() + ui.comboDate->currentIndex()).value();

	m_iterCurrentTimeLine = (mapTime.begin() + iIdx);
	ui.sliderTimeLine->setMaximum(m_iterCurrentTimeLine.key().msecsTo(m_iterCurrentTimeLine.value()));
	ui.sliderTimeLine->setValue(0);

	for (CameraUI* pCameraUI : m_aCameraUI)
	{
		pCameraUI->playTime(m_iterCurrentTimeLine.key());
	}
}

void CamerasRporter::slotSetTimeline(int iTime)
{
	//TODO: find a place to show time
	ui.labelInfo->setText(m_iterCurrentTimeLine.key().addMSecs(iTime).toString());

	for (CameraUI* pCameraUI : m_aCameraUI)
	{
		pCameraUI->playTime(m_iterCurrentTimeLine.key().addMSecs(iTime));
	}
}

void CamerasRporter::slotCamera1FinishPlay(QDateTime timeNext)
{
}

void CamerasRporter::slotCamera2FinishPlay(QDateTime timeNext)
{
}

void CamerasRporter::slotCamera1PlayTime(QDateTime timePlay)
{
	if (m_iterCurrentTimeLine != nullptr)
	{
		ui.sliderTimeLine->setValue(m_iterCurrentTimeLine.key().msecsTo(timePlay));
		ui.labelInfo->setText(timePlay.toString());
	}
}

void CamerasRporter::slotCamera2PlayTime(QDateTime timePlay)
{
}
