target = timetable
objects = main.o timetable.o timetable_io.o

.PHONY: all bin clean

bin: $(target)

all: $(objects)
$(objects): %.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -rf *.o *.txt *.dat *.log

$(target): $(objects)
	$(CC) -o $(target) $(objects)

