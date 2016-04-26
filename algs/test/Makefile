CPPFLAGS = -I ../src/include
vpath %.c ../src

all: test_dijkstra test_graph

#dijkstra: test_dijkstra.c dijkstra.c graph.c
#	$(CC) $(CPPFLAGS) -o $@ $^
#
#graph: test_graph.c graph.c
#	$(CC) $(CPPFLAGS) -o $@ $^
%: %.c dijkstra.c graph.c
	$(CC) $(CPPFLAGS) -o $@ $^

clean:
	rm -rf test_dijkstra
	rm -rf test_graph
