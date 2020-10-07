#ifndef QTS_KNAPSACK_QTS_H
#define QTS_KNAPSACK_QTS_H

#define ROUND 3000 // number of how many rounds to execute QTS
#define POPULATION_SIZE 20 // number of population
#define KNAPSACK_SIZE 275 // number of max knapsack size
#define SEQUENCE_LENGTH 100 // length of solution sequence

/**
 * DEBUG_MODE 1: print debugging information
 *            0: do not print debugging information
 *
 * EACH_ROUND_RESULT 1: print each round result respectively
 *                   0: do not print each round result respectively
 */
#define DEBUG_MODE 0
#define EACH_ROUND_RESULT 0
#define EPIN_OUTPUT 0

typedef struct Solution {
    int sol[SEQUENCE_LENGTH];
    int weight;
    int value;
    int fitness;
} Solution;

const char name[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
const int weight[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int value[10] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

void init();

void measure();

void calcFitness();

void update();

void fileOpen();

void outputEPIN(int);

void fileClose();

void prtPMatrix();

void showResult(); // print result of current statistical round

Solution *getFinalBest();

#endif //QTS_KNAPSACK_QTS_H
