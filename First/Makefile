CXX = g++
CXXFLAGS = -Wall -g
TARGET = main
OBJS = main.o first.o linkList.o

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

main.o: main.cpp first.h linkList.h
	$(CXX) $(CXXFLAGS) -c main.cpp

first.o: first.cpp first.h
	$(CXX) $(CXXFLAGS) -c first.cpp

linkList.o: linkList.cpp linkList.h
	$(CXX) $(CXXFLAGS) -c linkList.cpp

clean:
	rm -f $(OBJS) $(TARGET)
