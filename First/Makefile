CXX = g++
CXXFLAGS = -Wall -g
TARGET = main
OBJS = main.o first.o

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

main.o: main.cpp first.h
	$(CXX) $(CXXFLAGS) -c main.cpp

first.o: first.cpp first.h
	$(CXX) $(CXXFLAGS) -c first.cpp

clean:
	rm -f $(OBJS) $(TARGET)
