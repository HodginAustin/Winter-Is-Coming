GCC = g++ -std=c++14 -Wall -g


default:
	mkdir -p out/
	make all


all: main internal_state led zone schedule daily_state led_state controller profile
	$(GCC) -o ./out/plantergb ./out/main.o ./out/led.o ./out/zone.o ./out/dailystate.o ./out/schedule.o ./out/ledstate.o ./out/controller.o ./out/profile.o

main:
	$(GCC) -c ./main.cpp -o ./out/main.o

# Internal State
internal_state:
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


clean:
	rm -f ./out/*.*
