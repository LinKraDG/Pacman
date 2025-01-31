#include <array>
#include <Core/Map.h>
#include <Gameplay/Pacman.h>
#include <SFML/Graphics.hpp>
#include <Utils/Constants.h>


std::array<std::string, MAP_HEIGHT> load_map(char* map_sketch_file)
{
	//char mapMatrix[29][31];
	/*std::array<std::string, MAP_HEIGHT> map_sketch;

	FILE* mapFile;
	fopen_s(&mapFile, map_sketch_file, "r");

	char line[30];
	char* nextToken;

	//fgets(line, 29, mapFile);
	char* ptr = strtok_s(line, ",", &nextToken);
	int r = atoi(ptr);
	int c = atoi(strtok_s(NULL, ",", &nextToken));

	while (!feof(mapFile))
	{
		fgets(line, 29, mapFile);
		c = 0;
		ptr = strtok_s(line, ",", &nextToken);
		while (ptr != NULL && c < 30 && r < 33) {
			map_sketch[r][c] = atof(ptr);
			c++;
			ptr = strtok_s(NULL, ",", &nextToken);
		}
	}*/
	std::array<std::string, MAP_HEIGHT> map_sketch = {
		" ################### ",
		" #........#........# ",
		" #o##.###.#.###.##o# ",
		" #.................# ",
		" #.##.#.#####.#.##.# ",
		" #....#...#...#....# ",
		" ####.### # ###.#### ",
		"    #.#   0   #.#    ",
		"#####.# ##=## #.#####",
		"     .  #123#  .     ",
		"#####.# ##### #.#####",
		"    #.#       #.#    ",
		" ####.# ##### #.#### ",
		" #........#........# ",
		" #.##.###.#.###.##.# ",
		" #o.#.....P.....#.o# ",
		" ##.#.#.#####.#.#.## ",
		" #....#...#...#....# ",
		" #.######.#.######.# ",
		" #.................# ",
		" ################### "
	};

	return map_sketch;
}

void draw_map(const std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& i_map, sf::RenderWindow& i_window)
{
	sf::Sprite sprite;

	sf::Texture texture;
	texture.loadFromFile(MAP);

	sprite.setTexture(texture);

	for (char a = 0; a < MAP_WIDTH; a++)
	{
		for (char b = 0; b < MAP_HEIGHT; b++)
		{
			sprite.setPosition(static_cast<float>(SPRITES_SIZE * a), static_cast<float>(SPRITES_SIZE * b));

			switch (i_map[a][b])
			{
				case MapSprites::Door:
				{
					sprite.setTextureRect(sf::IntRect(2 * SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE));

					i_window.draw(sprite);

					break;
				}
				case MapSprites::Energizer:
				{
					sprite.setTextureRect(sf::IntRect(SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE));

					i_window.draw(sprite);

					break;
				}
				case MapSprites::Pellet:
				{
					sprite.setTextureRect(sf::IntRect(0, SPRITES_SIZE, SPRITES_SIZE, SPRITES_SIZE));

					i_window.draw(sprite);

					break;
				}
				case MapSprites::Wall:
				{
					bool down = 0;
					bool left = 0;
					bool right = 0;
					bool up = 0;

					if (b < MAP_HEIGHT - 1)
					{
						if (MapSprites::Wall == i_map[a][1 + b])
						{
							down = 1;
						}
					}

					if (0 < a)
					{
						if (MapSprites::Wall == i_map[a - 1][b])
						{
							left = 1;
						}
					}
					else
					{
						left = 1;
					}

					if (a < MAP_WIDTH - 1)
					{
						if (MapSprites::Wall == i_map[1 + a][b])
						{
							right = 1;
						}
					}
					else
					{
						right = 1;
					}

					if (0 < b)
					{
						if (MapSprites::Wall == i_map[a][b - 1])
						{
							up = 1;
						}
					}

					sprite.setTextureRect(sf::IntRect(SPRITES_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, SPRITES_SIZE, SPRITES_SIZE));

					i_window.draw(sprite);
				}
			}
		}
	}
}

std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH> convert_sketch(const std::array<std::string, MAP_HEIGHT>& i_map_sketch, std::array<sf::Vector2f, 4>& i_ghost_positions, Pacman *i_pacman)
{
	std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH> output_map{};

	for (char a = 0; a < MAP_HEIGHT; a++)
	{
		for (char b = 0; b < MAP_WIDTH; b++)
		{
			output_map[b][a] = MapSprites::Empty;

			switch (i_map_sketch[a][b])
			{
				case '#':
				{
					output_map[b][a] = MapSprites::Wall;

					break;
				}
				case '=':
				{
					output_map[b][a] = MapSprites::Door;

					break;
				}
				case '.':
				{
					output_map[b][a] = MapSprites::Pellet;

					break;
				}
				case '0':
				{
					i_ghost_positions[0].x = SPRITES_SIZE * b;
					i_ghost_positions[0].y = SPRITES_SIZE * a;

					break;
				}
				case '1':
				{
					i_ghost_positions[1].x = SPRITES_SIZE * b;
					i_ghost_positions[1].y = SPRITES_SIZE * a;

					break;
				}
				case '2':
				{
					i_ghost_positions[2].x = SPRITES_SIZE * b;
					i_ghost_positions[2].y = SPRITES_SIZE * a;

					break;
				}
				case '3':
				{
					i_ghost_positions[3].x = SPRITES_SIZE * b;
					i_ghost_positions[3].y = SPRITES_SIZE * a;

					break;
				}
				case 'P':
				{
					i_pacman->set_position(SPRITES_SIZE * b, SPRITES_SIZE * a);

					break;
				}
				case 'o':
				{
					output_map[b][a] = MapSprites::Energizer;
				}
			}
		}
	}

	return output_map;
}

bool map_collision(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman* i_pacman)
{
	bool output = 0;

	float cell_x = i_x / static_cast<float>(SPRITES_SIZE);
	float cell_y = i_y / static_cast<float>(SPRITES_SIZE);

	for (char a = 0; a < 4; a++)
	{
		float x = .0f;
		float y = .0f;

		switch (a)
		{
		case 0:
		{
			x = static_cast<float>(floor(cell_x));
			y = static_cast<float>(floor(cell_y));

			break;
		}
		case 1:
		{
			x = static_cast<float>(ceil(cell_x));
			y = static_cast<float>(floor(cell_y));

			break;
		}
		case 2: 
		{
			x = static_cast<float>(floor(cell_x));
			y = static_cast<float>(ceil(cell_y));

			break;
		}
		case 3: 
		{
			x = static_cast<float>(ceil(cell_x));
			y = static_cast<float>(ceil(cell_y));
		}
		}

		if (0 <= x && 0 <= y && MAP_HEIGHT > y && MAP_WIDTH > x)
		{
			if (0 == i_collect_pellets) 
			{
				if (MapSprites::Wall == i_map[x][y])
				{
					output = 1;
				}
				else if (0 == i_use_door && MapSprites::Door == i_map[x][y])
				{
					output = 1;
				}
			}
			else 
			{
				if (MapSprites::Energizer == i_map[x][y])
				{
					output = 1;

					i_map[x][y] = MapSprites::Empty;
					i_pacman->set_score(50);
				}
				else if (MapSprites::Pellet == i_map[x][y])
				{
					i_map[x][y] = MapSprites::Empty;
					i_pacman->set_score(10);
				}
			}
		}
	}

	return output;
}