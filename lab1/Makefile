all: compile run exterminate

compile:
	@gcc -o project ./src/main.c ./src/encoder.c ./src/decoder.c ./src/matrixMan.c

run:
	@./project ./data/nt-P3.ppm

exterminate:
	@rm -rf project
