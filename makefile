all: prog lsh cube cluster clean

prog: main.cpp clustering.o hashfunc.o hashtable.o hcubemap.o list.o lsh.o pqueue.o vectorfile.o metric.o vector.o
	g++ -o prog main.cpp clustering.o hashfunc.o hashtable.o hcubemap.o list.o lsh.o pqueue.o vectorfile.o metric.o vector.o
lsh: lsh_main.cpp clustering.o hashfunc.o hashtable.o hcubemap.o list.o lsh.o pqueue.o vectorfile.o metric.o vector.o
	g++ -o lsh lsh_main.cpp clustering.o hashfunc.o hashtable.o hcubemap.o list.o lsh.o pqueue.o vectorfile.o metric.o vector.o
cube: cube_main.cpp clustering.o hashfunc.o hashtable.o hcubemap.o list.o lsh.o pqueue.o vectorfile.o metric.o vector.o
	g++ -o cube cube_main.cpp clustering.o hashfunc.o hashtable.o hcubemap.o list.o lsh.o pqueue.o vectorfile.o metric.o vector.o
cluster: cluster_main.cpp clustering.o hashfunc.o hashtable.o hcubemap.o list.o lsh.o pqueue.o vectorfile.o metric.o vector.o
	g++ -o cluster cluster_main.cpp clustering.o hashfunc.o hashtable.o hcubemap.o list.o lsh.o pqueue.o vectorfile.o metric.o vector.o


clean:
	rm *.o

clustering.o: dstructs/clustering.cpp dstructs/clustering.hpp
	g++ -c dstructs/clustering.cpp

hashfunc.o: dstructs/hashfunc.cpp dstructs/hashfunc.hpp
	g++ -c dstructs/hashfunc.cpp

hashtable.o: dstructs/hashtable.cpp dstructs/hashtable.hpp
	g++ -c dstructs/hashtable.cpp

hcubemap.o: dstructs/hcubemap.cpp dstructs/hcubemap.hpp dstructs/pointstruct.hpp
	g++ -c dstructs/hcubemap.cpp

list.o: dstructs/list.cpp dstructs/list.hpp
	g++ -c dstructs/list.cpp

lsh.o: dstructs/lsh.cpp dstructs/lsh.hpp dstructs/pointstruct.hpp
	g++ -c dstructs/lsh.cpp

pqueue.o: dstructs/pqueue.cpp dstructs/pqueue.hpp
	g++ -c dstructs/pqueue.cpp

vectorfile.o: fileparse/vectorfile.cpp fileparse/vectorfile.hpp
	g++ -c fileparse/vectorfile.cpp

metric.o: geometry/metric.cpp geometry/metric.hpp
	g++ -c geometry/metric.cpp

vector.o: geometry/vector.cpp geometry/vector.hpp
	g++ -c geometry/vector.cpp