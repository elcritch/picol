picol: interp.c picol.h
	$(CC) interp.c -o $@

test: picol
	./picol test.pcl

examples: examples/command examples/hello
examples/command: examples/command.c picol.h
	$(CC) -I. examples/command.c -o $@
examples/hello: examples/hello.c picol.h
	$(CC) -I. examples/hello.c -o $@

examples-test: examples
	./examples/command
	./examples/hello

clean:
	rm picol picol.exe || true
	rm examples/command examples/command.exe || true
	rm examples/hello examples/hello.exe || true

.PHONY: clean examples examples-test test
