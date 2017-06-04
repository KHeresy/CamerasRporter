#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CamerasRporter.h"

class CamerasRporter : public QMainWindow
{
	Q_OBJECT

public:
	CamerasRporter(QWidget *parent = Q_NULLPTR);

private:
	Ui::CamerasRporterClass ui;
};
