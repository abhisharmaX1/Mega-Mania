#include "Scene_Play.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Scene_Menu.h"
#include "Action.h"

#include <iostream>
#include <fstream>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath)
{
    init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath)
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing (C)ollision boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing (G)rid

    // TODO: Register all other gameplay actions
    registerAction(sf::Keyboard::Up, "JUMP");
    registerAction(sf::Keyboard::Left, "MOVE_LEFT");
    registerAction(sf::Keyboard::Right, "MOVE_RIGHT");
    registerAction(sf::Keyboard::Down, "MOVE_DOWN");
    registerAction(sf::Keyboard::Space, "SHOOT"); 

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Arial"));

    loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity) {
    // TODO
    return Vec2(0, 0);
}

void Scene_Play::loadLevel(const std::string &filename)
{
    // reset the entity manager everytime we load a level
    m_entityManager = EntityManager();

    
    std::ifstream f(filename);
    std::string str;
    while (f >> str) {
        if (str == "Tile") {
            std::string type; size_t x, y;
            f >> type >> x >> y;
            auto e = m_entityManager.addEntity("tile");
            e->addComponent<CTransform>(Vec2(x, y));
            e->addComponent<CAnimation>(m_game->assets().getAnimation(type), true);
            e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
            // add a bounding box, this will now show up if we press the 'C' Key
            e->addComponent<CBoundingBox>(m_game->assets().getAnimation(type).getSize());
        } else if (str == "Dec") {
            std::string type; size_t x, y;
            f >> type >> x >> y;
            auto e = m_entityManager.addEntity("dec");
            e->addComponent<CTransform>(Vec2(x, y));
            e->addComponent<CAnimation>(m_game->assets().getAnimation(type), true);
        } else {
            f >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY
              >> m_playerConfig.SPEED >> m_playerConfig.JUMP >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY
              >> m_playerConfig.WEAPON;
            spawnPlayer();

        }
    } 



    

    // NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
    //       Components are now returned as references rather than pointers
    //       If you do not specify a reference variable type, it will copy the component
    //       Here is an example;
    //
    //       auto transform1 = entity->get<CTransform>()
    //       This will copy the transform into the variable 'transform1' - it is incorrect
    //       Any changes you make to transform1 will not be changed inside the entity
    //
    //       auto& transform2 = entoity->get<CTransform>()
    //       This will reference the transform with the variable 'transform2' - it is correct
    //       Now any changes you make to transform2 will be changed inside the entity
    //
}

void Scene_Play::spawnPlayer()
{
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
    m_player->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));
    m_player->getComponent<CTransform>().prevPos = Vec2(m_playerConfig.X, m_playerConfig.Y);
    // m_player->getComponent<CTransform>().velocity = Vec2(m_playerConfig.SPEED, m_playerConfig.JUMP);
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    m_player->addComponent<CInput>().canJump = false;
    m_player->addComponent<CState>("AIR");
    // TODO: be sure to add the remaining components to the player


    // helps with state management(ground or air)
    auto e = m_entityManager.addEntity("box");
    e->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y - 1));
    // e->getComponent<CTransform>().velocity = Vec2(eConfig.SPEED, eConfig.JUMP);
    e->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    // this should spawn a bullet at the given entity, going in the direction the entity is facing
    auto bullet = m_entityManager.addEntity("bullet");
    bullet->addComponent<CAnimation>(m_game->assets().getAnimation(m_playerConfig.WEAPON), true);
    bullet->addComponent<CTransform>(entity->getComponent<CTransform>().pos);
    bullet->addComponent<CBoundingBox>(m_game->assets().getAnimation(m_playerConfig.WEAPON).getSize());
    bullet->getComponent<CTransform>().velocity = {3.0f, 0.0f};
    bullet->addComponent<CLifespan>(90, m_currentFrame);
}

void Scene_Play::update()
{
    if (!m_paused) {
        m_entityManager.update();
        sMovement();
        sCollision();
        sLifespan();
        m_currentFrame++;
        sAnimation();
    }
    sRender();
}

