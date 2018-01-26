#include "./includes/header.hpp"

int main(){

	int option = 0;
	int bed_number = 0;
	int zone_number = 0;

	int ColorR = 0;
	int ColorG = 0;
	int ColorB = 0;
	int schedule_number = 0;
	int schedule_edit = 0;
	int brightness = 0;

	string url;

	system("clear");
	cout << "|-----------Welcome to PlanteRGB Lighting System-----------|" << endl;
	do{
		cout << "|-----------Configuration Menu-----------|" << endl;
		cout << " BED(1) | Zones(2) | Scheduling(3) | Profiles(4) | Apply Changes(5): ";
		cin >> option;
		if(option == 1){ // BED
			bed_number = configure_bed();
			
		}else if ( option == 2){ // Zones
			configure_zone();
		}else if ( option == 3){ // Scheduling
			configure_schedule(ColorR, ColorG, ColorB, schedule_number, schedule_edit, brightness);
				}else if( option == 4){ // profiles
			configure_profile();
		}
	}while(option != 5);

	cout << "Changes Applied" << endl;
	cout << "Thanks for using PlanteR-GB Lighting System!"<<endl;;

	return 0;
}

int configure_bed (){

	int bed_number;

	system("clear");
	cout << "|-----------Bed Configuration-----------|" << endl;
	cout << "Select which bed you would like to configure(1-4): ";
	cin >> bed_number;
	cout << endl;
	cout << "Bed " << bed_number << " was selected." << endl;
	return bed_number;
}
int configure_zone(){

	int zone_number;

	system("clear");
	cout << "|-----------Zone Configuration-----------|" << endl;
				cout << "Which Zone would you like to configure(1-4): ";
				cin >> zone_number;
				cout << endl;
				cout << "Congiuring zone " << zone_number << endl;
	return 1;
}

int configure_schedule(int &ColorR, int &ColorG, int &ColorB, int &schedule_number, int &schedule_edit, int &brightness)
{


	string schedule_time;

	system("clear");
	cout << "|-----------Scheduling Configuration-----------|" << endl;
	cout << "Edit Schedule(1) | Add Schedule (2)";
	cin >> schedule_number;
	if(schedule_number == 1){
		cout << "Editing schedule" << endl;
		cout << "which schedule would you like to edit?(1-4)";
		cin >> schedule_edit;
		cout << endl;
		cout << "Editing schedule " << schedule_edit << endl;
		cout << "Enter in Time: ";
		cin >> schedule_time;
		cout << endl;
		cout << "Enter in Red Value: ";
		cin >> ColorR;
		cout << endl;
		cout << "Enter in Green Value: ";
		cin >> ColorG;
		cout << endl;
		cout << "Enter in Blue Value: ";
		cin >> ColorB;
		cout << endl;
		cout << "Enter Brightness (0-100)";
		cin >> brightness;
		cout << endl;
		cout << "The current configuration is: Time: " << schedule_time
		 << " Red Value: " << ColorR << " Green Value: " << ColorG 
		 << " Blue Value: " << ColorB << " Brightness Level: " << brightness << endl;
	}else if(schedule_number = 2){

	}

	return 1;
}

int configure_profile(){
	system("clear");
	cout << "|-----------Profile Configuration-----------|" << endl;
	cout << "select Profile (1) | Add Profile (2)";
	return 1;
}


string build_url(){

	string url;



	return url;

}