CC = g++
CFLAGS = -c
CXXFLAGS = -Wall -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SOURCES = test.cpp GameObjects/Bullet.cpp GameObjects/BulletManager.cpp GameObjects/Alien.cpp GameObjects/AlienManager.cpp GameObjects/CollisionManager.cpp GameObjects/GameObject.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = $(wildcard GameObjects/*.hpp)

all: test

test: $(OBJECTS)
	$(CC) $(OBJECTS) -o test $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(CXXFLAGS) $< -o $@

clean:
	rm -rf *.o test