void Scene_Play::sMovement()
{
    // TODO: Implement player movement / jumping based on its CInput component
    // TODO: Implement gravity's effect on the player
    // TODO: Implement the maximum player speed in both X and Y directions
    // TODO: Setting an entity's scale.x to -1/1 will make it face to the left/right

    Vec2 playerVelocity = {0, m_player->getComponent<CTransform>().velocity.y};

    if (m_player->getComponent<CInput>().up && m_player->getComponent<CInput>().canJump) {
        playerVelocity.y = m_playerConfig.JUMP;
        m_player->getComponent<CInput>().canJump = false;
        m_player->getComponent<CState>().state = "AIR";
    }
    if (m_player->getComponent<CInput>().down) {
        playerVelocity.y = -m_playerConfig.JUMP;
    }
    if (m_player->getComponent<CInput>().left) {
        playerVelocity.x = -m_playerConfig.SPEED;
        m_player->getComponent<CTransform>().scale.x = -1.0;
    }
    if (m_player->getComponent<CInput>().right) {
        playerVelocity.x = m_playerConfig.SPEED;
        m_player->getComponent<CTransform>().scale.x = 1.0;
    }   
    
    playerVelocity.y += m_player->getComponent<CGravity>().gravity;
    if (m_player->getComponent<CState>().state == "GROUND") {
        playerVelocity.y = 0;
    }
    
    if (abs(playerVelocity.y) > m_playerConfig.MAXSPEED) {
        playerVelocity.y = m_playerConfig.MAXSPEED * ((playerVelocity.y) / abs(playerVelocity.y));
    }
    if (abs(playerVelocity.x) > m_playerConfig.MAXSPEED) {
        playerVelocity.x = m_playerConfig.MAXSPEED * ((playerVelocity.x) / abs(playerVelocity.x));
    }
    

    m_player->getComponent<CTransform>().prevPos = m_player->getComponent<CTransform>().pos; // save previous position
    
    m_player->getComponent<CTransform>().velocity = playerVelocity;
    m_player->getComponent<CTransform>().pos += playerVelocity;
    
    
    // movement of the box that helps manage state
    auto ppos = m_player->getComponent<CTransform>().pos;
    auto box = m_entityManager.getEntities("box")[0];
    box->getComponent<CTransform>().pos = Vec2(ppos.x, ppos.y - 1); 


    

    for (auto e : m_entityManager.getEntities("bullet")) {
        e->getComponent<CTransform>().pos.x += e->getComponent<CTransform>().velocity.x;
    }
}

void Scene_Play::sLifespan()
{
    // Check lifespan of entities that have them and, and destroy them if they go over
    for (auto e : m_entityManager.getEntities()) {
        if (e->hasComponent<CLifespan>()) {
            if (e->getComponent<CLifespan>().frameCreated + e->getComponent<CLifespan>().lifespan <= m_currentFrame) {
                e->destroy();
            }
        }
    }
}

