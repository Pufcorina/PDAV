all: compile runP3 exterminate

compile:
	@gcc -o project ./src/main.c ./src/encoder.c ./src/decoder.c ./src/matrixMan.c

runP3:
	@./project ./data/nt-P3.ppm ./output/finalP3.ppm

runP6:
		@./project ./data/nt-P6.ppm ./output/finalP6.ppm > /dev/null

exterminate:
	@rm -rf project
