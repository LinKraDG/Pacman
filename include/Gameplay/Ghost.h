#pragma once

#include <Gameplay/GameObject.h>
#include <Gameplay/Pacman.h>
#include <Utils/Constants.h>

class Ghost
{
	public:
		Ghost(char i_id);

		~Ghost() = default;

		bool pacman_collision(const sf::Vector2f& i_pacman_position);

		float get_target_distance(char i_direction);

		void draw(bool i_flash, sf::RenderWindow& i_window);
		void reset(const sf::Vector2f& i_home, const sf::Vector2f& i_home_exit);
		void set_position(float i_x, float i_y);
		void switch_mode();
		void update(char i_level, std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& i_map, Ghost& i_ghost_0, Pacman *i_pacman);
		void update_target(char i_pacman_direction, const sf::Vector2f& i_ghost_0_position, const sf::Vector2f& i_pacman_position);

		sf::Vector2f get_position();

		sf::Texture* ghostTexture{ nullptr };

	private:
		bool movement_mode;
		bool use_door;

		char direction;

		char frightened_mode;
		char frightened_speed_timer;

		char id;

		short animation_timer;

		sf::Sprite body;
		sf::Sprite face;

		sf::Vector2f home{ .0f, .0f };
		sf::Vector2f home_exit{ .0f, .0f };
		sf::Vector2f position{ .0f, .0f };
		sf::Vector2f target{ .0f, .0f };

};