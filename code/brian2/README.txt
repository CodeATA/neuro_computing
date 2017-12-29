synapses_gen.py用来随机产生突触连接和连接的权重，写入到synapases文件中

sim.py是模拟器，使用的是LIF模型，运行需要安装brian2

brian2的安装：
pip install brian2
brian2还依靠Cython来把python代码转化成C++代码，安装方式：pip install cython