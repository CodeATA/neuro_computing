from brian2 import *
#%matplotlib inline
#import matplotlib

synapse_num = 10
neuron_num = 5

# LIF Neuron Model
tau = 10*ms
eqs = '''
dv/dt = (v0 - v) / tau : volt
v0 : volt
'''
threshold = '''v > 1*mV'''
reset = '''v = 0*mV'''
refractory = 5*ms

G = NeuronGroup(neuron_num, eqs, threshold=threshold, reset=reset, method='exact')

# simple synapse model
s_model = '''w : volt'''
on_pre = '''v_post += w'''
on_post = ''' '''

S = Synapses(G, G, model=s_model, on_pre=on_pre, on_post=on_post)

# connect neurons and set weight for each synapse
pre = []
post = []
weight = []
with open("synapases", "r") as src:
  for line in src:
    split_line = line.split(',')
    pre.append(int(split_line[0]))
    post.append(int(split_line[1]))
    weight.append(float(split_line[2]))
  S.connect(i=pre, j=post)
  for i in range(0, synapse_num):
    S.w[pre[i], post[i]] = weight[i]*mV

# stimuli from outside
stimuli = PoissonGroup(neuron_num, np.arange(neuron_num)*Hz + 100*Hz)
sti_input = Synapses(stimuli, G, on_pre='v_post+=1*mV')
sti_input.connect(j='i')

# for recoding
spikemon = SpikeMonitor(G)

# start simulation
run(100*ms)

# for plot
#plot(spikemon.t/ms, spikemon.i, '.k')
#xlabel('Time (ms)')
#ylabel('Neuron index');

# print spike timing of each neuron
print(spikemon.all_values())