#include "Menu.hpp"
#include <ctime>
#include <exception>
#include <iostream>

int main()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	try
	{
		Menu menu;
		menu.executar();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Erro fatal: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

//assets obst�culo: https://bdragon1727.itch.io/free-trap-platformer
//assets inimigos: https://luizmelo.itch.io/monsters-creatures-fantasy
//assets jogador: https://aamatniekss.itch.io/fantasy-knight-free-pixelart-animated-character
//assets background: https://craftpix.net/freebies/free-pixel-art-fantasy-2d-battlegrounds/
//assets chef�o: https://elthen.itch.io/2d-pixel-art-cacodaemon-sprites
//assets bola de fogo: https://xyezawr.itch.io/free-pixel-effects-pack-13-fireballs?download
//assets portal: https://ruxar.itch.io/portal
//asset background menu: https://www.slynyrd.com/blog/2019/4/23/pixelblog-16-medieval-fantasy
//asset plataforma: https://www.pngwing.com/en/free-png-czlvo
//fonte: https://ninjikin.itch.io/font-antiquity-script