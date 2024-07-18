default:
	gcc main.cpp -lglut -lGL -lGLU -lm -o main

build_and_run:
	gcc main.cpp -lglut -lGL -lGLU -lm -o main
	./main
