#include <cassert>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <Utils/Constants.h>

bool Game::init(GameCreateInfo& createInfo)
{
	assert(m_window == nullptr && m_world == nullptr,
		"Game is already initialized, we are about to leak memory");

	m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);
	m_window->setView(sf::View(sf::FloatRect(0, 0, SPRITES_SIZE * MAP_WIDTH, FONT_HEIGHT*2 + SPRITES_SIZE * MAP_HEIGHT)));

	m_world = new World();
	const bool loadOk = m_world->load();

	return loadOk;
}

Game::~Game()
{
	delete m_world;
	delete m_window;
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	for (auto event = sf::Event(); m_window->pollEvent(event);)
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
		}
	}

	m_world->update(deltaMilliseconds);
}

void Game::render()
{
	m_window->clear();

	m_world->render(*m_window);

	m_window->display();
}