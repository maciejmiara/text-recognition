#include "app.h"
#include <QDir>
#include <qstringlist.h>
#include <QTime>
#include <qglobal.h>
#include <qfile.h>
#include <qtextstream.h>

App::App(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

App::~App()
{

}

void App::initTrainingLetters()
{
	for (int i = 0; i < INPUT_DATA; i++)
		trainingLetters[i] = i / INPUT_DATASETS;
}

void App::randomizeTrainingSet()
{
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());
	int replaceWith;
	int tmp;
	QString fileTmp;

	for (int i = 0; i < INPUT_DATA; i++)
	{
		replaceWith = qrand() % NEURONS_OUTPUT;

		tmp = trainingLetters[replaceWith];
		fileTmp = trainingFilesList[replaceWith];
		trainingLetters[replaceWith] = trainingLetters[i];
		trainingFilesList[replaceWith] = trainingFilesList[i];
		trainingLetters[i] = tmp;
		trainingFilesList[i] = fileTmp;
	}
}

QStringList App::readAndPrepareTrainingSetsInfo()
{
	initTrainingLetters();
	QDir trainingDir("training-sets");
	trainingFilesList = trainingDir.entryList(QStringList("*.jpg"));
	randomizeTrainingSet();
	return trainingFilesList;
}

void App::readWeights()
{
	QFile file("weights.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//handle error
	}

	int* neuronsInLayer = network.getNeuronsInLayer();

	QTextStream in(&file);
    for (int i = 0; i < LAYERS_NUM-1; i++)
		for (int j = 0; j < neuronsInLayer[i+1]; j++)
			for (int k = 0; k < neuronsInLayer[i]; k++)
			{
				QString line = in.readLine();
				network.getWeights()[i][j][k] = line.toDouble();
			}
}

void App::saveWeights()
{
	QFile file("weights.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//handle error
	}

	int* neuronsInLayer = network.getNeuronsInLayer();

	for (int i = 0; i < LAYERS_NUM-1; i++)
		for (int j = 0; j < neuronsInLayer[i+1]; j++)
			for (int k = 0; k < neuronsInLayer[i]; k++)
			{
				file.write(QString::number(network.getWeights()[i][j][k], 'g', 10).toLocal8Bit());
				file.write("\n");
			}

}