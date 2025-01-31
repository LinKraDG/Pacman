#pragma once

#include <array>
#include <cstdint>
#include <SFML/System/Vector2.hpp>
#include <Utils/Constants.h>

class GhostManager;
class Pacman;

namespace sf
{
	class RenderWindow;
}

class World
{
	public:

		~World();

		bool load();

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		bool game_won;
		char level;

		std::array<sf::Vector2f, 4> ghost_positions;

		GhostManager* m_ghostManager{ nullptr };
		Pacman* m_pacman{ nullptr };

		std::array<std::string, MAP_HEIGHT> map_sketch;
		std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH> m_map{};
};