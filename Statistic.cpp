#include "Statistic.h"
#include "QTS.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

int cntOverweight = 0; // count overweight times
int totalValue = 0;
int bestValue = 0;
int bestWeight = 0;
int bestFitness = 0;
int bestSequence[100];
int cntBestCase = 0;


bool isOverweight(int w) {
    return w > KNAPSACK_SIZE;
}

// statistic of result of total rounds
void statistic() {
    Solution *best;
    best = getFinalBest();

    // check if it is overweight
    if (isOverweight(best->weight)) {
        cntOverweight++;
#if DEBUG_MODE
        cout << "Overweight!!!" << endl;
#endif
    } else if (best->fitness == 620) {
        cntBestCase++;
    }

    totalValue += best->value;

    // check if it needs to update best fitness
    if (best->fitness > bestFitness) {
        bestWeight = best->weight;
        bestValue = best->value;
        bestFitness = best->fitness;
        memcpy(bestSequence, best->sol, sizeof(bestSequence));
    }
}

void prtLine() {
    for (int i = 0; i < 78; i++) {
        if (i % 11 == 0)
            cout << '+';
        else
            cout << '-';
    }
    cout << endl;
}

void finalResult() {
    cout << "==================== STATISTIC ====================\n";

    cout << "Statistic round: " << STATISTICAL_ROUND << endl;
    cout << "Round: " << ROUND << endl;
    cout << "Population: " << POPULATION_SIZE << endl;

    prtLine();

    cout << '|'
         << setw(10) << "Overweight" << '|'     // col. 1
         << setw(10) << "Overweight" << '|'     // col. 2
         << setw(10) << "Best case" << '|'      // col. 3
         << setw(10) << "Avg.   " << '|'        // col. 4
         << setw(10) << "Best   " << '|'        // col. 5
         << setw(10) << "Best   " << '|'        // col. 6
         << setw(10) << "Best case" << '|' << endl; // col. 7

    cout << '|'
         << setw(10) << "count  " << '|'        // col. 1
         << setw(10) << "rate   " << '|'        // col. 2
         << setw(10) << "weight  " << '|'       // col. 3
         << setw(10) << "value  " << '|'        // col. 4
         << setw(10) << "value  " << '|'        // col. 5
         << setw(10) << "fitness " << '|'       // col. 6
         << setw(10) << "count  " << '|' << endl; // col. 7

    prtLine();

    cout << '|'
         << setw(9) << cntOverweight << " |"    // col. 1
         << setw(7) << fixed << setprecision(2) << cntOverweight / (double) STATISTICAL_ROUND * 100  // col. 2
         << setw(2) << " %" << " |"
         << setw(9) << bestWeight << " |"       // col. 3
         << setw(9) << setprecision(2) << totalValue / (double) STATISTICAL_ROUND << " |"    // col. 4
         << setw(9) << bestValue << " |"        // col. 5
         << setw(9) << bestFitness << " |"      // col. 6
         << setw(9) << cntBestCase << " |" << endl; // col. 7

    prtLine();


    int count[10];
    for (int i = 0; i < 10; i++) {
        count[i] = 0;
    }

    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        if (bestSequence[i] == 1) {
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

    for (int i = 0; i < 10; i++) {
        cout << name[i] << ": " << count[i] << "  ";
    }
    cout << endl;
}