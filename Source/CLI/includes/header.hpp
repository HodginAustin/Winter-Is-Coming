#include <iostream>
#include <cstdlib> 
#include<string>


using namespace std;


int configure_bed ();
int configure_zone();
int configure_schedule(int &ColorR, int &ColorG, int &ColorB, int &schedule_number, int &schedule_edit, int &brightness);
int configure_profile();

int Apply_config();

string build_url();

