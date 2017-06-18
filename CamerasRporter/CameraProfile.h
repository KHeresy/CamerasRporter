#pragma once

#include <vector>

#include <QStringList>
#include <QTime>

class CameraProfile
{
public:
	QString		m_sName;
	QStringList	m_aCameraFolder;
	QString		m_sViedoFilenameFilter;
	QString		m_sFilenameToDateTime;
	QTime		m_tVideoLength;
};
