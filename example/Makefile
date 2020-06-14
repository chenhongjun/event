cc := g++
execute := test
src := test1.cpp test2.cpp ../event/thread_pool.cpp
head := test2.h ../event/thread_pool.h ../event/event.h
lib := -lpthread

$(execute): $(src)
	$(cc) -o $(execute) $(src) $(lib)
