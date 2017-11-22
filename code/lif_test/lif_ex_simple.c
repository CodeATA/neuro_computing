#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

typedef struct neuron_LIF {
	// membrane voltage [mV]
	double V_membrane;
	
	// membrane resting voltage [mV]
	double V_rest;
	
	// membrane resistance [MOhm]
	double R_membrane;
	
	// 'fixed' computation parameter - time constant multiplier for
	// closed-form solution
	// exp(-(machine time step in ms)/(R * C)) [.]
	double exp_TC;
	
	// offset current [nA]
	double I_offset;
	
	// countdown to end of next refractory period [timesteps]
	int refract_timer;
	
	// post-spike reset membrane voltage [mV]
	double V_reset;
	
	// refractory time of neuron [timesteps]
	int T_refract;
} neuron_LIF;

uint64_t get_clock_time() {
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == 0)
		return (uint64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
	else
		return 0;
}

double threshold;

static inline void _lif_neuron_closed_form (
		neuron_LIF *neuron,
		double V_prev,
		double input_this_timestep) {
	
	double alpha = input_this_timestep * neuron->R_membrane + neuron->V_rest;
	neuron->V_membrane = alpha - (neuron->exp_TC * (alpha - V_prev));
}

double neuron_model_state_update(
		double exc_input,
		double inh_input,
		double external_bias,
		neuron_LIF *neuron) {
	
	if (neuron->refract_timer <= 0) {
		double input_this_timestep = exc_input - inh_input + external_bias + neuron->I_offset;
		_lif_neuron_closed_form(neuron, neuron->V_membrane, input_this_timestep);
	}
	else {
		neuron->refract_timer -= 1;
	}
	return neuron->V_membrane;
}

void neuron_model_has_spiked(neuron_LIF* neuron) {
	neuron->V_membrane = neuron->V_reset;
	neuron->refract_timer = neuron->T_refract;
}

int main(int argc, char **argv) {
	
	uint64_t begin_time_value, end_time_value;
	uint64_t time_diff;
	
	neuron_LIF neuron;
	neuron.V_membrane = 10;
	neuron.V_rest = 5;
	neuron.R_membrane = 3.1;
	neuron.exp_TC = 0.1;
	neuron.refract_timer = 0;
	neuron.V_reset = 5;
	neuron.T_refract = 20;
	neuron.I_offset = 2;	
	
	threshold = 15;
	
	int loop_times = 1000000;
	//printf("input loop times: ");
	//scanf("%d", &loop_times);
	loop_times = atoi(argv[1]);
	
	FILE *spike_file;
	spike_file = fopen("spikes", "w");
	
	int spike_num = 0;
	
	begin_time_value = get_clock_time();
	
	int i = 0;
	for (i; i < loop_times; i++) {
		double V_membrane = neuron_model_state_update(3, 1, 1, &neuron);
		//printf("V_membrane: %lf\n", V_membrane);
		if (V_membrane > threshold) {
			spike_num++;
			neuron_model_has_spiked(&neuron);
			fprintf(spike_file, "spike %d at i=%d\n", spike_num, i);
			//printf("spike %d at i=%d\n", spike_num, i);
		}
	}
	
	end_time_value = get_clock_time();
	time_diff = end_time_value - begin_time_value;
	printf("time: %ld microseconds\n", time_diff);
	fclose(spike_file);
	
	return 0;
}


