CFLAGS = -Wall -Os -g 

picol: interp.c picol.h vendor/regexp.o
	$(CC) vendor/regexp.o interp.c -o $@ $(CFLAGS)

test: picol
	./picol test.pcl

vendor/regexp.o: vendor/regexp.h vendor/regexp.c
	$(CC) -c vendor/regexp.c -o $@ $(CFLAGS)

examples: examples/command examples/hello examples/regexp-ext
examples/command: examples/command.c picol.h extensions/regexp-wrapper.h
	$(CC) -I. examples/command.c -o $@ $(CFLAGS)
examples/hello: examples/hello.c picol.h
	$(CC) -I. examples/hello.c -o $@ $(CFLAGS)
examples/regexp-ext: examples/regexp-ext.c vendor/regexp.o picol.h extensions/regexp-wrapper.h
	$(CC) -I. vendor/regexp.o examples/regexp-ext.c -o $@ $(CFLAGS)

examples-test: examples
	./examples/command
	./examples/hello
	./examples/regexp-ext

clean:
	-rm picol picol.exe interp.obj
	-rm examples/command examples/command.exe command.obj
	-rm examples/hello examples/hello.exe hello.obj
	-rm examples/regexp-ext examples/regexp-ext.exe vendor/regexp.o regexp.obj regexp-ext.obj

.PHONY: clean examples examples-test test
