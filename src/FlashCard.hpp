#ifndef FLASHCARD_HPP
#define FLASHCARD_HPP
#include <cmath>

class FlashCard
{
private:
    sf::RectangleShape* cardQ;
    sf::RectangleShape* cardA;
    std::string Q;
    std::string A;
    bool opacity;

public:
    FlashCard(std::string Q, std::string A)
    {
        cardQ = new sf::RectangleShape(sf::Vector2f(100.0f, 50.0f));
        cardA = new sf::RectangleShape(sf::Vector2f(100.0f, 50.0f));
        sf::Vector2f q(50.0f, 25.0f);
        sf::Vector2f a(550.0f, 25.0f);
        cardQ->setOrigin(50.0f, 25.0f);
        cardA->setOrigin(50.0f, 25.0f);
        this->setCardPos('Q', q);
        this->setCardPos('A', a);
        this->setCardColor(sf::Color::White);

        this->Q = Q;
        this->A = A;
        opacity = true;
    }
    FlashCard()
    {
        cardQ = new sf::RectangleShape(sf::Vector2f(100.0f, 50.0f));
        cardA = new sf::RectangleShape(sf::Vector2f(100.0f, 50.0f));
        sf::Vector2f q(50.0f, 25.0f);
        sf::Vector2f a(550.0f, 25.0f);
        cardQ->setOrigin(50.0f, 25.0f);
        cardA->setOrigin(50.0f, 25.0f);
        this->setCardPos('Q', q);
        this->setCardPos('A', a);
        this->setCardColor(sf::Color::White);

        opacity = true;
    }
    ~FlashCard()
    {}
    sf::RectangleShape getCardQ()
    {
        return *cardQ;
    }
    sf::RectangleShape getCardA()
    {
        return *cardA;
    }
    void setA(std::string answer)
    {
        A = answer;
    }
    void setQ(std::string question)
    {
        Q = question;
    }
    std::string getQ()
    {
        return Q;
    }
    std::string getA()
    {
        return A;
    }
    void setCardPos(char card, sf::Vector2f& pos)
    {
        switch (card)
        {
            case 'Q':
                cardQ->setPosition(pos);
                break;
            case 'A':
                cardA->setPosition(pos);
                break;
            default:
                break;
        }
    }

    void setCardColor(sf::Color c)
    {
        cardQ->setFillColor(c);
        cardA->setFillColor(c);
    }

    void setOpacity(bool o)
    {
        if (!o) // if o is false, this is true
        {
            cardQ->setFillColor(sf::Color::Transparent);
            cardA->setFillColor(sf::Color::Transparent);
            opacity = false;
        }
        else if (!opacity && o) // if opacity is already false and o is true
        {
            cardQ->setFillColor(sf::Color::White);
            cardA->setFillColor(sf::Color::White);
            opacity = true;
        }
    }

    void setDefault(sf::Font& font)
    {
        sf::Text question, answer;
        sf::Vector2f q(50.0f, 25.0f);
        sf::Vector2f a(550.0f, 25.0f);
        cardQ->setFillColor(sf::Color::White);
        cardA->setFillColor(sf::Color::White);
        this->setCardPos('Q', q);
        this->setCardPos('A', a);
        setText(question, answer, font);
    }

    bool checkCollision()
    {
        sf::Vector2f halfsize = cardQ->getSize() / 2.0f;
        sf::Vector2f delta(std::abs(cardQ->getPosition().x - cardA->getPosition().x), std::abs(cardQ->getPosition().y - cardA->getPosition().y));
        sf::Vector2f intersect(delta.x - (halfsize.x + halfsize.x), delta.y - (halfsize.y + halfsize.y));
        if (intersect.x <= 0.0f && intersect.y <= 0.0f)
            return true;

        return false;
    }
    void draw(sf::RenderWindow& window, sf::Font& f)
    {
        sf::Text question, answer;
        setText(question, answer, f);

        window.draw(*cardQ);
        window.draw(question);
        window.draw(*cardA);
        window.draw(answer);
    }
    void setText(sf::Text& question, sf::Text& answer, sf::Font& f)
    {
        question.setFont(f);
        answer.setFont(f);
        question.setString(Q);
        answer.setString(A);
        question.setCharacterSize(8);
        answer.setCharacterSize(8);
        question.setFillColor(sf::Color::Black);
        answer.setFillColor(sf::Color::Black);
        question.setOrigin(sf::Vector2f(question.getCharacterSize(), question.getCharacterSize() / 2));
        answer.setOrigin(sf::Vector2f(answer.getCharacterSize(), answer.getCharacterSize() / 2));
        question.setPosition(sf::Vector2f(cardQ->getPosition().x - 25.0f, cardQ->getPosition().y));
        answer.setPosition(sf::Vector2f(cardA->getPosition().x, cardA->getPosition().y));
    }
};

#endif