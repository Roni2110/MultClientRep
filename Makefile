compile:
	g++ -o server.out server.cpp server.h CLI.cpp CLI.h DistanceClass.cpp DistanceClass.h Knn.cpp Knn.h commands.h -std=c++11
	g++ -o client.out client.cpp CLI.cpp CLI.h DistanceClass.cpp DistanceClass.h Knn.cpp Knn.h commands.h -std=c++11