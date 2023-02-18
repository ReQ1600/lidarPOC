#include <iostream>
#include <numbers>
#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>

int main()
{
	const double DEG = 1.8;
	const int NUMBER_OF_POINTS = 360 / DEG; //must be whole obviously
	const double ANGLE_INCREASE_RAD = DEG * std::numbers::pi / 180;

	float angleRad = 0;

	std::vector<std::vector<float>> tabCos_sin(NUMBER_OF_POINTS, std::vector<float>(2, 0));
	std::vector<std::vector<float>> tabRotated(NUMBER_OF_POINTS, std::vector<float>(2, 0));

	for (int i = 0; i < NUMBER_OF_POINTS; i++)//structured binding not going to work bc table must be created on runtime unlucky
	{
		tabCos_sin[i][0] = std::round(std::cos(angleRad) * 10000) / 10000;
		tabCos_sin[i][1] = -std::round(std::sin(angleRad) * 10000) / 10000;
		angleRad += ANGLE_INCREASE_RAD;
	}

	angleRad = 0; //for checking
	for (int i = 0; i < NUMBER_OF_POINTS; i++)
	{
		std::cout << "angles: " << angleRad << ": " << tabCos_sin[i][0] << ", " << tabCos_sin[i][1] << '\n';
		angleRad += DEG;
	}

	//"simulating" an array of distance data. Normally it would be provided by a TOF sensor
	srand(std::time(NULL));
	std::unique_ptr<float[]> distance = std::make_unique<float[]>(NUMBER_OF_POINTS);//unque bc dont wanna make a loop for destroing (laziness rly)
	for (int i = 0; i < NUMBER_OF_POINTS; i++)
	{
		distance[i] = rand() % 300 + 100;
		//std::cout <<"dist: " << distance[i] << std::endl;
	}

	//rotating and assigning points for displaying
	for (int i = 0; i < NUMBER_OF_POINTS; i++)
	{
		tabRotated[i][0] = 640 + tabCos_sin[i][1] * distance[i];//x
		tabRotated[i][1] = 450 + tabCos_sin[i][0] * distance[i];//y
		std::cout << "tabrot: " << tabRotated[i][0] << ", " << tabRotated[i][1] << '\n';
	}

	al_init();//only for visualization, not gonna check for nulls
	al_init_primitives_addon();

	ALLEGRO_DISPLAY* disp = al_create_display(1280, 900);//display and other pixel values are hardcoded becouse it's only a poc
	al_clear_to_color(al_map_rgb(137, 148, 153));

	al_draw_filled_circle(640, 450, 4, al_map_rgb(0, 0, 0));//center

	//drawing points around center	
	for (int i = 0; i < NUMBER_OF_POINTS; i++)
	{
		al_draw_filled_circle(tabRotated[i][0], tabRotated[i][1], 2, al_map_rgb(255, 255, 255)); //in here only pointY * [-sin(angle),cos(angle)] is needed becouse x is always 0
		if (i + 1 == NUMBER_OF_POINTS) al_draw_line(tabRotated[i][0], tabRotated[i][1], tabRotated[0][0], tabRotated[0][1], al_map_rgb(0, 0, 0), 2);
		else al_draw_line(tabRotated[i][0], tabRotated[i][1], tabRotated[i + 1][0], tabRotated[i + 1][1], al_map_rgb(0, 0, 0), 2);
	}

	al_flip_display();
	getchar();//used for closing cuz getting events and doing a proper loop is an overkill for this thing
	al_destroy_display(disp);
	al_shutdown_primitives_addon();
	return 0;
}