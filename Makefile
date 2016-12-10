# compiles the solving program and minisat
main: solve.cpp
			 g++ -std=c++11 -o solve solve.cpp
			 cd minisat/ && export MROOT='pwd'
			 cd minisat/core	&& make

# removes both the remaining *.cnf and *.out files (if any).
# the solver executable is also removed
clean:
			rm -f solve *.cnf *.out sat.log output.txt
