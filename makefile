default:
	gcc source/main.cpp -lglut -lGL -lGLU -lm -o main

build_and_run:
	gcc source/main.cpp -lglut -lGL -lGLU -lm -o main
	./main
