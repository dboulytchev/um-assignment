um: main.cpp
	g++ -o um main.cpp -std=c++11 -O3

.PHONY: clean

clean:
	rm um
