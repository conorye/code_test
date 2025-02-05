A = a b c
B=$(foreach f,$(A),$(f).o)

c= a b c d/

D = $(filter %/, $(c) )
E = $(filter-out %/, $(c))
F = $(wildcard *.c)
dep_files = $(patsubst %.c,%.d,$(F))


all:
	@echo $(B)
	@echo $(D)
	@echo $(E)
	@echo F=$(F)
	@echo dep_files=$(dep_files)