# Compiler
CC = g++
CFLAGS = -fopenmp -O2 -pg

# Executable name
EXE = integration

# Source file
SRC = integration.cpp

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC)

# Run profiling and generate report
profile: $(EXE)
	./$(EXE)
	gprof $(EXE) gmon.out > profile_report.txt

# Clean up generated files
clean:
	rm -f $(EXE) gmon.out profile_report.txt

# Run the program
run: $(EXE)
	./$(EXE)
