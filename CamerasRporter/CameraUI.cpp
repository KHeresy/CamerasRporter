#include "CameraUI.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

CameraUI::CameraUI(QWidget *parent) :QWidget(parent)
{
	ui.setupUi(this);

	m_pVideoItem = new QGraphicsVideoItem();
	ui.videoArea->setScene(new QGraphicsScene());
	ui.videoArea->scene()->addItem(m_pVideoItem);

	m_mPlayer.setVideoOutput(m_pVideoItem);
	m_mPlayer.setMuted(true);

	connect(&m_mPlayer, &QMediaPlayer::durationChanged, this, &CameraUI::durationChanged);
	connect(&m_mPlayer, &QMediaPlayer::mediaStatusChanged, this, &CameraUI::mediaStatusChanged);
	connect(&m_mPlayer, &QMediaPlayer::positionChanged, this, &CameraUI::positionChanged);
	connect(&m_mPlayer, &QMediaPlayer::stateChanged, this, &CameraUI::stateChanged);
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

		m_vDateTimeList.reserve(m_vFileList.size());
		for (auto& sName : m_vFileList)
		{
			QDateTime mDateTime = QDateTime::fromString(sName.mid(0, 15), "yyyyMMdd_HHmmss");
			m_vDateTimeList.push_back(mDateTime);
			m_setDate.insert(mDateTime.date());
		}
		return true;
	}
	return false;
}

void CameraUI::resizeEvent(QResizeEvent * pEvent)
{
	ui.videoArea->fitInView(m_pVideoItem, Qt::KeepAspectRatio);
}

void CameraUI::slotAudio(bool bClicked)
{
	m_mPlayer.setMuted(!bClicked);
}

void CameraUI::slotPlay(bool bClicked)
{
	if (bClicked)
	{
		m_mPlayer.play();
	}
	else
	{
		m_mPlayer.pause();
	}
}

void CameraUI::slotPrevious()
{
	m_mPlayer.pause();
	m_mPlayer.setPosition(m_mPlayer.position() - 100);
}

void CameraUI::slotNext()
{
	m_mPlayer.pause();
	m_mPlayer.setPosition(m_mPlayer.position() + 100);
}

void CameraUI::slotSetTime(int iTime)
{
	m_mPlayer.setPosition(iTime);
}

void CameraUI::slotSaveImage()
{
	QString sFilename = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG (*.png) ;; JPEG (*.jpg)"));
	if (sFilename != "")
	{
		QGraphicsScene* pScene = ui.videoArea->scene();
		pScene->clearSelection();
		pScene->setSceneRect(pScene->itemsBoundingRect());
		
		QImage imgOutput(m_pVideoItem->nativeSize().toSize(), QImage::Format_ARGB32);
		imgOutput.fill(Qt::transparent);

		QPainter qPainter(&imgOutput);
		pScene->render(&qPainter);

		imgOutput.save(sFilename);
	}
}

void CameraUI::durationChanged(qint64 duration)
{
	ui.sliderTime->setMaximum(duration);

	QTime mTime = QTime::fromMSecsSinceStartOfDay(duration);
	ui.labelTotalTime->setText(mTime.toString("mm:ss.zzz"));
}

void CameraUI::mediaStatusChanged(QMediaPlayer::MediaStatus eStatus)
{
	switch (eStatus)
	{
	case QMediaPlayer::BufferedMedia:
	case QMediaPlayer::LoadedMedia:
		ui.videoArea->fitInView(m_pVideoItem,Qt::KeepAspectRatio);
	}
}

void CameraUI::positionChanged(qint64 position)
{
	ui.sliderTime->setValue(position);

	QTime mTime = QTime::fromMSecsSinceStartOfDay(position);
	ui.labelCurrentTime->setText(mTime.toString("mm:ss.zzz"));
}

void CameraUI::stateChanged(QMediaPlayer::State eState)
{
	switch (eState)
	{
	case QMediaPlayer::PlayingState:
		ui.buttonPlay->setChecked(true);
		break;

	case QMediaPlayer::StoppedState:
	case QMediaPlayer::PausedState:
		ui.buttonPlay->setChecked(false);
		break;
	}
}
