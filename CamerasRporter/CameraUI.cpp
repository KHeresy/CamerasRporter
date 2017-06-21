#include "CameraUI.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

CameraUI::CameraUI(QWidget *parent) :QWidget(parent)
{
	ui.setupUi(this);

	m_iTimeScaleInUI = 100;
	m_iCurrentIndx = -1;

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
	m_iCurrentIndx = iIdx;
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
			m_vFileList.push_back({ sName, mDateTime, QDateTime() });
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

QMap<QDateTime, QDateTime> CameraUI::getTimeSet(const QDate & mDate)
{
	QMap<QDateTime, QDateTime> mapTimeList;

	QPair<QDateTime, QDateTime> pTimeline;
	for (auto& rFile : m_vFileList)
	{
		if (rFile.timeBegin.date() == mDate)
		{
			if (!rFile.timeEnd.isValid())
			{
				//TODO: load from file?
				rFile.timeEnd = rFile.timeBegin.addSecs(60);
			}

			if (pTimeline.first.isValid())
			{
				if(pTimeline.second.secsTo(rFile.timeBegin) > 20) //TODO: should make 20 as parameter
				{
					mapTimeList.insert(pTimeline.first, pTimeline.second);

					pTimeline.first = rFile.timeBegin;
					pTimeline.second = rFile.timeEnd;
				}
				else
				{
					pTimeline.second = rFile.timeEnd;
				}
			}
			else
			{
				pTimeline.first = rFile.timeBegin;
				pTimeline.second = rFile.timeEnd;
			}
		}
	}

	if(pTimeline.first.isValid())
		mapTimeList.insert(pTimeline.first, pTimeline.second);

	return mapTimeList;
}

bool CameraUI::playTime(const QDateTime & timeToPlay)
{
	if (m_iCurrentIndx < 0)
		m_iCurrentIndx = m_vFileList.size() / 2;

	if (m_vFileList[m_iCurrentIndx].timeBegin > timeToPlay)
	{
		for (int i = m_iCurrentIndx; i >= 0; --i)
		{
			if (m_vFileList[i].timeEnd < timeToPlay)
				break;

			if (m_vFileList[i].timeBegin <= timeToPlay)
			{
				m_iCurrentIndx = i;
				m_mPlayer.setMedia(QUrl::fromLocalFile(m_sPath + m_vFileList[i].sFilename));
				m_mPlayer.play();
				m_mPlayer.setPosition(m_vFileList[i].timeBegin.msecsTo(timeToPlay));
				return true;
			}
		}
	}
	else
	{
		if (m_vFileList[m_iCurrentIndx].timeEnd > timeToPlay)
		{
			m_mPlayer.setPosition(m_vFileList[m_iCurrentIndx].timeBegin.msecsTo(timeToPlay));
			return true;
		}
		else
		{
			for (int i = m_iCurrentIndx; i < m_vFileList.size(); ++i)
			{
				if (m_vFileList[i].timeBegin > timeToPlay)
					break;

				if (m_vFileList[i].timeEnd > timeToPlay)
				{
					m_iCurrentIndx = i;
					m_mPlayer.setMedia(QUrl::fromLocalFile(m_sPath + m_vFileList[i].sFilename));
					m_mPlayer.play();
					m_mPlayer.setPosition(m_vFileList[i].timeBegin.msecsTo(timeToPlay));
					return true;
				}
			}
		}
	}

	//TODO: may use binary search
	for (int i = 0; i < m_vFileList.size(); ++i)
	{
		if (m_vFileList[i].timeBegin <= timeToPlay && m_vFileList[i].timeEnd > timeToPlay)
		{
			m_iCurrentIndx = i;
			m_mPlayer.setMedia(QUrl::fromLocalFile(m_sPath + m_vFileList[i].sFilename));
			m_mPlayer.play();
			m_mPlayer.setPosition(m_vFileList[i].timeBegin.msecsTo(timeToPlay));
			return true;
		}
	}

	m_iCurrentIndx = -1;
	return false;
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
	if (m_iCurrentIndx >= 0)
	{
		ui.sliderTime->setValue(position / m_iTimeScaleInUI);

		emit timeChanged(m_vFileList[m_iCurrentIndx].timeBegin.addMSecs(position));

		QTime mTime = QTime::fromMSecsSinceStartOfDay(position);
		ui.labelCurrentTime->setText(mTime.toString("mm:ss.zzz"));
	}
}

void CameraUI::stateChanged(QMediaPlayer::State eState)
{
	switch (eState)
	{
	case QMediaPlayer::PlayingState:
		ui.buttonPlay->setChecked(true);
		if (m_iCurrentIndx >= 0 )
			emit beginPlay(m_vFileList[m_iCurrentIndx].timeBegin);
		break;

	case QMediaPlayer::PausedState:
		ui.buttonPlay->setChecked(false);
		break;

	case QMediaPlayer::StoppedState:
		ui.buttonPlay->setChecked(false);
		if (m_iCurrentIndx < m_vFileList.size() - 1)
			emit endPlay(m_vFileList[m_iCurrentIndx+1].timeBegin);
		break;
	}
}
