FLAGS = -pthread
OFLAGS = -g

XNOIRDC_CPP_SRCS = $(shell find src/ -name '*.cpp')
XNOIRDC_CPP_OBJS = $(patsubst src/%.cpp,build/%.o,$(XNOIRDC_CPP_SRCS))

XNOIRDC_OBJS = $(XNOIRDC_CPP_OBJS)

.PHONY: clean

build/xnoircd: $(XNOIRDC_OBJS)
	g++ $(FLAGS) $^ -o $@

build/%.o: src/%.cpp
	g++ -c $< -o $@

clean:
	rm *.o
