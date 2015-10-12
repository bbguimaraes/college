all: nqueens/NQueens.class

.PHONY: clean
clean:
	rm -f nqueens/*.class

%.class: %.java
	javac $<

nqueens/NQueens.class: nqueens/NQueens.java
nqueens/NQueens.java: \
	nqueens/Solver.java nqueens/Solver.class \
	nqueens/Window.java nqueens/Window.class
nqueens/Solver.java: nqueens/Window.java nqueens/Window.class
