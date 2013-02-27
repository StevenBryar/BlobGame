#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "InputManager.h"
#include "constants.h"
#include "InputListener.h"
#include "Renderer.h"
#include "Camera.h"
#include "SpriteManager.h"
#include "GameObject.h"
#include "2dSprite.h"
#include "BlobGame.h"
#include "Util.h"

void updateMouse(sf::Window* window);

int main(){
	sf::RenderWindow* window = new sf::RenderWindow
		(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT),WINDOW_NAME);
	window->setFramerateLimit(60);
	sf::Clock theClock;
	double lastUpdateTime = theClock.getElapsedTime().asSeconds();
	double delta = 0.0f;
	bool focus = true;
	BlobGame::instance()->getCamera()->setView
		(new sf::View(sf::FloatRect(0,0,WINDOW_WIDTH,WINDOW_HEIGHT)));
	//Game loop
	while(window->isOpen()){
		Util::instance()->update();
		delta = theClock.getElapsedTime().asSeconds() - lastUpdateTime;
		lastUpdateTime = theClock.getElapsedTime().asSeconds();
		sf::Event event;
		InputManager::instance()->update();
		while(window->pollEvent(event)){
			switch(event.type){
			case sf::Event::Closed:
				SpriteManager::instance()->cleanupInstance();
				InputManager::instance()->cleanUpInstance();
				BlobGame::instance()->cleanupInstance();
				Renderer::instance()->cleanupInstance();
				window->close();
				return 0;
			case sf::Event::LostFocus:
			case sf::Event::GainedFocus:
				focus = !focus;
				break;
			case sf::Event::MouseMoved:
				if(focus){
					InputManager::instance()->handleMouseInput
						(MOUSE_MOVED,sf::Mouse::getPosition(*window).x,
						sf::Mouse::getPosition(*window).y);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if(focus){
					if(event.mouseButton.button == sf::Mouse::Left){
						InputManager::instance()->handleMouseInput
						(MOUSE_LB_RELEASED,sf::Mouse::getPosition(*window).x,
						sf::Mouse::getPosition(*window).y);
					}
					else if(event.mouseButton.button == sf::Mouse::Right){
						InputManager::instance()->handleMouseInput
						(MOUSE_RB_RELEASED,sf::Mouse::getPosition(*window).x,
						sf::Mouse::getPosition(*window).y);
					}
				}
				break;
			case sf::Event::MouseButtonPressed:
				if(focus){
					if(event.mouseButton.button == sf::Mouse::Left){
						InputManager::instance()->handleMouseInput
							(MOUSE_LB_PRESSED,sf::Mouse::getPosition(*window).x,
							sf::Mouse::getPosition(*window).y);
					}
				}
				break;
			case sf::Event::KeyReleased:
				if(focus){
					if(event.key.code != sf::Keyboard::Escape){
						InputManager::instance()->handleKeyInput(KEY_RELEASED,(keyType)event.key.code);
					}
					else{
						SpriteManager::instance()->cleanupInstance();
						InputManager::instance()->cleanUpInstance();
						BlobGame::instance()->cleanupInstance();
						Renderer::instance()->cleanupInstance();
						window->close();
						return 0;
					}
				}
			}
		}
		BlobGame::instance()->update();
		SpriteManager::instance()->update();
		Renderer::instance()->render(window,BlobGame::instance()->getCamera());
		if(focus){
		updateMouse(window);
		}
	}
	return 0;
}

void updateMouse(sf::Window* window){
		if(sf::Mouse::getPosition(*window).x < 0){
			sf::Mouse::setPosition(sf::Vector2i(0,sf::Mouse::getPosition(*window).y),(*window));
		}
		else if(sf::Mouse::getPosition(*window).x > BlobGame::instance()->getCamera()->getWidth()){
			sf::Mouse::setPosition(sf::Vector2i(BlobGame::instance()->getCamera()->getWidth()-2,
				sf::Mouse::getPosition(*window).y),(*window));
		}
		if(sf::Mouse::getPosition(*window).y < 0){
			sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition(*window).x,0),(*window));
		}
		else if(sf::Mouse::getPosition(*window).y > BlobGame::instance()->getCamera()->getHeight()){
			sf::Mouse::setPosition(sf::Vector2i(sf::Mouse::getPosition(*window).x,
				BlobGame::instance()->getCamera()->getHeight()-2),(*window));
		}
}
