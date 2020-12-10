#include <iostream>
#include <sstream>

#define DELETE 8
#define ENTER 13
#define ESCAPE 27
#define NEWLINE 10

class TextBox
{
private:
    sf::Text textbox;
    std::ostringstream text;
    bool isSelected = false;
    bool hasLimit = false;
    int limit;

    void inputLogic(int charTyped)
    {
        if (charTyped != DELETE && charTyped != ENTER && charTyped != ESCAPE)
        {
            text << static_cast<char>(charTyped);
        }
        else if (charTyped == DELETE)
        {
            if (text.str().length() > 0)
            {
                deleteLast();
            }
        }
        textbox.setString(text.str() + "_");
    }

    void deleteLast()
    {
        std::string t = text.str();
        if (!t.empty() && t[t.length() - 1] == '\n')
        {
            t.erase(t.length() - 1);
        }

        t.pop_back();
        text.str("");
        text << t;

        textbox.setString(text.str());
    }

public:
    TextBox()
    {
    }

    TextBox(int size, sf::Color color, bool sel)
    {
        textbox.setCharacterSize(size);
        textbox.setFillColor(color);
        isSelected = sel;
        if (sel)
        {
            textbox.setString("_");
        }
        else
        {
            textbox.setString("");
        }
    }

    void setFont(sf::Font& font)
    {
        textbox.setFont(font);
    }

    void setpos(sf::Vector2f pos)
    {
        textbox.setPosition(pos);
    }

    void setLimit(bool ToF)
    {
        hasLimit = ToF;
    }

    void setLimit(bool ToF, int lim)
    {
        hasLimit = ToF;
        limit = lim;
    }

    void setSelected(bool sel)
    {
        isSelected = sel;
        if (!sel)
        {
            std::string t = text.str();
            std::string newT = "";
            for (long unsigned int i = 0; i < t.length(); i++)
            {
                newT += t[i];
            }
            textbox.setString(newT);
        }
    }

    std::string getText()
    {
        return text.str();
    }

    void drawTo(sf::RenderWindow& window)
    {
        window.draw(textbox);
    }

    void typedOn(sf::Event input)
    {
        if (isSelected)
        {
            int charTyped = input.text.unicode;
            if (charTyped < 128)
            {
                if (hasLimit)
                {
                    if ((int)text.str().length() <= limit)
                    {
                        inputLogic(charTyped);
                    }
                    else if (charTyped == DELETE)
                    {
                        deleteLast();
                    }
                }
                else
                {
                    inputLogic(charTyped);
                }
            }
        }
    }

    void wrap(float p)
    {
        if (textbox.findCharacterPos(text.str().length()).x >= p)
        {
            std::string t = text.str();
            size_t space = t.find_last_of(' ');

            if (space != std::string::npos)
            {
                t.insert(space + 1, "\n");
            }
            else
                t.push_back('\n');
            text.str("");
            text << t;
            textbox.setString(text.str());
        }
    }

    void clear()
    {
        text.clear();
        textbox.setString(text.str());
        isSelected = false;
    }
};