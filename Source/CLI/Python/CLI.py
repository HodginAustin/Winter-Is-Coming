import os


def configure_Zones():
	os.system('cls' if os.name == 'nt' else 'clear')
	print("|------------------------Zone Menu------------------------|")
	option = input("Which Zone would you like to configure (1-4): ")


def configure_Schedule():
	os.system('cls' if os.name == 'nt' else 'clear')


def configure_profiles():
	os.system('cls' if os.name == 'nt' else 'clear')



def main():

	option = 0;

	while option != '4':
		os.system('cls' if os.name == 'nt' else 'clear')

		print("|-----------Welcome to PlanteRGB Lighting System-----------|")

		print("|--------------------Configuration Menu--------------------|")

		option = input(" Zones(1) | Scheduling(2) | Profiles(3) | Apply Changes(4): ")

		if option == 1:
			print("Selected Zones")
			configure_Zones()
		elif  option == 2:
			print("Selected Scheduling")
			configure_Schedule()
		elif  option == 3:
			print("Profiles Selected")
			configure_profiles()
		elif  option == 4:
			print("Changes Applied")


if __name__ == "__main__":
	main() #Startng the main function
