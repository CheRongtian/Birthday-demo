#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

struct FadingChar {
    sf::Text text;
    int alpha = 0;
};

struct Star {
    sf::ConvexShape shape;
    float frequency;
    float phase;
};

enum EndingState {
    Waiting,
    Entering,
    Shown
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "File 1");

    sf::Music bgm;
    if (!bgm.openFromFile("/Users/cherongtian/Desktop/bgm.ogg")) {
        std::cout << "Failed to load background music!" << std::endl;
    } else {
        bgm.setLoop(true);
        bgm.setVolume(60.f);
        bgm.play();
    }

    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cout << "Font load failed!" << std::endl;
        return -1;
    }

    std::string line1 = "Happy Birthday";
    std::string line2 = "To XX";

    std::vector<FadingChar> letters;
    unsigned int charSize = 50;

    float spacing1 = 5.0f;
    float spacingLetters = 30.0f;
    float spacingSpaces = 25.0f;

    // First Line Width
    float totalWidth1 = 0.0f;
    for (char c : line1) {
        sf::Text temp(std::string(1, c), font, charSize);
        totalWidth1 += temp.getLocalBounds().width + spacing1;
    }
    totalWidth1 -= spacing1;
    float startX1 = (window.getSize().x - totalWidth1) / 2.f;
    float y1 = 200;

    // Second Line Width Estimation
    float totalWidth2 = 0.0f;
    for (char c : line2) {
        totalWidth2 += (c == ' ') ? spacingSpaces : spacingLetters;
    }
    totalWidth2 -= spacingLetters;
    float startX2 = (window.getSize().x - totalWidth2) / 2.f;
    float y2 = 300;

    // First Line
    float x1 = startX1;
    for (char c : line1) {
        FadingChar fc;
        fc.text.setFont(font);
        fc.text.setString(c);
        fc.text.setCharacterSize(charSize);
        fc.text.setFillColor(sf::Color(164, 151, 180, 0));
        fc.text.setPosition(x1, y1);
        x1 += fc.text.getLocalBounds().width + spacing1;
        letters.push_back(fc);
    }

    // Second Line
    float x2 = startX2;
    for (char c : line2) {
        FadingChar fc;
        fc.text.setFont(font);
        fc.text.setString(c);
        fc.text.setCharacterSize(charSize);
        fc.text.setFillColor(sf::Color(164, 151, 180, 0));
        fc.text.setPosition(x2, y2);
        letters.push_back(fc);
        x2 += (c == ' ') ? spacingSpaces : spacingLetters;
    }

    // Star Initialization
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<Star> stars;
    for (int i = 0; i < 100; ++i) {
        Star star;
        star.shape.setPointCount(4);
        star.shape.setPoint(0, sf::Vector2f(0.f, -2.f));
        star.shape.setPoint(1, sf::Vector2f(2.f, 0.f));
        star.shape.setPoint(2, sf::Vector2f(0.f, 2.f));
        star.shape.setPoint(3, sf::Vector2f(-2.f, 0.f));
        star.shape.setFillColor(sf::Color(255, 255, 255, 0));
        float x = static_cast<float>(rand() % window.getSize().x);
        float y = static_cast<float>(rand() % window.getSize().y);
        star.shape.setPosition(x, y);
        star.frequency = 0.2f * (rand() % 100 + 50);
        star.phase = static_cast<float>(rand() % 360);
        stars.push_back(star);
    }

    int currentIndex = 0;
    sf::Clock clock;
    const int fadeSpeed = 5;

    // Final Text
    EndingState endingState = Waiting;
    sf::Text endingText;
    endingText.setFont(font);
    endingText.setString("Wish You Find A Boyfriend :)");
    endingText.setCharacterSize(60);
    endingText.setFillColor(sf::Color(255, 255, 255));
    sf::FloatRect bounds = endingText.getLocalBounds();

    float finalY = 400;
    float currentY = 600;
    endingText.setPosition((window.getSize().x - bounds.width) / 2.f, currentY);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        float time = clock.getElapsedTime().asMilliseconds();

        // Star
        for (auto& star : stars) {
            float a = 127 + 127 * std::sin(star.frequency * time + star.phase);
            star.shape.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(a)));
        }

        // Text Shows
        if (currentIndex < letters.size() && time > 10) {
            FadingChar& fc = letters[currentIndex];
            if (fc.alpha < 255) {
                fc.alpha += fadeSpeed;
                if (fc.alpha > 255) fc.alpha = 255;
                fc.text.setFillColor(sf::Color(164, 151, 180, fc.alpha));
            } else {
                currentIndex++;
                if (currentIndex == letters.size()) {
                    endingState = Entering;
                    clock.restart();
                }
            }
            clock.restart();
        }

        // Slide
        if (endingState == Entering) {
            if (currentY > finalY) {
                currentY -= 0.1f;
                if (currentY <= finalY) {
                    currentY = finalY;
                    endingState = Shown;
                }
            }
        }

        // Final Position
        if (endingState != Waiting) {
            endingText.setPosition((window.getSize().x - bounds.width) / 2.f, currentY);
        }

        // Show
        window.clear(sf::Color(5, 5, 15));
        for (auto& star : stars) window.draw(star.shape);
        for (auto& fc : letters) window.draw(fc.text);
        if (endingState != Waiting) window.draw(endingText);
        window.display();
    }

    return 0;
}
