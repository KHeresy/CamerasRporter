#include "CameraUI.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

CameraUI::CameraUI(QWidget *parent) :QWidget(parent)
{
	ui.setupUi(this);

	m_iTimeScaleInUI = 100;

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
	if (iIdx >= 0 && iIdx < m_vFileList.size())
	{
		m_mPlayer.setMedia(QUrl::fromLocalFile(m_sPath + m_vFileList[iIdx].sFilename));
		m_mPlayer.play();
		m_mPlayer.pause();
	}
}

bool CameraUI::setPath(QString sPath)
{
	m_sPath = sPath + m_sName;
	QDir dirFolder = m_sPath;
	if (dirFolder.exists())
	{
		QStringList vFileList = dirFolder.entryList({ "*.mp4" }, QDir::Files, QDir::Name);

		m_vFileList.clear();
		m_vFileList.reserve(vFileList.size());

		for (auto& sName : vFileList)
		{
			QDateTime mDateTime = QDateTime::fromString(sName.mid(0, 15), "yyyyMMdd_HHmmss");
			m_vFileList.push_back({ sName, mDateTime, QTime() });
			m_setDate.insert(mDateTime.date());
		}

		ui.comboFileList->clear();
		ui.comboFileList->addItems(vFileList);
		return true;
	}
	return false;
}

void CameraUI::resizeEvent(QResizeEvent * pEvent)
{
	ui.videoArea->fitInView(m_pVideoItem, Qt::KeepAspectRatio);
}

QMap<QTime, QTime> CameraUI::getTimeSet(const QDate & mDate)
{
	static QTime timeZero(0, 0, 0, 0);
	QMap<QTime, QTime> mapTimeList;

	QPair<QTime, QTime> pTimeline;
	for (auto& rFile : m_vFileList)
	{
		if (rFile.timeBegin.date() == mDate)
		{
			if (!rFile.timeLength.isValid())
			{
				//TODO: load from file?
				rFile.timeLength = QTime(0, 1, 0, 0);
			}

			if (pTimeline.first.isValid())
			{
				if(pTimeline.second.secsTo(rFile.timeBegin.time()) > 20) //TODO: should make 20 as parameter
				{
					mapTimeList.insert(pTimeline.first, pTimeline.second);

					pTimeline.first = rFile.timeBegin.time();
					pTimeline.second = getTimeEnd(rFile);
				}
				else
				{
					pTimeline.second = getTimeEnd( rFile );
				}
			}
			else
			{
				pTimeline.first = rFile.timeBegin.time();
				pTimeline.second = getTimeEnd(rFile);
			}
		}
	}

	if(pTimeline.first.isValid())
		mapTimeList.insert(pTimeline.first, pTimeline.second);

	return mapTimeList;
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
	m_mPlayer.setPosition(m_mPlayer.position() - 5 * m_iFrameInterval);
}

void CameraUI::slotNext()
{
	m_mPlayer.setPosition(m_mPlayer.position() + 5 * m_iFrameInterval);
}

void CameraUI::slotSetTime(int iTime)
{
	m_mPlayer.setPosition(m_iTimeScaleInUI * iTime);
}

void CameraUI::slotSliderAction(int)
{
	slotSetTime( ui.sliderTime->value());
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
	ui.sliderTime->setMaximum(duration / m_iTimeScaleInUI);

	QTime mTime = QTime::fromMSecsSinceStartOfDay(duration);
	ui.labelTotalTime->setText(mTime.toString("mm:ss.zzz"));
}

void CameraUI::mediaStatusChanged(QMediaPlayer::MediaStatus eStatus)
{
	switch (eStatus)
	{
	case QMediaPlayer::BufferedMedia:
	case QMediaPlayer::LoadedMedia:
		m_iFrameInterval = 1000 / (int)m_mPlayer.metaData("VideoFrameRate").toReal();
		ui.videoArea->fitInView(m_pVideoItem,Qt::KeepAspectRatio);
	}
}

void CameraUI::positionChanged(qint64 position)
{
	ui.sliderTime->setValue(position / m_iTimeScaleInUI);

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
