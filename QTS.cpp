#include "QTS.h"
#include <iostream>
#include <random>
#include <iomanip>
#include <cstring>
#include <fstream>

using namespace std;

Solution population[POPULATION_SIZE]; // population pool
Solution bestSolution;
Solution worstSolution;
Solution finalBest;
double pMatrix[SEQUENCE_LENGTH]; // probability matrix
ofstream fileOut;

int myRandom(int, int); // return a random number and its range is [param 1, param 2]

void processOW(Solution *); // process overweight by drop an item

void init() {
    // initialize probability matrix
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        pMatrix[i] = 0.5;
    }
    bestSolution.fitness = 0;
    worstSolution.fitness = 620;
    finalBest.fitness = 0;
}

// measure solution's sequence
void measure() {
    double randomMatrix[SEQUENCE_LENGTH];

    for (int i = 0; i < POPULATION_SIZE; i++) {
        // generate a random matrix
        for (int j = 0; j < SEQUENCE_LENGTH; j++) {
            randomMatrix[j] = myRandom(0, 100) / 100.0;
        }
        // measure the value of each item (taken or not)
        for (int j = 0; j < SEQUENCE_LENGTH; j++) {
            if (pMatrix[j] > randomMatrix[j]) {
                population[i].sol[j] = 1;
            } else {
                population[i].sol[j] = 0;
            }
        }
    }
}

/**
 * Binary encoding:
 *
 * population[0] ~ population[9] stands for item 'A'
 * population[10] ~ population[19] stands for item 'B'
 *  .   .   .   .   .   .   .   .   .   .   .   .   .
 *  .   .   .   .   .   .   .   .   .   .   .   .   .
 *  .   .   .   .   .   .   .   .   .   .   .   .   .
 * population[90] ~ population[99] stands for item 'J'
 *
 */
void calcFitness() {
    // reset the best case and the worst case at this round
//    bestSolution.fitness = 0;
    worstSolution.fitness = 620;

    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i].weight = 0;
        population[i].value = 0;
        // calculate its weight and value
        for (int j = 0; j < SEQUENCE_LENGTH; j++) {
            if (j < 10) {
                population[i].weight += population[i].sol[j] * weight[0];
                population[i].value += population[i].sol[j] * value[0];
            } else if (j < 20) {
                population[i].weight += population[i].sol[j] * weight[1];
                population[i].value += population[i].sol[j] * value[1];
            } else if (j < 30) {
                population[i].weight += population[i].sol[j] * weight[2];
                population[i].value += population[i].sol[j] * value[2];
            } else if (j < 40) {
                population[i].weight += population[i].sol[j] * weight[3];
                population[i].value += population[i].sol[j] * value[3];
            } else if (j < 50) {
                population[i].weight += population[i].sol[j] * weight[4];
                population[i].value += population[i].sol[j] * value[4];
            } else if (j < 60) {
                population[i].weight += population[i].sol[j] * weight[5];
                population[i].value += population[i].sol[j] * value[5];
            } else if (j < 70) {
                population[i].weight += population[i].sol[j] * weight[6];
                population[i].value += population[i].sol[j] * value[6];
            } else if (j < 80) {
                population[i].weight += population[i].sol[j] * weight[7];
                population[i].value += population[i].sol[j] * value[7];
            } else if (j < 90) {
                population[i].weight += population[i].sol[j] * weight[8];
                population[i].value += population[i].sol[j] * value[8];
            } else {
                population[i].weight += population[i].sol[j] * weight[9];
                population[i].value += population[i].sol[j] * value[9];
            }
        }

        // calculate its fitness
        if (population[i].weight <= KNAPSACK_SIZE) {
            population[i].fitness = population[i].value;
        } else {
            // it gotta drop an item which is the lowest cost-performance ratio one if it is overweight
            processOW(&population[i]);
        }

        // check if the best and the worst solution changed
        if (population[i].fitness > bestSolution.fitness)
            memcpy(&bestSolution, &population[i], sizeof(population[i]));
        if (population[i].fitness < worstSolution.fitness)
            memcpy(&worstSolution, &population[i], sizeof(Solution));
        if (bestSolution.fitness > finalBest.fitness)
            memcpy(&finalBest, &bestSolution, sizeof(bestSolution));
    }
}

