graphs/graph.jpg: graph_data.txt graphs/graph.dot  
	dot -Tjpg graphs/graph.dot -o graphs/graph.jpg
graphs/graph.dot: thesis graph_data.txt
	./thesis graph_data.txt
thesis: main.cpp headers/ac.hpp headers/bk.hpp headers/mce_setup.hpp headers/rc.hpp
	g++ main.cpp -o thesis
clean :
	rm thesis graphs/* output/*