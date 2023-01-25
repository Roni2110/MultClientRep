compile:
	g++ -pthread -o server.out Server.cpp Server.h CLI.cpp CLI.h DistanceClass.cpp DistanceClass.h Knn.cpp Knn.h commands.h -std=c++11
	g++ -pthread -o client.out client.cpp CLI.cpp CLI.h DistanceClass.cpp DistanceClass.h Knn.cpp Knn.h commands.h -std=c++11