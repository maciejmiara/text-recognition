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

	void init();
	void connectEverything();
	void readAndPrepareTrainingSetsInfo();
	void initTrainingLetters();
	void randomizeTrainingSet();
	void testNetwork();

private:
	Ui::AppClass ui;
	Network network;
	QStringList trainingFilesList;
	QStringList testFilesList;
	int trainingLetters[INPUT_DATA];
	bool networkTrained;
	QString imageFile;
	QString resultFile;
	TrainingSet trainingSet;
	int testResults[TEST_DATA];

public slots:
	void readWeights();
	void saveWeights();
	void recognize();
	void learn();
	void getImagePath();
	void getResultPath();
};

#endif // APP_H
