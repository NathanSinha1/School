// Include everything necessary here 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    // <YOUR CODE: Handle the possible errors in input data given by the user and say how to execute the code>
    if (argc!= 6) { //checks if correct amount of inputs
        printf("Usage: %s <POPULATION_SIZE> <MAX_GENERATIONS> <crossover_rate> <mutate_rate> <stop_criteria>\n", argv[0]);
        return 1;
    }
    
    // <YOUR CODE: Assign all inputs given by the user argv[i]> like
    // POPULATION_SIZE, MAX_GENERATIONS, crossover_rate, mutate_rate, stop_criteria
    int POPULATION_SIZE = atoi(argv[1]);
    int MAX_GENERATIONS = atoi(argv[2]);
    double crossover_rate = atof(argv[3]);
    double mutate_rate = atof(argv[4]);
    double stop_criteria = atof(argv[5]);
    double bestFitness = 10000000.0;
    double oldBestFitness = 1000.0;
    int continueGeneration = 1;

    if (POPULATION_SIZE <= 0 || MAX_GENERATIONS <= 0 || crossover_rate < 0 || mutate_rate < 0 || stop_criteria < 0){ // checks if input is valid
        printf("Invalid input, please try again\n");
        return 1;
    }

    // ###################################################################################
    // you dont need to change anything here
    // the number of variables
    int NUM_VARIABLES = 2;
    // the lower bounds of variables
    double Lbound[] = {-5.0, -5.0};
    // the upper bounds of variable
    double Ubound[] = {5.0, 5.0};
    // ###################################################################################


    // <YOUR CODE: Here make all the initial print outs>
    printf("Genetic Algorithem is initialized\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("The number of variables is %d\n", NUM_VARIABLES);
    printf("Lower bounds: ");
    for (int i = 0; i < NUM_VARIABLES; i++) {
        printf("%f ", Lbound[i]);
    }
    printf("\n");
    printf("Upper bounds: ");
    for (int i = 0; i < NUM_VARIABLES; i++) {
        printf("%f ", Ubound[i]);
    }
    printf("\n\n");
    printf("Population Size: %d\n", POPULATION_SIZE);
    printf("Max Generations: %d\n", MAX_GENERATIONS);
    printf("Stopping Criteria: %.16f\n\n", stop_criteria);
    printf("Results\n");
    printf("-------------------------------------------------------------------------------\n");

    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    // <YOUR CODE: Declare all the arrays you need here>
    double fitness[POPULATION_SIZE];
    double new_population[POPULATION_SIZE][NUM_VARIABLES];
    double population[POPULATION_SIZE][NUM_VARIABLES];
    double bestSolution[NUM_VARIABLES];
    
    // <YOUR CODE: Call generate_population function to initialize the "population"> like:
    // generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);
    generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);

    // <YOUR CODE: Implement the logic of stopping criteria here>
    for (int generation = 0; generation < MAX_GENERATIONS && continueGeneration; generation++) {
        // <YOUR CODE: Compute the fitness values using objective function for
        // each row in "population" (each set of variables)> like:
        compute_objective_function(POPULATION_SIZE, NUM_VARIABLES, population, fitness);
        
        // <YOUR CODE: Here implement the logic of finding the best solution with minimum fitness value
        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (fitness[i] < bestFitness) { //compares previous best fitness with old best fitness
                oldBestFitness = bestFitness;
                bestFitness = fitness[i];
                for (int j = 0; j < NUM_VARIABLES; j++) { // gets variables of best fitness
                    bestSolution[j] = population[i][j];
                }
            }
        }

        // and the stopping criteria>
        if (fabs(bestFitness-oldBestFitness) < stop_criteria) {
            continueGeneration = 0;
        }
        
        // <YOUR CODE: Implement the logic of crossover here>
        crossover(POPULATION_SIZE, NUM_VARIABLES, fitness, new_population, population, crossover_rate);
        // <YOUR CODE: Implement the logic of mutation here>
        mutate(POPULATION_SIZE, NUM_VARIABLES, new_population, population, Lbound, Ubound, mutate_rate);

        // Now you have the new population, and it goes to the beginning of loop to re-compute all again
    }

    // <YOUR CODE: Jump to this part of code if the stopping criteria is met before MAX_GENERATIONS is met>

    // ###################################################################################
    // You dont need to change anything here
    // Here we print the CPU time taken for your code
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("CPU time: %f seconds\n", cpu_time_used);
    // ###################################################################################

    // <Here print out the best solution and objective function value for the best solution like the format>
    printf("Best solution found: ");
    for (int i = 0; i < NUM_VARIABLES; i++) {
        printf("%f ", bestSolution[i]);
    }
    printf("\n");
    printf("Best fitness found: %f\n", bestFitness);

    return 0;
}
