#pragma once
#include <array>

#include <Gameplay/Pacman.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Utils/Constants.h>

std::array<std::string, MAP_HEIGHT> load_map(char* map_sketch_file);

void draw_map(const std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& i_map, sf::RenderWindow& i_window);

bool map_collision(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman* i_pacman);

std::array<std::array<MapSprites, MAP_HEIGHT>, MAP_WIDTH> convert_sketch(const std::array<std::string, MAP_HEIGHT>& i_map_sketch, std::array < sf::Vector2f, 4 > &i_ghost_positions, Pacman* i_pacman);