CPPFLAGS = -I ../src/include
vpath %.c ../src

all: dijkstra

dijkstra: test_dijkstra.c dijkstra.c
	$(CC) $(CPPFLAGS) -o $@ $^
