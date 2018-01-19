GCC = g++ -std=c++14 -Wall -g
INTERNAL_STATE_OBJECTS = ./out/internalstate.o ./out/led.o ./out/zone.o ./out/dailystate.o ./out/schedule.o ./out/ledstate.o ./out/controller.o ./out/profile.o


default: directories
	make internal_state
	make control_service_main
	make control_service
	make tests


# Output directories
directories:
	mkdir -p out/
	mkdir -p out/tests/


# Control service
control_service_main:
	$(GCC) -c ./main.cpp -o ./out/main.o

control_service:
	$(GCC) -o ./out/plantergb ./out/main.o $(INTERNAL_STATE_OBJECTS)


# Internal State
internal_state: internal_state_manager led zone schedule daily_state led_state controller profile

internal_state_manager:
	$(GCC) -c ./InternalState.cpp -o ./out/internalstate.o

led:
	$(GCC) -c ./models/LED.cpp -o ./out/led.o

zone:
	$(GCC) -c ./models/Zone.cpp -o ./out/zone.o

schedule:
	$(GCC) -c ./models/Schedule.cpp -o ./out/schedule.o

daily_state:
	$(GCC) -c ./models/DailyState.cpp -o ./out/dailystate.o

led_state:
	$(GCC) -c ./models/LEDState.cpp -o ./out/ledstate.o

controller:
	$(GCC) -c ./models/Controller.cpp -o ./out/controller.o

profile:
	$(GCC) -c ./models/Profile.cpp -o ./out/profile.o


# Tests
tests: directories
	$(GCC) -c ./tests/run_tests.cpp -o ./out/tests/run_tests.o
	$(GCC) -o ./out/tests/run_tests $(INTERNAL_STATE_OBJECTS) ./out/tests/run_tests.o


clean:
	rm -rf ./out/*
