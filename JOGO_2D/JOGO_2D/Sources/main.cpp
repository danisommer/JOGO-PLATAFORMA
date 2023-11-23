#include "Menu.hpp"

int main()
{
	time_t t;
	srand(time(&t));

	Menu menu;

	menu.executar();

	return 0;
}
