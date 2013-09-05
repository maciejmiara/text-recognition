#pragma once

const int MAXIMUM_LETTER_GRAYNESS = 150;
const int NEURONS_VERTICAL = 15;
const int NEURONS_HORIZONTAL = 15;
const int NEURONS_NUM = NEURONS_VERTICAL * NEURONS_HORIZONTAL;
const double MIN_INPUT = -0.5;
const double MAX_INPUT = 0.5;
const int NEURONS_INPUT = NEURONS_NUM;
const int NEURONS_OUTPUT = 26;
const double LEARN_RATE = 0.3;
const int MAX_LEARNING_ITERATIONS = 10000;
const double QUIT_ERROR = 0.1;
const double REDUCTION = 0.99;
const int INPUT_DATA = NEURONS_OUTPUT;