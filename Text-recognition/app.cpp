#include "app.h"
#include <QDir>
#include <qstringlist.h>
#include <QTime>
#include <qglobal.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include "LetterAnalyzer.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

App::App(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	networkTrained = false;
}

App::~App()
{

}

void App::init()
{
	int amount = 0;
	ui.recognize->setEnabled(false);
	ui.saveWeights->setEnabled(false);
	connectEverything();
}

void App::connectEverything()
{
	connect(ui.loadImage, SIGNAL(clicked()), this, SLOT(getImagePath()));
	connect(ui.loadResult, SIGNAL(clicked()), this, SLOT(getResultPath()));
	connect(ui.loadWeights, SIGNAL(clicked()), this, SLOT(readWeights()));
	connect(ui.saveWeights, SIGNAL(clicked()), this, SLOT(saveWeights()));
	connect(ui.recognize, SIGNAL(clicked()), this, SLOT(recognize()));
	connect(ui.learn, SIGNAL(clicked()), this, SLOT(learn()));
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

void App::readAndPrepareTrainingSetsInfo()
{
	initTrainingLetters();
	QDir trainingDir("training-sets");
	trainingFilesList = trainingDir.entryList(QStringList("*.jpg"));
	randomizeTrainingSet();
	QImage img;
	QImage cropped;

	for (int i = 0; i < INPUT_DATA; i++)
	{
		img.load("training-sets/" + trainingFilesList[i]);
		cropped = LetterAnalyzer::crop(img);
		double* analyzed = LetterAnalyzer::parse(cropped);
		trainingSet.insertSet(i, analyzed);
	}
}

void App::testNetwork()
{
	QDir trainingDir("test-sets");
	testFilesList = trainingDir.entryList(QStringList("*.jpg"));
	QImage img;
	QImage cropped;

	for (int i = 0; i < TEST_DATA; i++)
	{
		img.load("test-sets/" + testFilesList[i]);
		cropped = LetterAnalyzer::crop(img);
		double* analyzed = LetterAnalyzer::parse(cropped);
		testResults[i] =  network.recognize(analyzed);
	}
}

Network* App::getNetwork()
{
	return &network;
}

void App::readWeights()
{
	QFile file("weights.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Blad", "Nie mozna odczytac pliku z wagami!");
		return;
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

	if (!ui.saveWeights->isEnabled())
		ui.saveWeights->setEnabled(true);

	networkTrained = true;
}

void App::saveWeights()
{
	QFile file("weights.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Blad", "Nie mozna zapisac pliku z wagami!");
		return;
	}

	int* neuronsInLayer = network.getNeuronsInLayer();

	for (int i = 0; i < LAYERS_NUM-1; i++)
		for (int j = 0; j < neuronsInLayer[i+1]; j++)
			for (int k = 0; k < neuronsInLayer[i]; k++)
			{
				file.write(QString::number(network.getWeights()[i][j][k], 'g', 14).toLocal8Bit());
				file.write("\n");
			}

	file.close();
}

void App::recognize()
{
	testNetwork();

	int amount = 0;
	Mat image = imread(imageFile.toStdString(), 0);
	int* recognizedLetters = imageParser.getContour(image, amount, &network);

	QFile file(resultFile);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Blad", "Nie mozna odczytac pliku wynikowego!");
		return;
	}

	for (int i = 0; i < amount; i++)
		file.write(QString(recognizedLetters[i] + 'A').toLocal8Bit());

	file.close();

	QMessageBox::information(this, "Info", "Rozpoznawanie zostalo zakonczone");
}

void App::learn()
{
	readAndPrepareTrainingSetsInfo();
	network.learn(INPUT_DATA, &trainingSet, trainingLetters);
	testNetwork();
	networkTrained = true;
	ui.saveWeights->setEnabled(true);
	QMessageBox::information(this, "Info", "Uczenie zostalo zakonczone");
}

void App::getImagePath()
{
	imageFile = QFileDialog::getOpenFileName(this, "Open file", "", "Image Files(*.jpg)");
	if (imageFile.isEmpty())
		return;
	ui.imagePath->setText(imageFile);
	if (!ui.recognize->isEnabled() && !resultFile.isEmpty() && networkTrained)
		ui.recognize->setEnabled(true);
}

void App::getResultPath()
{
	resultFile = QFileDialog::getOpenFileName(this, "Open file", "", "Text Files(*.txt)");
	if (resultFile.isEmpty())
		return;
	ui.resultPath->setText(resultFile);
	if (!ui.recognize->isEnabled() && !imageFile.isEmpty() && networkTrained)
		ui.recognize->setEnabled(true);
}