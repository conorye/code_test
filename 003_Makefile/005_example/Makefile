objs = a.o b.o c.o

dep_files := $(patsubst %,.%.d, $(objs))
dep_files := $(wildcard $(dep_files))
CFLAGES= -Werror -Iinclude

test: $(objs)
	gcc $(CFLAGES) -o test $^
	@echo dep_files = $(dep_files)


ifneq ($(dep_files),)
include $(dep_files)
endif

%.o:%.c
	gcc -c -o $@ $< -MD -MF .$@.d 

distclean:
	rm $(dep_files)

clean:
	rm *.o test
.PHONY:clean