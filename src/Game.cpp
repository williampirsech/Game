#include "Game.h"

#include <iostream>

Game::Game(void)
    : window()
{}

void Game::Draw(sf::Drawable& d) {
    window.draw(d);
}

void Game::init() 
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(1067,600), "Game",sf::Style::Default,settings);
    window.setFramerateLimit(60);

    if (!font.loadFromFile("resource/font/arialbd.ttf")) {
        //std::cout << "Failed to load font" << std::endl;
    }

    if (!directionTexture.loadFromFile("resource/img/arrow.png")) {
        //std::cout << "Failed to load texture" << std::endl;
    }

    if (!backgroundTexture.loadFromFile("resource/skybox/3.png")) {
        //std::cout << "Failed to load texture" << std::endl;
    }
    background.setTexture(backgroundTexture);
}

void Game::run() {
    //Just a demo
    sf::Clock clock;

    auto sun = MovableCreator::planetMaker(40.f,20,sf::Color::White);
    auto planet = MovableCreator::planetMaker(5.f,20,sf::Color::White);
    auto player = MovableCreator::create("spaceship.xml");
    
    player.setPosition(0.35,0.35);
    player.setVelocity(0.1,0);

    sun.setMass(3.f);
    sun.setPosition(100,100);
    sun.setVelocity(0,0);
    
    planet.setMass(0.2f);
    planet.setPosition(50,50);
    planet.setVelocity(0.1,-0.1);

    std::cout << window.getSize().x << "," << window.getSize().y  << std::endl;
    
    auto viewSize = sf::Vector2f(window.getSize());
    auto viewRatio = viewSize.x/viewSize.y;
    const size_t viewIncrement = 5;
    
    player.collidableState = CollidableState::Rigid;
    sun.collidableState = CollidableState::Rigid;
    planet.collidableState = CollidableState::Rigid;

    dynamics.add(player);
    dynamics.add(sun);
    dynamics.add(planet);

    sf::View gameView(player.getPosition(),viewSize);
    sf::View directionView(player.getPosition(), sf::Vector2f(100,100));
    directionView.setViewport(sf::FloatRect(0.8f,0.8f,0.2f,0.2f));

    background.setScale(window.getSize().x/background.getGlobalBounds().width,window.getSize().y/background.getGlobalBounds().height);
    FuelMeter fuelmeter(window);
    fuelmeter.show();
    Minimap minimap(window,player,directionView);
    minimap.show();

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            switch(e.type) {
                case sf::Event::Closed: window.close(); break;
                default: break;
            }
        }

        auto dt = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                player.accelerate(dt,0.08);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player.rotate(dt,-360);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.rotate(dt,+360);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
            viewSize.x += viewRatio*viewIncrement;
            viewSize.y += viewIncrement;
            gameView.setSize(viewSize);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
                viewSize.x -= viewRatio*viewIncrement;
                viewSize.y -= viewIncrement;
                gameView.setSize(viewSize);
        }

        window.clear(sf::Color::Black);
        
        gameView.setCenter(player.getPosition());
        
        window.setView(window.getDefaultView());
        
        window.draw(background);
        fuelmeter.update();
        window.draw(fuelmeter);
        
        window.setView(directionView);
        minimap.update();
        window.draw(player.getVertexArray());

        dynamics.incrementSystem(dt);

        window.setView(gameView);
        
        for (const auto& p : dynamics.getMovables()) {
            window.draw(p->getVertexArray());
        }
        //window.draw(triangle);
        window.display();
    }
}