#include <Core/AssetManager.h>
#include <Core/Map.h>
#include <Gameplay/Pacman.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <Utils/Constants.h>


bool Pacman::init()
{


	pacmanTexture = AssetManager::getInstance()->loadTexture(PACMAN_SPRITE);
	pacmanDeathTexture = AssetManager::getInstance()->loadTexture(PACMAN_DEATH_SPRITE);

	dead = false;

	score = 0;

	m_sprite.setPosition(position);
	
	return true;
}

void Pacman::update(char level, std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& map/*float deltaMilliseconds*/)
{
	std::array<bool, 4> walls{};
	walls[0] = map_collision(false, false, PACMAN_SPEED + position.x, position.y, map, this);
	walls[1] = map_collision(false, false, position.x, position.y - PACMAN_SPEED, map, this);
	walls[2] = map_collision(false, false, position.x - PACMAN_SPEED, position.y, map, this);
	walls[3] = map_collision(false, false, position.x, PACMAN_SPEED + position.y, map, this);

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (walls[0] == false)
		{
			l_direction = 0;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (walls[1] == false)
		{
			l_direction = 1;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (walls[2] == false)
		{
			l_direction = 2;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (walls[3] == false)
		{
			l_direction = 3;
		}
	}

	if (walls[l_direction] == false)
	{
		switch (l_direction)
		{
		case 0:
		{
			position.x += PACMAN_SPEED;

			break;
		}
		case 1:
		{
			position.y -= PACMAN_SPEED;

			break;
		}
		case 2:
		{
			position.x -= PACMAN_SPEED;

			break;
		}
		case 3:
		{
			position.y += PACMAN_SPEED;
		}
		}
	}

	if (-SPRITES_SIZE >= position.x)
	{
		position.x = SPRITES_SIZE * MAP_WIDTH - PACMAN_SPEED;
	}
	else if (SPRITES_SIZE * MAP_WIDTH <= position.x)
	{
		position.x = PACMAN_SPEED - SPRITES_SIZE;
	}

	if (map_collision(1, 0, position.x, position.y, map, this) == true)
	{
		energizer_timer = static_cast<float>(ENERGIZER_DURATION / pow(2, level));
	}
	else
	{
		energizer_timer = std::max(.0f, energizer_timer - 1);
	}

}

void Pacman::render(bool victory, sf::RenderWindow& window)
{
	frame = floor(animation_timer / PACMAN_ANIMATION_SPEED);

	if (dead == true || victory == true)
	{
		if (animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
		{
			animation_timer++;


			m_sprite.setTexture(*pacmanDeathTexture);

			m_sprite.setTextureRect(sf::IntRect(SPRITES_SIZE * frame, SPRITES_SIZE * l_direction, SPRITES_SIZE, SPRITES_SIZE));


			window.draw(m_sprite);
		}
		else
		{
			animation_over = true;
		}
	}
	else
	{

		m_sprite.setTexture(*pacmanTexture);

		m_sprite.setTextureRect(sf::IntRect(SPRITES_SIZE * frame, SPRITES_SIZE * l_direction, SPRITES_SIZE, SPRITES_SIZE));
		m_sprite.setPosition(position);
		window.draw(m_sprite);
		animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
	}
	
}

void Pacman::reset()
{
	animation_over = 0;
	dead = false;

	score = .0f;

	l_direction = 0;

	animation_timer = 0;
	energizer_timer = 0;
}

void Pacman::next_level()
{
	animation_over = 0;
	dead = false;

	l_direction = 0;

	animation_timer = 0;
	energizer_timer = 0;
}

void Pacman::death() 
{

	 dead = true;
	 animation_timer = 0;
	
}

bool Pacman::is_dead()
{
	return dead;
}

bool Pacman::animation_is_over()
{
	return animation_over;
}

short Pacman::get_energizer_timer()
{
	return energizer_timer;
}

char Pacman::get_direction()
{
	return l_direction;
}

sf::Vector2f Pacman::get_position()
{
	return position;
}

void Pacman::set_animation_timer(float i_animation_timer)
{
	animation_timer = i_animation_timer;
}

void Pacman::set_position(float x, float y)
{
	position = { x, y };
}

void Pacman::set_score(short score_bonus)
{
	score += score_bonus;
}

short Pacman::get_score()
{
	return score;
}