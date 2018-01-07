default:
	mkdir -p out/
	make all


all: main internal_state led zone schedule led_state controller
	g++ -std=c++14 -o ./out/plantergb ./out/main.o ./out/led.o ./out/zone.o ./out/schedule.o ./out/ledstate.o ./out/controller.o

main:
	g++ -std=c++14 -Wall -c ./main.cpp -o ./out/main.o

# Internal State
internal_state:
	g++ -std=c++14 -Wall -c ./InternalState.cpp -o ./out/InternalState.o

led:
	g++ -std=c++14 -Wall -c ./models/LED.cpp -o ./out/led.o

zone:
	g++ -std=c++14 -Wall -c ./models/Zone.cpp -o ./out/zone.o

schedule:
	g++ -std=c++14 -Wall -c ./models/Schedule.cpp -o ./out/schedule.o

led_state:
	g++ -std=c++14 -Wall -c ./models/LEDState.cpp -o ./out/ledstate.o

controller:
	g++ -std=c++14 -Wall -c ./models/Controller.cpp -o ./out/controller.o


clean:
	rm -f ./out/*.*