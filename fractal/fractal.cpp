#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

sf::Vector2f operator/(const sf::Vector2f& left, const sf::Vector2f& right) {
    return sf::Vector2f(left.x / right.x, left.y / right.y);
}

sf::Vector2f operator*(const sf::Vector2f& left, const sf::Vector2f& right) {
    return sf::Vector2f(left.x * right.x, left.y * right.y);
}

sf::Vector2f operator*(const sf::Vector2f& left, double c) {
    return sf::Vector2f(left.x * c, left.y * c);
}

const char* texName = "texture.png";
const char* shaderName = "shader.frag";
const int size = 500;

int main() {
    sf::RenderWindow window(sf::VideoMode(size, size), "Test window", 0);
    sf::Texture texture;

    if(!texture.loadFromFile(texName)) {
        std::cout << "Error: can not load texture from the file. Exit." << std::endl;
        return 1;
    }

    if(!sf::Shader::isAvailable()) {
        std::cout  << "Error: shaders is not avaliable on this system. Exit." << std::endl;
        return 1;
    }

    sf::Shader shader;
    if(!shader.loadFromFile(shaderName, sf::Shader::Fragment)) {
        std::cout << "Error: can not load shader from the file. Exit." << std::endl;
        return 1;
    }

    sf::Vector2f camera(-1.8, -1.5);
    sf::Vector2f screen(size, size);
    float zoom = 3.0;

    shader.setParameter("tex", texture);
    shader.setParameter("screen", screen);


    sf::RectangleShape shape(sf::Vector2f(size, size));

    sf::Vector2f pos(0, 0);
    bool pressed = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch(event.type) {
            case sf::Event::Closed:
                std::cout << "Bye!" << std::endl;
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                pressed = true;
                break;
            case sf::Event::MouseMoved:
                if(pressed) {
                    float x = event.mouseMove.x;
                    float y = event.mouseMove.y;
                    float dx = (x - pos.x) * zoom / screen.x;
                    float dy = (y - pos.y) * zoom / screen.y;
                    camera -= sf::Vector2f(dx, dy);
                }
                pos.x = event.mouseMove.x;
                pos.y = event.mouseMove.y;
                break;
            case sf::Event::MouseButtonReleased:
                pressed = false;
                break;
            case sf::Event::MouseWheelMoved:
                pos.x = event.mouseWheel.x;
                pos.y = event.mouseWheel.y;
                camera -= ((pos / screen) * zoom * (event.mouseWheel.delta > 0 ? -0.1 : 0.1));
                zoom *= (event.mouseWheel.delta > 0 ? 0.9 : 1.1);
                break;
            default:
                break;
            }
        }
        shader.setParameter("camera", camera);
        shader.setParameter("zoom", zoom);
        window.clear(sf::Color::White);
        window.draw(shape, &shader);
        window.display();
    }

    return 0;
}

