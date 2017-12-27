import random
import time

synapse_num = int(input("input synapse number: "))
neuron_num = int(input("input neuron number: "))

synapse = set()

random.seed(time.time())

with open("synapases", "w") as out_file:
  #out_file.write("neuron: %d\nsynapse: %d\n" % (neuron_num, synapse_num))
  for i in range(0, synapse_num):
    repeat = True
    while (repeat):
      start = random.randint(0, neuron_num-1)
      end = random.randint(0, neuron_num-1)
      while (end == start):
        end = random.randint(0, neuron_num-1)
      if (start,end) not in synapse:
        repeat = False
        synapse.add((start,end))
    out_file.write("%d,%d,%f\n" % (start, end, random.random()))