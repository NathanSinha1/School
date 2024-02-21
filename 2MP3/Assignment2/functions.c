// Include everything necessary here 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "functions.h"

double generate_random(double min, double max)
{
    // implement a function to return a value between min and max
    double rand_num = (double)rand() / (double)RAND_MAX;
    return min + rand_num * (max - min);
}

unsigned int generate_int()
{
    // implement the function to return a random integer value
    return (unsigned int)rand();
}

// Function to initialize a random population
void generate_population(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES])
{
    // randomly initialize for all values in "population[i][j]""
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            population[i][j] = generate_random(Lbound[j], Ubound[j]);
        }
    }
}

// Function to compute the objective function for each member of the population
void compute_objective_function(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double fitness[POPULATION_SIZE])
{
    /* compute "fitness[i]"" for each set of decision variables (individual) or each row in "population"
    by calling "Objective_function" */
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
    }
}

void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate)
{
    /* Implement the logic of crossover function here based on "fitness_probs" or each set
    of decision variables (individual) or each row in "population".
    And save the new population in "new_population"*/
    double fitnessProb[POPULATION_SIZE], cumulativeProb[POPULATION_SIZE];
    double fitnessSum = 0.0;
    int index[POPULATION_SIZE];

    for (int i = 0; i < POPULATION_SIZE; i++) { // Calculate fitness probability of each individual
        fitnessProb[i] = 1 / (fitness[i] + 0.000001);
        fitnessSum += fitnessProb[i];
    }
    for (int i = 0; i < POPULATION_SIZE; i++) { // Normalize the probabilities
        fitnessProb[i] /= fitnessSum;
    }

    cumulativeProb[0]=fitnessProb[0];
    for (int i = 1; i < POPULATION_SIZE; i++) {  //cumulates the normalized probability
        cumulativeProb[i] = cumulativeProb[i - 1] + fitnessProb[i];
    }

    for (int i = 0; i < POPULATION_SIZE; i++) { //picks indices which live and die
        double random_num = generate_random(0.0, 1.0);
        if (cumulativeProb[i]<random_num && random_num<cumulativeProb[i+1]) {
            index[i] = 0;
        }
        else {
            index[i] = 1;
        }
    }

    for (int i = 0; i < POPULATION_SIZE; i++) { //change population based on who lives and dies
        for (int j = 0; j < NUM_VARIABLES; j++) {
            new_population[i][j] = population[index[i]][j];
        }
    }

    for (int i = 0; i < POPULATION_SIZE; i+=2) {
        if (generate_random(0.0, 1.0) < crossover_rate) {
            double parent1[NUM_VARIABLES], parent2[NUM_VARIABLES];
            double child1[NUM_VARIABLES], child2[NUM_VARIABLES];

            double crosspointTemp = generate_random(1.0, NUM_VARIABLES); // generate crosspoint
            int crosspoint = (int)crosspointTemp;

            for (int j = 0; j < NUM_VARIABLES; j++) { // Select parents
                parent1[j] = population[i][j];
                parent2[j] = population[i + 1][j];
            }

            for (int j = 0; j < crosspoint; j++) { // Cross genes of parent
                child1[j] = parent1[j];
                child2[j] = parent2[j];
            }
            for (int j = crosspoint; j < NUM_VARIABLES; j++) {
                child1[j] = parent2[j];
                child2[j] = parent1[j];
            }

            for (int j = 0; j < NUM_VARIABLES; j++) { // Set new population
                new_population[i][j] = child1[j];
                new_population[i + 1][j] = child2[j];
            }
        }
    }

    for (int i = 0; i < POPULATION_SIZE; i++){ // set new population back to population
        for (int j = 0; j < NUM_VARIABLES; j++) {
            population[i][j] = new_population[i][j];
        }
    }
}

void mutate(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double new_population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES], double mutate_rate)
{
    /*Implement the logic of mutation on "new_population" and then copy everything into "population"*/
    int total_gen = POPULATION_SIZE * NUM_VARIABLES;
    int total_gen_mutate = round(total_gen * mutate_rate); //how much to mutate
    int genes_to_mutate_indices[total_gen_mutate];

    for (int i = 0; i < total_gen_mutate; i++) { // pick indices to mutate
        do { //prevent same index from being selected twice
            genes_to_mutate_indices[i] = round(generate_random(0, total_gen));
        } while (i > 0 && genes_to_mutate_indices[i] == genes_to_mutate_indices[i-1]);
    }
     
    for (int i = 0; i < total_gen_mutate; i++) { // mutates selected genes
        int row = genes_to_mutate_indices[i] / NUM_VARIABLES; //selects what row
        int col = genes_to_mutate_indices[i] % NUM_VARIABLES; //selects what colm
        new_population[row][col] = generate_random(Lbound[col], Ubound[col]); // randomly changes the variable of the row and col selected
    }
    
    for (int i = 0; i < POPULATION_SIZE; i++) { // set new population back to population
        for (int j = 0; j < NUM_VARIABLES; j++) {
            population[i][j] = new_population[i][j];
        }
    }
}