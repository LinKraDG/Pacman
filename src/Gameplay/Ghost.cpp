#include <array>
#include <cmath>
#include <Core/AssetManager.h>
#include <Core/Map.h>
#include <Gameplay/Pacman.h>
#include <Gameplay/Ghost.h>
#include <Utils/Constants.h>
#include <SFML/Graphics.hpp>

Ghost::Ghost(char i_id) : id(i_id)
{
	ghostTexture = AssetManager::getInstance()->loadTexture(GHOSTS_SPRITE);
}

bool Ghost::pacman_collision(const sf::Vector2f& i_pacman_position)
{
	if (position.x > i_pacman_position.x - SPRITES_SIZE && position.x < SPRITES_SIZE + i_pacman_position.x)
	{
		if (position.y > i_pacman_position.y - SPRITES_SIZE && position.y < SPRITES_SIZE + i_pacman_position.y)
		{
			return true;
		}
	}

	return false;
}

float Ghost::get_target_distance(char i_direction)
{
	short x = position.x;
	short y = position.y;

	switch (i_direction)
	{
		case 0:
		{
			x += GHOST_SPEED;

			break;
		}
		case 1:
		{
			y -= GHOST_SPEED;

			break;
		}
		case 2:
		{
			x -= GHOST_SPEED;

			break;
		}
		case 3:
		{
			y += GHOST_SPEED;
		}
	}

	return static_cast<float>(sqrt(pow(x - target.x, 2) + pow(y - target.y, 2)));
}