void processOW(Solution *p) {
    // drop an item which is the lowest cost-performance ratio one
    for (int i = SEQUENCE_LENGTH - 1; i >= 0; i--) {
        if (p->sol[i] == 1) {
            p->sol[i] = 0;

            if (i >= 90) {
                p->weight -= weight[9];
                p->value -= value[9];
            } else if (i >= 80) {
                p->weight -= weight[8];
                p->value -= value[8];
            } else if (i >= 70) {
                p->weight -= weight[7];
                p->value -= value[7];
            } else if (i >= 60) {
                p->weight -= weight[6];
                p->value -= value[6];
            } else if (i >= 50) {
                p->weight -= weight[5];
                p->value -= value[5];
            } else if (i >= 40) {
                p->weight -= weight[4];
                p->value -= value[4];
            } else if (i >= 30) {
                p->weight -= weight[3];
                p->value -= value[3];
            } else if (i >= 20) {
                p->weight -= weight[2];
                p->value -= value[2];
            } else if (i >= 10) {
                p->weight -= weight[1];
                p->value -= value[1];
            } else {
                p->weight -= weight[0];
                p->value -= value[0];
            }

            if (p->weight <= KNAPSACK_SIZE) {
                p->fitness = p->value;
                break;
            }
        }
    }
}

// update pMatrix's probability, and its probability belongs to [0.1, 0.9]
void update() {
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        if (bestSolution.sol[i] == 1 && worstSolution.sol[i] == 0) {
            if (pMatrix[i] < 0.9)
                pMatrix[i] += 0.1;
        } else if (bestSolution.sol[i] == 0 && worstSolution.sol[i] == 1) {
            if (pMatrix[i] > 0.1)
                pMatrix[i] -= 0.1;
        }
    }
}

// Print the number of items taken
void showResult() {
    cout << "========== Result at this round ==========" << endl;
    cout << "Best case: " << endl;

    int count[10];
    for (int i = 0; i < 10; i++) {
        count[i] = 0;
    }

    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        cout << bestSolution.sol[i] << '\t';
        if ((i + 1) % 10 == 0)
            cout << endl;

        if (bestSolution.sol[i] == 1) {
            if (i < 10) {
                count[0]++;
            } else if (i < 20) {
                count[1]++;
            } else if (i < 30) {
                count[2]++;
            } else if (i < 40) {
                count[3]++;
            } else if (i < 50) {
                count[4]++;
            } else if (i < 60) {
                count[5]++;
            } else if (i < 70) {
                count[6]++;
            } else if (i < 80) {
                count[7]++;
            } else if (i < 90) {
                count[8]++;
            } else {
                count[9]++;
            }
        }
    }

    // See how many each item is taken
    for (int i = 0; i < 10; i++) {
        cout << name[i] << ": " << count[i] << "  ";
    }
    cout << endl;

    cout << "bestSolution weight: " << bestSolution.weight
         << "\t value: " << bestSolution.value
         << "\t fitness: " << bestSolution.fitness << endl;
}

void fileOpen() {
    fileOut.open("../output/result.epin", ios::out);
    fileOut << "Particle : " << endl;
}

void outputEPIN(int round) {
    fileOut << '*' << (round + 1) << ' ';
    fileOut << bestSolution.fitness << ": ";
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fileOut << population[i].fitness << ' ';
        fileOut << "Bag" << (i + 1) << ' ';
        for (int j = 0; j < SEQUENCE_LENGTH; j++) {
            fileOut << population[i].sol[j] << ',';
            fileOut << fixed << setprecision(1) << pMatrix[j] << ',';
            fileOut << "bit" << (j + 1);
            if (j == SEQUENCE_LENGTH - 1)
                fileOut << '/';
            else
                fileOut << ' ';
        }
    }
    fileOut << endl;
}

void fileClose() {
    fileOut.close();
}

void prtPMatrix() {
    cout << "========== Probability Matrix ==========" << endl;
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        cout << fixed << setprecision(2) << setw(5) << pMatrix[i] << '\t';
        if ((i + 1) % 10 == 0)
            cout << endl;
    }
}

Solution *getFinalBest() {
    return &finalBest;
}

// return a random number and its range is [start, end]
int myRandom(int start, int end) {
    random_device rd;
    static default_random_engine gen = default_random_engine(rd());
    uniform_int_distribution<int> dis(start, end);
    return dis(gen);
}
