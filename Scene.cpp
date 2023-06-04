#include "Scene.h"
#include "GameEngine.h"

Scene::Scene()
{

}

Scene::Scene(GameEngine * gameEngine)
    : m_game(gameEngine)
{ 
    
}

void Scene::setPaused(bool paused)
{
    m_paused = paused;
}

size_t Scene::width() const
{
    return m_game->window().getSize().x;
}

size_t Scene::height() const
{
    return m_game->window().getSize().y;
}

size_t Scene::currentFrame() const
{
    return m_currentFrame;
}

bool Scene::hasEnded() const
{
    return m_hasEnded;
}

const ActionMap& Scene::getActionMap() const
{
    return m_actionMap;
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
    m_actionMap[inputKey] = actionName;
}

void Scene::doAction(const Action& action)
{
    // ignore null actions
    if (action.name() == "NONE") { return; }

    sDoAction(action);
}

void Scene::simulate(const size_t frames)
{
    for (size_t i = 0; i < frames; i++)
    {
        update();
    }
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
    // Create a SFML vertex array to hold the line vertices
    sf::VertexArray line(sf::Lines, 2);

    // Set the position and color of the line vertices
    line[0].position = sf::Vector2f(p1.x, p1.y);
    line[0].color = sf::Color::White;
    line[1].position = sf::Vector2f(p2.x, p2.y);
    line[1].color = sf::Color::White;

    // Draw the line to the SFML render window
    m_game->window().draw(line);
}
