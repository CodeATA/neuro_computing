import brian2
import matplotlib

# one type of synapse has only one delay

synapse_num = int(input("input synapse number: "))
neuron_num = int(input("input neuron number: "))

eqs = '''

'''
threshold = ''' '''
reset = ''' '''
refractory = 5*ms

G = NeuronGroup(neuron_num, eqs, threshold=threshold, reset=reset, metho='exact')

s_model = '''w : 1'''
on_pre = ''' '''
on_post = ''' '''

S = Synapses(G, G, model=s_model, on_pre=on_pre)

pre = []
post = []
weight = []
# unfinished
for i in range(0, synapse_num):
  pre.append()
  post.append()
  weight.append()
S.connect(i=pre, j=post)
for i in range(0, synapse_num):
  S.w[pre[i], post[i]] = weight[i]

stimuli = PoissonGroup(neuron_num, np.arange(100)*Hz + 10*Hz)
sti_input = Synapses(stimuli, G, on_pre='v_post+=0.1')
sti_input.connect(j='i')

# for recoding


# start simulation
run(100*ms)

# for plot