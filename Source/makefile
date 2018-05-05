GCC = g++ -std=c++14 -Wall -g
INTERNAL_STATE_OBJECTS = ./out/internalstate.o ./out/led.o ./out/zone.o ./out/dailystate.o ./out/ledstate.o ./out/controller.o ./out/profile.o
API_OBJECTS = ./out/api.o
DATA_PARSER_OBJECTS = ./out/dataparser.o
STATE_COMPOSER_OBJECTS = ./out/statecomposer.o
SETTINGS_OBJECTS = ./out/settings.o

default: directories
	make settings
	make internal_state_objects
	make internal_state
	make api
	make data_parser
	make state_composer
	make control_service_main
	make control_service


# Output directories
directories:
	mkdir -p out/
	mkdir -p out/tests/


# Control service
control_service_main:
	$(GCC) -c ./main.cpp -o ./out/main.o -lpistache -lpthread

control_service:
	$(GCC) -o ./out/plantergb -L/usr/local/lib ./out/main.o $(INTERNAL_STATE_OBJECTS) $(API_OBJECTS) $(DATA_PARSER_OBJECTS) $(STATE_COMPOSER_OBJECTS) $(SETTINGS_OBJECTS) -lpistache -lpthread -lsqlite3


# Internal State
internal_state_objects: led zone daily_state led_state controller profile

internal_state:
	$(GCC) -c ./InternalState.cpp -o ./out/internalstate.o

led:
	$(GCC) -c ./models/LED.cpp -o ./out/led.o

zone:
	$(GCC) -c ./models/Zone.cpp -o ./out/zone.o

daily_state:
	$(GCC) -c ./models/DailyState.cpp -o ./out/dailystate.o

led_state:
	$(GCC) -c ./models/LEDState.cpp -o ./out/ledstate.o

controller:
	$(GCC) -c ./models/Controller.cpp -o ./out/controller.o

profile:
	$(GCC) -c ./models/Profile.cpp -o ./out/profile.o


# Data Parser
data_parser:
	$(GCC) -c ./DataParser.cpp -o ./out/dataparser.o


# API
api:
	$(GCC) -c ./API.cpp -o ./out/api.o


# State Composer
state_composer:
	$(GCC) -c ./StateComposer.cpp -o ./out/statecomposer.o  -lpthread


# Settings
settings:
	$(GCC) -c ./Settings.cpp -o ./out/settings.o  -lpthread


# Tests
tests: directories internal_state_tests data_parser_tests state_composer_tests

internal_state_tests:
	$(GCC) -c ./tests/internalState/run_tests.cpp -o ./out/tests/internal_state.o
	$(GCC) -o ./out/tests/internal_state $(INTERNAL_STATE_OBJECTS) ./out/tests/internal_state.o

data_parser_tests:
	$(GCC) -c ./tests/dataParser/run_tests.cpp -o ./out/tests/data_parser.o
	$(GCC) -o ./out/tests/data_parser $(INTERNAL_STATE_OBJECTS) $(DATA_PARSER_OBJECTS) $(SETTINGS_OBJECTS) ./out/tests/data_parser.o -lsqlite3

state_composer_tests:
	$(GCC) -c ./tests/stateComposer/run_tests.cpp -o ./out/tests/state_composer.o
	$(GCC) -o ./out/tests/state_composer $(STATE_COMPOSER_OBJECTS) $(INTERNAL_STATE_OBJECTS $(DATA_PARSER_OBJECTS) $(SETTINGS_OBJECTS)) ./out/tests/state_composer.o -lpthread

# Install
copy_api:
	# Copy API library if it doesn't exist
	if [ ! -f /usr/local/lib/libpistache.a ]; then \
		sudo cp ./environment/usr/local/lib/libpistache.a /usr/local/lib/ ; \
		sudo cp -r ./environment/usr/local/include/* /usr/local/include/ ; \
	fi

install_sqlite:
	sudo apt-get install libsqlite3-dev
	#sudo apt-get install sqlite3


# Download and build the API from source (only to be used for unix systems other than the Raspberry Pi Zero W)
build_api:
	rm -rf pistache
	git clone https://github.com/oktal/pistache.git
	git submodule update --init
	cd pistache
	mkdir build
	cd build
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
	make
	sudo make install
	rm -rf pistache


# Clean up
clean:
	rm -rf ./out/*
	rm -f *.log
	rm db.sqlite
