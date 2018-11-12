#include <math.h>
#include <gsl/gsl_rng.h>
#include "anneal.hpp"

/*Return a number in the range [0,1] that is the probability of taking the described step.*/
static double getStepProbability(double oldEnergy, double newEnergy, double temperature)
{
    if(newEnergy < oldEnergy) {
        return 1.0;//If this step improves the cost, always take it.
    } else if(temperature == 0) {
        return 0.0;//Never take steps that don't improve the cost at temperature zero.
    } else {
        return exp((oldEnergy-newEnergy)/temperature);
    }
}



void anneal(void *alignment,
            anneal_params_t params,
            anneal_get_energy_t getEnergy,
            anneal_propose_step_t proposeStep,
            anneal_make_step_t makeStep,
            const gsl_rng *rng)
{
    double temperature = params.initialTemperature;
    double currentEnergy = getEnergy(alignment);
    double nextEnergy;
    
    while(temperature != 0) {
        if(temperature < params.minTemperature) temperature = 0;//Do one final run of pure hill-climbing.
        
        for(int i = 0; i < params.stepsPerTemperature; i++) {
            nextEnergy = proposeStep(alignment, rng);//Propose a step and get its cost.
            
            //Calculate the probability of taking the proposed step.
            double probability = getStepProbability(currentEnergy, nextEnergy, temperature);
            
            if(gsl_rng_uniform(rng) < probability) {
                makeStep(alignment);//Take the step.
            }
        }
        
        temperature *= params.coolingFactor;
    }
}
