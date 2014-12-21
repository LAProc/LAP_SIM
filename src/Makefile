CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SIM_SOURCES= FMA.cpp Inv_Sqrt.cpp LAPU.cpp IO.cpp PE.cpp Reg_File.cpp Simulator.cpp
MAT_SOURCES=matrix_maker.cpp

SIM_OBJECTS=$(SIM_SOURCES:.cpp=.o)
MAT_OBJECTS=$(MAT_SOURCES:.cpp=.o)

SIM_EXECUTBALE=LAC_SIM
TESTER_EXEC=Matrix_Maker

all: $(SIM_EXECUTBALE) $(TESTER_EXEC)  

$(SIM_EXECUTBALE): $(SIM_OBJECTS)
	$(CC)  $(SIM_OBJECTS) $(LDFLAGS) -o $@

$(TESTER_EXEC): $(MAT_OBJECTS)
	$(CC) $(MAT_OBJECTS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f  *.o