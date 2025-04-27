all:
	g++ -Wall -g RedBlackTree.cpp RedBlackTreeTests.cpp -o rbt-tests
clean:
	rm -rf rbt-tests
run:
	./rbt-tests
try: all run