void Ghost::draw(bool i_flash, sf::RenderWindow& i_window)
{
	char body_frame = static_cast<char>(floor(animation_timer / static_cast<float>(GHOST_ANIMATION_SPEED)));

	sf::Sprite body;
	sf::Sprite face;

	body.setTexture(*ghostTexture);
	body.setPosition(position.x, position.y);
	body.setTextureRect(sf::IntRect(SPRITES_SIZE * body_frame, 0, SPRITES_SIZE, SPRITES_SIZE));

	face.setTexture(*ghostTexture);
	face.setPosition(position.x, position.y);

	if (0 == frightened_mode)
	{
		switch (id)
		{
			case 0:
			{
				body.setColor(sf::Color(255, 0, 0));

				break;
			}
			case 1:
			{
				body.setColor(sf::Color(255, 182, 255));

				break;
			}
			case 2:
			{
				body.setColor(sf::Color(0, 255, 255));

				break;
			}
			case 3:
			{
				body.setColor(sf::Color(255, 182, 85));
			}
		}

		face.setTextureRect(sf::IntRect(SPRITES_SIZE * direction, SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE));

		i_window.draw(body);
	}
	else if (1 == frightened_mode)
	{
		body.setColor(sf::Color(36, 36, 255));
		face.setTextureRect(sf::IntRect(4 * SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE));

		if (1 == i_flash && 0 == body_frame % 2)
		{
			body.setColor(sf::Color(255, 255, 255));
			face.setColor(sf::Color(255, 0, 0));
		}
		else
		{
			body.setColor(sf::Color(36, 36, 255));
			face.setColor(sf::Color(255, 255, 255));
		}

		i_window.draw(body);
	}
	else
	{
		face.setTextureRect(sf::IntRect(SPRITES_SIZE * direction, 2 * SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE));
	}

	i_window.draw(face);

	animation_timer = (1 + animation_timer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}

void Ghost::reset(const sf::Vector2f& i_home, const sf::Vector2f& i_home_exit)
{
	movement_mode = 0;
	use_door = 0 < id;

	direction = 0;
	frightened_mode = 0;
	frightened_speed_timer = 0;

	animation_timer = 0;

	home = i_home;
	home_exit = i_home_exit;
	target = i_home_exit;
}

void Ghost::set_position(float i_x, float i_y)
{
	position = { i_x, i_y };
}

void Ghost::switch_mode()
{
	movement_mode = 1 - movement_mode;
}

void Ghost::update(char i_level, std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& i_map, Ghost& i_ghost_0, Pacman *i_pacman)
{
	bool move = 0;

	char available_ways = 0;
	char speed = GHOST_SPEED;

	std::array<bool, 4> walls{};

	if (frightened_mode == 0 && i_pacman->get_energizer_timer() == ENERGIZER_DURATION / pow(2, i_level))
	{
		frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

		frightened_mode = 1;
	}
	else if (i_pacman->get_energizer_timer() == 0 && frightened_mode == 1)
	{
		frightened_mode = 0;
	}

	if (frightened_mode == 2 && fmod(position.x,GHOST_ESCAPE_SPEED) == 0 && fmod(position.y, GHOST_ESCAPE_SPEED) == 0)
	{
		speed = GHOST_ESCAPE_SPEED;
	}

	update_target(i_pacman->get_direction(), i_ghost_0.get_position(), i_pacman->get_position());

	walls[0] = map_collision(0, use_door, speed + position.x, position.y, i_map, i_pacman);
	walls[1] = map_collision(0, use_door, position.x, position.y - speed, i_map, i_pacman);
	walls[2] = map_collision(0, use_door, position.x - speed, position.y, i_map, i_pacman);
	walls[3] = map_collision(0, use_door, position.x, speed + position.y, i_map, i_pacman);

	if (1 != frightened_mode)
	{
		char optimal_direction = 4;

		move = 1;

		for (char a = 0; a < 4; a++)
		{
			if (a == (2 + direction) % 4)
			{
				continue;
			}
			else if (0 == walls[a])
			{
				if (4 == optimal_direction)
				{
					optimal_direction = a;
				}

				available_ways++;

				if (get_target_distance(a) < get_target_distance(optimal_direction))
				{
					optimal_direction = a;
				}
			}
		}

		if (1 < available_ways)
		{
			direction = optimal_direction;
		}
		else
		{
			if (4 == optimal_direction)
			{
				direction = (2 + direction) % 4;
			}
			else
			{
				direction = optimal_direction;
			}
		}
	}
	else
	{
		char random_direction = rand() % 4;

		if (0 == frightened_speed_timer)
		{
			move = 1;

			frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

			for (char a = 0; a < 4; a++)
			{
				if (a == (2 + direction) % 4)
				{
					continue;
				}
				else if (0 == walls[a])
				{
					available_ways++;
				}
			}

			if (0 < available_ways)
			{
				while (1 == walls[random_direction] || random_direction == (2 + direction) % 4)
				{
					random_direction = rand() % 4;
				}

				direction = random_direction;
			}
			else
			{
				direction = (2 + direction) % 4;
			}
		}
		else
		{
			frightened_speed_timer--;
		}
	}

	if (1 == move)
	{
		switch (direction)
		{
		case 0:
		{
			position.x += speed;

			break;
		}
		case 1:
		{
			position.y -= speed;

			break;
		}
		case 2:
		{
			position.x -= speed;

			break;
		}
		case 3:
		{
			position.y += speed;
		}
		}

		if (-SPRITES_SIZE >= position.x)
		{
			position.x = SPRITES_SIZE * MAP_WIDTH - speed;
		}
		else if (position.x >= SPRITES_SIZE * MAP_WIDTH)
		{
			position.x = speed - SPRITES_SIZE;
		}
	}

	if (pacman_collision(i_pacman->get_position()) == true)
	{
		if (frightened_mode == 0)
		{
			i_pacman->death();
		}
		else 
		{
			if (frightened_mode == 1) {
				i_pacman->set_score(200);
			}
			use_door = 1;

			frightened_mode = 2;

			target = home;
			
		}
	}
}

void Ghost::update_target(char i_pacman_direction, const sf::Vector2f& i_ghost_0_position, const sf::Vector2f& i_pacman_position)
{
	if (1 == use_door)
	{
		if (position == target)
		{
			if (home_exit == target)
			{
				use_door = 0;
			}
			else if (home == target)
			{
				frightened_mode = 0;

				target = home_exit;
			}
		}
	}
	else
	{
		if (0 == movement_mode)
		{
			switch (id)
			{
			case 0:
			{
				target = { SPRITES_SIZE * (MAP_WIDTH - 1), 0 };

				break;
			}
			case 1:
			{
				target = { 0, 0 };

				break;
			}
			case 2:
			{
				target = { SPRITES_SIZE * (MAP_WIDTH - 1), SPRITES_SIZE * (MAP_HEIGHT - 1) };

				break;
			}
			case 3:
			{
				target = { 0, SPRITES_SIZE * (MAP_HEIGHT - 1) };
			}
			}
		}
		else
		{
			switch (id)
			{
			case 0:
			{
				target = i_pacman_position;

				break;
			}
			case 1:
			{
				target = i_pacman_position;

				switch (i_pacman_direction)
				{
				case 0:
				{
					target.x += SPRITES_SIZE * GHOST_1_CHASE;

					break;
				}
				case 1:
				{
					target.y -= SPRITES_SIZE * GHOST_1_CHASE;

					break;
				}
				case 2:
				{
					target.x -= SPRITES_SIZE * GHOST_1_CHASE;

					break;
				}
				case 3:
				{
					target.y += SPRITES_SIZE * GHOST_1_CHASE;
				}
				}

				break;
			}
			case 2:
			{
				target = i_pacman_position;

				switch (i_pacman_direction)
				{
				case 0:
				{
					target.x += SPRITES_SIZE * GHOST_2_CHASE;

					break;
				}
				case 1:
				{
					target.y -= SPRITES_SIZE * GHOST_2_CHASE;

					break;
				}
				case 2:
				{
					target.x -= SPRITES_SIZE * GHOST_2_CHASE;

					break;
				}
				case 3:
				{
					target.y += SPRITES_SIZE * GHOST_2_CHASE;
				}
				}

				target.x += target.x - i_ghost_0_position.x;
				target.y += target.y - i_ghost_0_position.y;

				break;
			}
			case 3:
			{
				if (SPRITES_SIZE * GHOST_3_CHASE <= sqrt(pow(position.x - i_pacman_position.x, 2) + pow(position.y - i_pacman_position.y, 2)))
				{
					target = i_pacman_position;
				}
				else
				{
					target = { 0, SPRITES_SIZE * (MAP_HEIGHT - 1) };
				}
			}
			}
		}
	}
}

sf::Vector2f Ghost::get_position()
{
	return position;
}
