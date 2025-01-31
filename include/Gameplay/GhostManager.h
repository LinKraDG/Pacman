#pragma once
#include <array>
#include <Gameplay/Ghost.h>
#include <Utils/Constants.h>
#include <SFML/Graphics/RenderWindow.hpp>

class Pacman;

class GhostManager{
	public:
		GhostManager();

		~GhostManager() = default;

		void render(bool i_flash, sf::RenderWindow& i_window);
		void reset(char i_level, const std::array<sf::Vector2f, 4>& i_ghost_positions);

		void update(char i_level, std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman *i_pacman);

	private:

		char current_wave;

		short wave_timer;

		std::array<Ghost, 4> ghosts;

};