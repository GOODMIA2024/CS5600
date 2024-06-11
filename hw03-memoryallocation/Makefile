all: allocmem loadmem

allocmem: alloc-mem-tao.c
	gcc -o allocmem alloc-mem-tao.c

loadmem: load-mem-tao.c
	gcc -o loadmem load-mem-tao.c

clean:
	rm -f allocmem loadmem