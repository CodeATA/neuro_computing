from brian2 import *
import matplotlib

# one type of synapse has only one delay

synapse_num = 10
neuron_num = 5

tau = 10*ms

eqs = '''
dv/dt = (v0 - v) / tau : volt (unless refractory)
v0 : volt
'''
threshold = '''v > 10*mV'''
reset = '''v = 0*mV'''
refractory = 5*ms

G = NeuronGroup(neuron_num, eqs, threshold=threshold, reset=reset, method='exact')

s_model = '''w : volt'''
on_pre = '''v_post += w'''
on_post = ''' '''

S = Synapses(G, G, model=s_model, on_pre=on_pre, on_post=on_post)

pre = []
post = []
weight = []
# unfinished
with open("synapases", "r") as src:
  for line in src:
    split_line = line.split(',')
    pre.append(int(split_line[0]))
    post.append(int(split_line[1]))
    weight.append(float(split_line[2]))
  print(pre)
  print(post)
  print(weight)
  S.connect(i=pre, j=post)
  for i in range(0, synapse_num):
    S.w[pre[i], post[i]] = weight[i]*mV

stimuli = PoissonGroup(neuron_num, np.arange(neuron_num)*Hz + 10*Hz)
sti_input = Synapses(stimuli, G, on_pre='v_post+=0.1*mV')
sti_input.connect(j='i')

# for recoding


# start simulation
run(100*ms)

# for plot