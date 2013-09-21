#pragma once

const int MAXIMUM_LETTER_GRAYNESS = 180;
const int NEURONS_VERTICAL = 10;
const int NEURONS_HORIZONTAL = 8;
const int NEURONS_NUM = NEURONS_VERTICAL * NEURONS_HORIZONTAL;
const double MIN_INPUT = -0.5;
const double MAX_INPUT = 0.5;
const int NEURONS_INPUT = NEURONS_NUM;
const int NEURONS_HIDDEN = 40;
const int NEURONS_OUTPUT = 26;
const int MAX_LEARNING_EPOCHS = 1000;
const double QUIT_ERROR = 0.01;
const int INPUT_DATASETS = 15;
const int TEST_DATASETS = 5;
const int INPUT_DATA = NEURONS_OUTPUT*INPUT_DATASETS;
const int TEST_DATA = NEURONS_OUTPUT*TEST_DATASETS;
const int LAYERS_NUM = 3;
// wspó³czynnik aktywacji
const double BETA = 0.8;
// wspó³czynnik uczenia
const double ETA = 0.3;
// wspó³czynnik momentum
const double ALFA = 0.5;