void Scene_Play::sCollision()
{
    // TODO: REMEMBER: SFML's (0,0) position is on the TOP-LEFT corner
    //       This means jumping will have a negative y-component
    //       and gravity will have a positive y-component
    //       Also, something BELOW something else will have a y value GREATER than it
    //       Also, something ABOVE something else will have a y value LESS than it


    // TODO: Implement bullet / tile collisions
    //       Destroy the tile if it has a Brick animation
    for (auto b : m_entityManager.getEntities("bullet")) {
        for (auto t : m_entityManager.getEntities("tile")) {
            auto overlap = Physics::GetOverlap(b, t);
            if (overlap.x > 0 && overlap.y > 0) {
                b->destroy();
                if (t->getComponent<CAnimation>().animation.getName() == "Brick") {
                    t->destroy();
                }
            }
        }
    }


    // TODO: Implement player / tile collisions and resolutions
    //       Update the CState component of the player to store whether
    //       it is currently on the ground or in the air. This will be
    //       used by the Animation system
    for (auto t : m_entityManager.getEntities("tile")) {
        auto overlap = Physics::GetOverlap(m_player, t);
        auto ppos = m_player->getComponent<CTransform>().pos;
        auto tpos = t->getComponent<CTransform>().pos;
        auto prevOverlap = Physics::GetPreviousOverlap(m_player, t);
        if (overlap.x > 0 && overlap.y > 0) {
            if (prevOverlap.y > 0) { // for collisions on the sides
                if (m_player->getComponent<CTransform>().pos.x > t->getComponent<CTransform>().pos.x) {
                    m_player->getComponent<CTransform>().pos.x += overlap.x;
                } else {
                    m_player->getComponent<CTransform>().pos.x -= overlap.x;
                }
            }
            if (prevOverlap.x > 0) { // for vertical collisions
                if (m_player->getComponent<CTransform>().pos.y > t->getComponent<CTransform>().pos.y) {
                    m_player->getComponent<CTransform>().pos.y += overlap.y;
                    m_player->getComponent<CState>().state = "GROUND"; // todo : when to switch to air
                    m_player->getComponent<CInput>().canJump = true;
                } else {
                    m_player->getComponent<CTransform>().pos.y -= overlap.y;
                    m_player->getComponent<CTransform>().velocity.y = 0;
                }
            }
        }
    }


    // this is a hacky solution to manage player state (ground or air) but i can only think of this for now
    // maybe later move the line that changes position to sMovement
    m_player->getComponent<CState>().state = "AIR";
    for (auto t : m_entityManager.getEntities("tile")) {

        if (m_player) {
            auto ppos = m_player->getComponent<CTransform>().pos;
            auto box = m_entityManager.getEntities("box")[0];
            box->getComponent<CTransform>().pos = Vec2(ppos.x, ppos.y - 1); 
            auto overlap = Physics::GetOverlap(box, t);
            if (overlap.x > 0 && overlap.y > 0) {
                m_player->getComponent<CState>().state = "GROUND";
                m_player->getComponent<CInput>().canJump = true;
            }
        }

        
    }

    
    

    

    

    // TODO: Check to see if the player has fallen down a hole (y > height())
    if (m_player->getComponent<CTransform>().pos.y < 0) {
        m_player->destroy();
        spawnPlayer();
    }
    // TODO: Don't let the player walk off the left side of the map
    if (m_player->getComponent<CTransform>().pos.x - 24 < 0) {
        m_player->getComponent<CTransform>().pos = m_player->getComponent<CTransform>().prevPos;
    }
}

void Scene_Play::sDoAction(const Action &action)
{
    if (action.type() == "START") 
    {
        if       (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if  (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if  (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if  (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if  (action.name() == "QUIT") { onEnd(); }
        else if  (action.name() == "JUMP") { 
            m_player->getComponent<CInput>().up = true;
        }
        else if  (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = true;}
        else if  (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = true;}
        else if  (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = true;}
        else if  (action.name() == "SHOOT" && m_player->getComponent<CInput>().canShoot) { 
            m_player->getComponent<CInput>().shoot = true;
            m_player->getComponent<CInput>().canShoot = false;
            spawnBullet(m_player);
        }
    } else if (action.type() == "END") {
        if  (action.name() == "JUMP") { 
            m_player->getComponent<CInput>().up = false;
        }
        else if  (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = false;}
        else if  (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = false;}
        else if  (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = false;}
        else if  (action.name() == "SHOOT") { 
            m_player->getComponent<CInput>().shoot = false;
            m_player->getComponent<CInput>().canShoot = true;
        }
    }
}

void Scene_Play::sAnimation()
{
    // TODO: Complete the animation class code first

    // TODO: set the animation of the player based on the CState Component
    // TODO: for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
    //       if the animation is not repeated, and it has ended, destroy the entity
}


void Scene_Play::onEnd()
{
    // TODO: When the scene ends, change back to the menu scene
    //       use m_game->changeScene(correct params);
    m_hasEnded = true;
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), false);
}

void Scene_Play::sRender()
{   
    // color the background darker so you know that the game is paused
    if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)); }
    else           { m_game->window().clear(sf::Color(50, 50, 150)); }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto& pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities())
        {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>())
            {
                auto& animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setPosition(transform.pos.x, m_game->window().getSize().y - transform.pos.y);
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }
        }
    }

    // draw all entity collision bounding boxes with a rectangle shape
    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {   
                if (e->tag() == "box") continue;
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x-1, box.size.y-1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, m_game->window().getSize().y - transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    // draw the grid so that it is can easy to debug
    if (m_drawGrid)
    {
        float leftX = m_game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x)
        {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_gridSize.y)
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }
}


