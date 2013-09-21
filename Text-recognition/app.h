#ifndef APP_H
#define APP_H

#include <QtWidgets/QMainWindow>
#include "ui_app.h"
#include "globals.h"
#include "Network.h"

class App : public QMainWindow
{
	Q_OBJECT

public:
	App(QWidget *parent = 0);
	~App();

	QStringList readAndPrepareTrainingSetsInfo();
	void initTrainingLetters();
	void randomizeTrainingSet();
	void readWeights();
	void saveWeights();

private:
	Ui::AppClass ui;
	Network network;
	QStringList trainingFilesList;
	QStringList testFilesList;
	int trainingLetters[INPUT_DATA];
};

#endif // APP_H
