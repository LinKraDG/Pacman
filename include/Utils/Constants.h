#pragma once

constexpr char FONT_HEIGHT = 16;
constexpr char* FONT_SPRITES = "../Data/Images/Font.png";

constexpr char* GAME_TITLE = "PACMAN";

constexpr char* GHOSTS_SPRITE = "../Data/Images/Ghost.png";
constexpr char GHOST_ANIMATION_FRAMES = 6;
constexpr char GHOST_ANIMATION_SPEED = 4;
constexpr char GHOST_ESCAPE_SPEED = 4;
constexpr char GHOST_FRIGHTENED_SPEED = 3;
constexpr char GHOST_SPEED = 1;

constexpr char GHOST_1_CHASE = 2;
constexpr char GHOST_2_CHASE = 1;
constexpr char GHOST_3_CHASE = 4;



constexpr short CHASE_DURATION = 1024;
constexpr short ENERGIZER_DURATION = 512;
constexpr short FRAME_DURATION = 16667;
constexpr short GHOST_FLASH_START = 64;
constexpr short LONG_SCATTER_DURATION = 512;
constexpr short SHORT_SCATTER_DURATION = 256;

constexpr char* MAP = "../Data/Images/Map.png";
constexpr char* MAP_SKETCH = "../Data/Map/Map.txt";
constexpr char MAP_HEIGHT = 21;
constexpr char MAP_WIDTH = 21;


constexpr char* PACMAN_SPRITE = "../Data/Images/Pacman.png";
constexpr char* PACMAN_DEATH_SPRITE = "../Data/Images/PacmanDeath.png";
constexpr char PACMAN_ANIMATION_SPEED = 4;
constexpr char PACMAN_ANIMATION_FRAMES = 6;
constexpr char PACMAN_DEATH_FRAMES = 12;
constexpr char PACMAN_SPEED = 2;

constexpr char SPRITES_SIZE = 16;

constexpr char SCREEN_RESIZE = 2;

enum MapSprites
{
	Door,
	Empty,
	Energizer,
	Pellet,
	Wall
};