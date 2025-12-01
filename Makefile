CXX = g++
CXXFLAGS = -std=gnu++11 -Wall -Wextra -O2

OBJS = main.o InventorySystem.o

all: inventory_app

inventory_app: $(OBJS)
	$(CXX) $(CXXFLAGS) -o inventory_app $(OBJS)

main.o: main.cpp InventorySystem.h Product.h HashTable.h MyVector.h
	$(CXX) $(CXXFLAGS) -c main.cpp

InventorySystem.o: InventorySystem.cpp InventorySystem.h Product.h HashTable.h MyVector.h
	$(CXX) $(CXXFLAGS) -c InventorySystem.cpp

clean:
	rm -f *.o inventory_app
