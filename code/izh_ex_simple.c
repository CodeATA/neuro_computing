#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

typedef struct neuron_IZH {
	double A;
	double B;
	double C;
	double D;
	
	double V;
	double U;
	
	double I_offset;
	
	double this_h;
} neuron_IZH;

uint64_t get_clock_time() {
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == 0)
		return (uint64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
	else
		return 0;
}

double threshold;

static inline void _rk2_kernel_midpoint (
		neuron_IZH *neuron,
		double h,
		double input_this_timestep) {
	
	double lastV1 = neuron->V;
	double lastU1 = neuron->U;
	double a = neuron->A;
	double b = neuron->B;

	double pre_alph = 140.0 + input_this_timestep - lastU1;
	double alpha = pre_alph
                 + ( 5.0 + 0.0400 * lastV1) * lastV1;
	double eta = lastV1 + 0.5000 * h * alpha;

    // could be represented as a long fract?
	double beta = 0.5000 * (h * (b * lastV1 - lastU1) * a);

    neuron->V += h * (pre_alph - beta
                      + ( 5.0 + 0.0400 * eta) * eta);

    neuron->U += a * h * (-lastU1 - beta + b * eta);
}

double neuron_model_state_update(
		double exc_input,
		double inh_input,
		double external_bias,
		neuron_IZH *neuron) {
	
	
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


