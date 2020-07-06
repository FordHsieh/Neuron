EXE    = Neuron
CC     = g++
DEL    = rm -rf
#LIB    = -lm -lxml

all: $(EXE)

clean:
	$(DEL) $(EXE) *.o core

Neuron: BPN.o Neuron.o
	$(CC) -o Neuron BPN.o Neuron.o

BPN.o: BPN.h BPN.inl BPN.cpp
	$(CC) -c BPN.cpp

Neron.o: BPN.cpp  Neuron.cpp
	 $(CC) -c Neuron.cpp
