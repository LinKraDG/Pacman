#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>
#include <string>
#include <Utils/Constants.h>

class Pacman
{
	public:
		sf::Vector2f position;

		sf::Texture* pacmanTexture{ nullptr };
		sf::Texture* pacmanDeathTexture{ nullptr };
		sf::Vector2f speed{ .0f, .0f };

		bool isAlive;

		bool init();

		~Pacman() = default;

		void update(char level, std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& map);
		void render(bool victory, sf::RenderWindow& window);
		void reset();
		void Pacman::next_level();

		bool animation_is_over();
		void set_animation_timer(float i_animation_timer);
		short get_energizer_timer();
		char get_direction();

		void set_position(float x, float y);
		sf::Vector2f get_position();

		void set_score(short score_bonus);
		short get_score();

		bool is_dead();
		void death();
		
	private:
		sf::Sprite m_sprite;

		bool animation_over;

		char l_direction;
		char frame;

		short animation_timer;
		float energizer_timer;
		float score;

		bool dead;

		sf::Vector2f m_direction{ .0f, .0f };
		
};