#include <Gameplay/GhostManager.h>
#include <Gameplay/Ghost.h>
#include <Gameplay/Pacman.h>
#include <Utils/Constants.h>

GhostManager::GhostManager() :
	current_wave(0),
	wave_timer(LONG_SCATTER_DURATION),
	ghosts({ Ghost(0), Ghost(1), Ghost(2), Ghost(3) })
{

}

void GhostManager::render(bool i_flash, sf::RenderWindow& i_window)
{
	for (Ghost& ghost : ghosts)
	{
		ghost.draw(i_flash, i_window);
	}
}

void GhostManager::reset(char i_level, const std::array<sf::Vector2f, 4>& i_ghost_positions)
{
	current_wave = 0;

	wave_timer = static_cast<float>(LONG_SCATTER_DURATION / pow(2, i_level));

	for (char a = 0; a < 4; a++)
	{
		ghosts[a].set_position(i_ghost_positions[a].x, i_ghost_positions[a].y);
	}

	for (Ghost& ghost : ghosts)
	{
		ghost.reset(ghosts[2].get_position(), ghosts[0].get_position());
	}
}

void GhostManager::update(char i_level, std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman *i_pacman)
{

	if (i_pacman->get_energizer_timer() == 0)
	{
		if (wave_timer == 0)
		{
			if (current_wave < 7)
			{
				current_wave++;

				for (Ghost& ghost : ghosts)
				{
					ghost.switch_mode();
				}
			}

			if (current_wave % 2 == 1)
			{
				wave_timer = CHASE_DURATION;
			}
			else if (current_wave == 2)
			{
				wave_timer = static_cast<short>(LONG_SCATTER_DURATION / pow(2, i_level));
			}
			else
			{
				wave_timer = static_cast<short>(SHORT_SCATTER_DURATION / pow(2, i_level));
			}
		}
		else
		{
			wave_timer--;
		}
	}

	for (Ghost& ghost : ghosts)
	{
		ghost.update(i_level, i_map, ghosts[0], i_pacman);
	}
}
