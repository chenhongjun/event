cc := g++
execute := test
src := boss_work.cpp ../event/thread_pool.cpp
head := ../event/thread_pool.h ../event/event.h
lib := -lpthread

$(execute): $(src)
	$(cc) -o $(execute) $(src) $(lib)
clean: 
	rm -rf test
