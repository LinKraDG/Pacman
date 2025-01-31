#include <Core/AssetManager.h>
#include <Core/Map.h>
#include <Core/World.h>
#include <Gameplay/Pacman.h>
#include <Gameplay/GhostManager.h>
#include <UI/Text.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

World::~World()
{
	delete m_ghostManager;
	delete m_pacman;
}

bool World::load()
{
	level = 0;
	game_won = false;

	constexpr float millisecondsToSeconds = 1 / 1000.f;

	std::array<sf::Vector2f, 4> ghost_positions;


	GhostManager* ghost_manager = new GhostManager();

	Pacman* pacman = new Pacman();
	const bool initOk = pacman->init();

	map_sketch = load_map(MAP_SKETCH);
	m_map = convert_sketch(map_sketch, ghost_positions, pacman);

	m_ghostManager = ghost_manager;
	ghost_manager->reset(level, ghost_positions);
	m_pacman = pacman;

	return initOk;
}

void World::update(uint32_t deltaMilliseconds)
{
	if (game_won == false && m_pacman->is_dead() == false)
	{
		game_won = true;

		m_pacman->update(level, m_map);

		m_ghostManager->update(level, m_map, m_pacman);

		for (const std::array<MapSprites, MAP_HEIGHT>& column : m_map)
		{
			for (const MapSprites& cell : column)
			{
				if (MapSprites::Pellet == cell)
				{
					game_won = 0;

					break;
				}
			}

			if (game_won == false)
			{
				break;
			}
		}

		if (game_won == true)
		{
			m_pacman->set_animation_timer(0);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		game_won = 0;

		if (m_pacman->is_dead() == true)
		{
			level = 0;
			m_pacman->reset();
		}
		else
		{
			level++;
			m_pacman->next_level();
		}

		m_map = convert_sketch(map_sketch, ghost_positions, m_pacman);

		m_ghostManager->reset(level, ghost_positions);
	}
}

void World::render(sf::RenderWindow& window)
{

	if (0 == game_won && 0 == m_pacman->is_dead())
	{
		draw_map(m_map, window);

		m_ghostManager->render(GHOST_FLASH_START >= m_pacman->get_energizer_timer(), window);
		draw_text(0, 0, SPRITES_SIZE * MAP_HEIGHT, "Nivel: " + std::to_string(1 + level), window);
		draw_text(0, 0, SPRITES_SIZE * MAP_HEIGHT + SPRITES_SIZE, "Puntuacion: " + std::to_string(m_pacman->get_score()), window);
	}

	m_pacman->render(game_won, window);

	if (m_pacman->animation_is_over())
	{
		if (1 == game_won)
		{
			draw_text(1, 0, 0, "Siguiente nivel!", window);
		}
		else
		{
			draw_text(1, 0, 0, "Has perdido", window);
		}
	}
}