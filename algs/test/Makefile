CPPFLAGS = -I ../src/include
vpath %.c ../src

all: dijkstra

dijkstra: test_dijkstra.c dijkstra.c graph.c
	$(CC) $(CPPFLAGS) -o $@ $^

clean:
	rm -rf dijkstra
