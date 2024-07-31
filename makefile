default:
	gcc source/main.cpp -lglut -lGL -lGLU -lm -lstdc++ -o main

build_and_run:
	gcc source/main.cpp -lglut -lGL -lGLU -lm -lstdc++ -o main
	./main
