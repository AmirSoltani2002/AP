#ifndef BOARD_H
#define BOARD_H
#include "string"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "user.h"
#include "data.h"
#include "cell.h"
#include "setting.h"
#include "helper.h"
#include "button.h"
#include "fstream"
using std::ifstream;
class Board
{
public:
    Board(sf::RenderWindow* _window);
    sf::RenderWindow* window;
    Cells cells;
    User* user_w;
    User* user_b;
    User* curr_user;
    Piece* curr_piece;
    sf::Font font;
    bool end;
    bool checked;
    sf::Text status_text;
    vector <string> dPos;
    vector <string> aPos;
    static vector<Piece*> pieces;
    static vector<Piece*> deleted;
    sf::SoundBuffer buf;
    sf::Sound sound;
    Button button;
    void init();
    void draw();
    void run();
    void mouse_clicked(const sf::Vector2i& position);
    void cell_occupied_clicked(const int&row, const int&column);
    void cell_empty_clicked(int row, int column);
    void put_Piece_in_cell(int row, int column);
    void check_end();
    void update_status_text();
    bool move(const int&x, const int&y, Piece* piece, bool movePiece = 1);
    bool reach(const int&a, const int&b, const int& type, Piece* piece);
    bool check(Piece* piece);
    void initPiece(string&);
    void green(Piece*);
    void red(Piece*);
    void blue(Piece*);
    Piece* find_king(const char&);
    bool mate(Piece* piece);
    bool inGame(Piece* piece);
    string num2str(int&i, int&j);
    bool exist(const int&x, const int&y);
    Piece* find(const int&x, const int&y);
    void sort(vector <string>& list);
    bool checkIn(int&i, int&j, Piece& piece);
    bool checkFull(Piece& piece);
    void dangerPos(const char& color);
    void attackPos(const char& color);
    void show(const char& type);
    void undo(int x1, int y1, int x2, int y2, Piece* p1, Piece* changed);
    bool check_win(const char&color);
    void check_checked(const char&color);
    void defence(const char& color);
    void attack(const char& color);
    bool AA(Piece* minor, Piece* king, int level, Piece* starter = 0);
    bool BB(Piece* minor, Piece* king, const int& level, Piece* starter = 0);
};

#endif

#ifndef BUTTON_H
#define BUTTON_H
#include "SFML/Graphics.hpp"
#include "string"
#include "setting.h"
using namespace std;
using namespace sf;
class Button
{
public:
    sf::RectangleShape exitButtonImage;
    sf::Font font;
    sf::Text startText;
    string directory;
    float exitButtonHeight;
    float exitButtonWidth;
    Button()
    {
        exitButtonImage.setSize(sf::Vector2f(setting::cell_size, setting::cell_size));
        exitButtonImage.setFillColor(sf::Color(168, 168, 10, 80));
        exitButtonImage.setPosition(setting::board_x-100, 0);
        font.loadFromFile("resources/fonts/roboto.ttf");
        exitButtonWidth = exitButtonImage.getLocalBounds().width;
        exitButtonHeight = exitButtonImage.getLocalBounds().height;
    }
    void start()
    {
        startText.setFont( font );
        startText.setStyle( sf::Text::Bold );
        startText.setString("Reset Map");
        startText.setFillColor( sf::Color::White );
        startText.setCharacterSize( 20 );
        startText.setPosition(setting::board_x-95, 35);
    }
};
#endif
#ifndef CELL_H
#define CELL_H

#include "vector"
#include "data.h"
#include "Piece.h"

class Cell
{
public:
    sf::RectangleShape rect;
    CellStatus cell_status = EMPTY;
    Piece* piece;
};

typedef typename std::vector<std::vector<Cell>> Cells;


#endif
#ifndef HELPER_H
#define HELPER_H

#include "SFML/Graphics.hpp"
#include "setting.h"
#include "data.h"
using namespace std;


sf::Vector2f get_cell_position(int row,int column);

string get_Piece_path(const char&, const char&);

int get_cell_index(int x);

string get_name_of_id(char color);

#endif
#ifndef Piece_H
#define Piece_H

#include <SFML/Graphics.hpp>
#include "helper.h"
#include "setting.h"
class Piece
{
    public:
        sf::Sprite sprite;
        vector<vector<int>> danger;
        vector<vector<int>> attacking;
        char type;
        int x;
        int y;
        char color;
        int moveNum;
        Piece(const int &x,const int &y);
        void move(const int&x, const int&y);
        virtual void setMoveNum();
        void load_texture();
};
class Pawn: public Piece
{
    public:
        void setMoveNum() override;    
        Pawn(const int &x,const int &y, const char& col);
};
class Rook: public Piece
{
    public:
        void setMoveNum() override;
        Rook(const int &x,const int &y, const char& col);            
};
class Knight: public Piece
{
    public:
        void setMoveNum() override;
        Knight(const int &x,const int &y, const char& col);    
};
class King: public Piece
{
    public:
        void setMoveNum() override;
        King(const int &x,const int &y, const char& col);     
};
class Bishop: public Piece
{
    public:
        void setMoveNum() override;
        Bishop(const int &x,const int &y, const char& col);     
};
class Queen: public Piece
{
    public:
        void setMoveNum() override;
        Queen(const int &x,const int &y, const char& col);     
};
#endif
#ifndef SETTING_H
#define SETTING_H

#include "SFML/Graphics.hpp"

struct setting
{
    static int board_x;
    static int board_y;
    static int cell_size;
    static int cell_offset;
    static sf::Color cell_color;
    static int line_space;
};

#endif
#ifndef USER_H
#define USER_H

#include "data.h"
#include "cell.h"

class User
{
public:
    User(const char& _color);
    char color;
};

#endif
#include "board.h"
#include "iostream"
#include <SFML/Audio.hpp>
#include "button.h"
#include <stdlib.h>
using sf::String;
using sf::Text;
Board::Board(sf::RenderWindow* _window) : window(_window)
{
    this->user_w = new User('W');
    this->user_b = new User('B');
    this->window->setFramerateLimit(60);
}
bool Board::move(const int&x, const int&y, Piece* piece, bool movePiece)
{
    piece->setMoveNum();
    for(int i = 0; i<piece->moveNum; i++)
    {
        bool r = reach(x, y, i, piece);
        if(r)
        {
            if(movePiece)
            {
                if(exist(x, y))
                {
                    Piece* p = find(x, y);
                    deleted.insert(deleted.begin(), p);
                }
                piece->move(x, y);
            }
            return true;
        }
    }
    return false;
}
void Board::init()
{
    end = false;
    this->cells.resize(8);
    for (int row = 0; row < 8; row++)
    {
        this->cells[row].resize(8);
        for (int column = 0; column < 8; column++)
        {
            this->cells[row][column].rect.setSize(sf::Vector2f(setting::cell_size, setting::cell_size));
            this->cells[row][column].rect.setFillColor(setting::cell_color);
            this->cells[row][column].rect.setPosition(get_cell_position(row, column));
        }
    }
    string temp;
    string res;
    ifstream inp;
    inp.open("resources/map/map.txt");
    while (inp>>temp)
    {
        res += temp;
    }
    initPiece(res);
    ////initPiece();
    this->curr_user = this->user_w;
    font.loadFromFile("resources/fonts/roboto.ttf");
    status_text.setFont(font);
    status_text.setCharacterSize(60);
    status_text.setStyle(sf::Text::Regular);
    status_text.setFillColor(sf::Color(10, 10, 128, 200));
    status_text.setPosition(setting::board_x-20, setting::board_y/2 - status_text.getCharacterSize()*3);
    status_text.setRotation(status_text.getRotation() + 90);
    this->update_status_text();
}
void Board::initPiece(string& text)
{
    int i, row, col;
    i = row = col = 0;
    while (row<8)
    {
        if(text[i] == '-')
        {
            i += 2;
            col++;
            
        }
        else
        {
            if(text[i] == 'P')
            {
                this->cells[row][col].piece = new Pawn(row, col, text[i+1]);
            }
            else if(text[i] == 'Q')
            {
                this->cells[row][col].piece = new Queen(row, col, text[i+1]);
            }
            else if(text[i] == 'N')
            {
                this->cells[row][col].piece = new Knight(row, col, text[i+1]);
            }
            else if(text[i] == 'B')
            {
                this->cells[row][col].piece = new Bishop(row, col, text[i+1]);
            }
            else if(text[i] == 'R')
            {
                this->cells[row][col].piece = new Rook(row, col, text[i+1]);
            }
            else if(text[i] == 'K')
            {
                this->cells[row][col].piece = new King(row, col, text[i+1]); 
            }
            pieces.push_back(this->cells[row][col].piece);
            this->cells[row][col].cell_status = OCCUPIED;
            this->cells[row][col].piece->sprite.setPosition(this->cells[row][col].rect.getPosition());
            i += 2;
            col++;
        }
        if(col == 8)
        {
            col = 0;
            row++;
        }
    }
}
void Board::draw()
{
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            this->window->draw(this->cells[row][column].rect);
            if (this->cells[row][column].cell_status == OCCUPIED)
                this->window->draw(this->cells[row][column].piece->sprite);
        }
    }
    this->window->draw(this->status_text);
}

void Board::run()
{
    sf::Texture texture;
    texture.loadFromFile("resources/images/empty-wooden-chessboard.jpg");
    sf::Sprite background(texture);
    float piece_scale_x = (float)(setting::board_x-100) / background.getTexture()->getSize().x;
    float piece_scale_y = (float)(setting::board_y) / background.getTexture()->getSize().y;
    background.setScale(piece_scale_x, piece_scale_y);
    setting::cell_size = (setting::board_x - 170)/8;
    sf::Texture t;
    t.loadFromFile("resources/images/marmar.png");
    sf::Sprite table(t);
    piece_scale_x = (float)(setting::board_x) / table.getTexture()->getSize().x;
    piece_scale_y = (float)(setting::board_y) / table.getTexture()->getSize().y;
    table.setScale(piece_scale_x, piece_scale_y);
    this->init();
    defence(curr_user->color);
    attack(curr_user->color);
    button.start();
    this->window->display();
    while (this->window->isOpen()) {
        sf::Event event;
        while (this->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->window->close();
            }
            if(event.type == sf::Event::MouseMoved)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition( *window );
                sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                if ( button.exitButtonImage.getGlobalBounds().contains( mousePosF ) )
                {
                    button.exitButtonImage.setFillColor( sf::Color(168, 10, 10, 200) );
                }
                else
                {
                    button.exitButtonImage.setFillColor( sf::Color(10, 10, 10, 80) );
                }
            }
            if (!this->end && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                this->mouse_clicked(sf::Mouse::getPosition(*(this->window)));
            }
        }
        window->clear(sf::Color::White);
        window->draw(table);
        window->draw(background);
        window->draw( button.exitButtonImage );
        window->draw( button.startText );
        this->update_status_text();
        this->draw();
        check_checked(curr_user->color);
        this->window->display();
    }
}

void Board::mouse_clicked(const sf::Vector2i& position)
{   
    int row = get_cell_index(position.y), column = get_cell_index(position.x);
    sf::Vector2f mousePosF( static_cast<float>( position.x ), static_cast<float>( position.y ) );
    if ( button.exitButtonImage.getGlobalBounds().contains( mousePosF ) )
    {
        for(int r = 0; r<8; r++)
        {
            for(int c = 0; c<8; c++)
            {
                if(this->cells[r][c].cell_status == OCCUPIED)
                {
                    delete this->cells[r][c].piece;
                    this->cells[r][c].piece = 0;
                    this->cells[r][c].cell_status = EMPTY;
                }

            }
        }
        curr_user = user_w;
        pieces = {};
        deleted = {};
        string res;
        string temp;
        ifstream inp;
        inp.open("resources/map/map.txt");
        while (inp>>temp)
        {
            res += temp;
        }
        initPiece(res);
        defence(curr_user->color);
        attack(curr_user->color);
    }
    if (this->cells[row][column].cell_status == EMPTY || curr_user->color != cells[row][column].piece->color)
    {
        this->cell_empty_clicked(row, column);
    }
    else
    {
        this->cell_occupied_clicked(row, column);
    }
}
void Board::cell_occupied_clicked(const int&row, const int&column)
{
    if(curr_user->color == cells[row][column].piece->color)
    {
        curr_piece = cells[row][column].piece;
        green(curr_piece);
        blue(curr_piece);
        red(curr_piece);
    }
    else
    {
        for(int row = 0; row<8; row++)
        {
            for(int column = 0; column<8; column++)
            {
                cells[row][column].rect.setFillColor(setting::cell_color);
            }
        }
    }
}

Piece* Board::find_king(const char& color)
{
    for(int i = 0; i<pieces.size(); i++)
    {
        if(pieces[i]->type == 'K' && pieces[i]->color == color)
        {
            return pieces[i];
        }
    }
}
void Board::green(Piece* piece)
{
    for(int row = 0; row<8; row++)
    {
        for(int column = 0; column<8; column++)
        {
            cells[row][column].rect.setFillColor(setting::cell_color);
        }
    }
    Piece* king = find_king(piece->color);
    for(int j = 0; j<8; j++)
    {
        for(int k = 0; k<8; k++)
        {
            int x1 = (*piece).x;
            int y1 = (*piece).y;
            Piece* p1 = deleted.size()!=0?deleted[0]:0;
            if(move(j, k, piece))
            {
                Piece* p2 = deleted.size()!=0?deleted[0]:0;
                Piece* p3 = p2!=p1 ? p2:0;
                if(!check(king))
                {
                    undo(x1, y1, j, k, piece, p3);
                    if(this->cells[j][k].cell_status == OCCUPIED)
                    {
                       this->cells[j][k].rect.setFillColor(sf::Color(0,128,0, 200));
                    }
                    else if(this->cells[j][k].cell_status == EMPTY)
                    {
                        this->cells[j][k].rect.setFillColor(sf::Color(10, 255, 10, 80));
                    }
                }
                else
                {
                    undo(x1, y1, j, k, piece, p3);
                }
            }
        }
    }
}
void Board::red(Piece* piece)
{
    for(int i = 0; i<piece->danger.size(); i++)
    {
        int r = piece->danger[i][0];
        int c = piece->danger[i][1];
        if(cells[r][c].rect.getFillColor() == sf::Color(10, 255, 10, 80) || cells[r][c].rect.getFillColor() == sf::Color(0,128,0, 200))
            this->cells[r][c].rect.setFillColor(sf::Color(255, 10, 10, 80));
    } 
}
void Board::blue(Piece* piece)
{
    for(int i = 0; i<piece->attacking.size(); i++)
    {
        int r = piece->attacking[i][0];
        int c = piece->attacking[i][1];
        if(cells[r][c].rect.getFillColor() == sf::Color(10, 255, 10, 80) || cells[r][c].rect.getFillColor() == sf::Color(0,128,0, 200))
            this->cells[r][c].rect.setFillColor(sf::Color(10, 10, 255, 80));
    } 
}
void Board::cell_empty_clicked(int row, int column)
{
    if(cells[row][column].rect.getFillColor() ==sf::Color(10, 10, 255, 80) || cells[row][column].rect.getFillColor() == sf::Color(0,128,0, 200) || cells[row][column].rect.getFillColor() == sf::Color(10, 255, 10, 80) || cells[row][column].rect.getFillColor() == sf::Color(255, 10, 10, 80) || cells[row][column].rect.getFillColor() == sf::Color(10, 255, 10, 80))
    {
        for(int row = 0; row<8; row++)
        {
            for(int column = 0; column<8; column++)
            {
                cells[row][column].rect.setFillColor(setting::cell_color);
            }
        }
        put_Piece_in_cell(row, column);
        this->end = check_win(this->curr_user->color);
        if (this->end)
        {
            buf.loadFromFile("resources/music/victory.ogg");
            sound.setBuffer(buf);
            sound.setVolume(100);
            sound.play();
            Piece* king = find_king(curr_user->color == 'W'? 'B':'W');
            int row = king->x;
            int column = king->y;
            this->cells[row][column].rect.setFillColor(sf::Color(10, 164, 164, 80));
            return;
        }
        this->curr_user = curr_user == user_w? user_b:user_w;
        for(int p = 0; p<pieces.size(); p++)
        {
            pieces[p]->danger = {};
            pieces[p]->attacking = {};
        }
        defence(curr_user->color);
        attack(curr_user->color);
    }
    else
    {
        for(int row = 0; row<8; row++)
        {
            for(int column = 0; column<8; column++)
            {
                cells[row][column].rect.setFillColor(setting::cell_color);
            }
        }
    }
    curr_piece = 0;
}
bool Board::check_win(const char&color)
{
    Piece* king = find_king(color == 'W'? 'B':'W');
    if(mate(king))
    {
        return true;
    }
    else
    {
        return false;
    }
}
void Board::check_checked(const char& color)
{
    Piece* king = find_king(color);
    const int &row = king->x;
    const int &column = king->y;
    static int counter =  0;
    if(check(king))
    {
        counter++;
        if(counter == 1)
        {
            buf.loadFromFile("resources/music/check.ogg");
            sound.setBuffer(buf);
            sound.setVolume(100);
            sound.play();
        }
        this -> cells[row][column].rect.setFillColor(sf::Color(164, 20, 164, 80));
    }
    else
    {
        counter = 0;
        this -> cells[row][column].rect.setFillColor(setting::cell_color);
    }
}
void Board::put_Piece_in_cell(int row, int column)
{
    if(this->cells[row][column].cell_status == OCCUPIED)
    {
        deleted.insert(deleted.begin(), this->cells[row][column].piece);
        buf.loadFromFile("resources/music/remove.ogg");
        sound.setBuffer(buf);
        sound.setVolume(100);
        sound.play();
    }
    else
    {
        this->cells[row][column].cell_status = OCCUPIED;
        buf.loadFromFile("resources/music/move.ogg");
        sound.setBuffer(buf);
        sound.setVolume(100);
        sound.play();
    }
    int r = curr_piece->x;
    int c = curr_piece->y;
    this->cells[r][c].cell_status = EMPTY;
    this->cells[r][c].piece = 0;
    curr_piece->sprite.setPosition(this->cells[row][column].rect.getPosition());
    this->cells[row][column].piece = curr_piece;
    curr_piece->x = row;
    curr_piece->y = column;
}
void Board::update_status_text()
{
    if (this->end)
        status_text.setString(get_name_of_id(this->curr_user->color) + " Wins!");
    else
        status_text.setString(get_name_of_id(this->curr_user->color) + " Turn");
}
bool Board::reach(const int&a, const int&b, const int& type, Piece* piece)
{
    const int& x = piece->x;
    const int& y = piece->y;
    const char& color = piece->color;
    const int& moveNum = piece->moveNum;
    if(piece->type == 'P')
    {
        if(moveNum == 2)
        {
            if(color == 'W')
            {
                if(type == 0)
                {
                    if(a==x-1 && b == y && !exist(a, b))
                        return true;
                    else if((a == x-1 && b == y-1) && exist(a, b))
                    {
                        if((*find(a, b)).color != (*find(x, y)).color)
                        {
                            return true;
                        }
                    }
                    else if((a == x-1 && b == y+1) && exist(a, b))
                    {
                        if((*find(a, b)).color != (*find(x, y)).color)
                        {
                            return true;
                        }
                    }
                }
                else if(type == 1)
                    if(a==x-2 && y == b && !exist(a, b))
                    {
                        for(int i = 1; i<2; i++)
                        {
                            if(exist(x-i, y))
                                return false;
                        }
                        return true;
                    }
            }
            else
            {
                if(type == 0)
                {
                    if(a==x+1 && b == y && !exist(a, b))
                        return true;
                    else if((a == x+1 && b == y+1) && exist(a, b))
                    {
                        if((*find(a, b)).color != (*find(x, y)).color)
                        {
                            return true;
                        }
                    }
                    else if((a == x+1 && b == y-1)&& exist(a, b))
                    {
                        if((*find(a, b)).color != (*find(x, y)).color)
                        {
                            return true;
                        }
                    }
                }
                else if(type == 1)
                    if(a==x+2 && y == b && !exist(a, b))
                    {
                        for(int i = 1; i<2; i++)
                        {
                            if(exist(x+i, y))
                                return false;
                        }
                        return true;
                    }
            }
        }
        else
        {
            if(color == 'W')
            {
                if(a==x-1 && b == y && !exist(a, b))
                    return true;
                else if((a == x-1 && b == y-1) &&  exist(a, b))
                {
                    if((*find(a, b)).color != (*find(x, y)).color)
                    {
                        return true;
                    }
                }
                else if((a == x-1 && b == y+1) && exist(a, b))
                {
                    if((*find(a, b)).color != (*find(x, y)).color)
                    {
                        return true;
                    }
                }
            }
            else
            {
                if(a==x+1 && b == y && !exist(a, b))
                    return true;
                else if((a == x+1 && b == y+1) && exist(a, b))
                {
                    if((*find(a, b)).color != (*find(x, y)).color)
                    {
                        return true;
                    }
                }
                else if((a == x+1 && b == y-1) && exist(a, b))
                {
                    if((*find(a, b)).color != (*find(x, y)).color)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    else if(piece->type == 'R')
    {
        switch (type)
        {
        case 0:
            if(a==x && b>y)
            {
                for(int i = 1; i<b-y; i++)
                {
                    if(exist(x, y+i))
                        return false;
                }
                if(exist(a, b) && (*find(a, b)).color == color)
                    return false;
                return true;
            }
            return false;
        
        case 1:
            if(a==x && b<y)
            {
                for(int i = 1; i<y-b; i++)
                {
                    if(exist(x, y-i))
                        return false;
                }
                if(exist(a, b) && (*find(a, b)).color == color)
                    return false;
                return true;
            }
            return false;
        case 2:
            if(b==y && a>x)
            {
                for(int i = 1; i<a-x; i++)
                {
                    if(exist(x+i, y))
                        return false;
                }
                if(exist(a, b) && (*find(a, b)).color == color)
                    return false;
                return true;
            }
            return false;
        case 3:
            if(b==y && a<x)
            {
                for(int i = 1; i<x-a; i++)
                {
                    if(exist(x-i, y))
                        return false;
                }
                if(exist(a, b) && (*find(a, b)).color == color)
                    return false;
                return true;
            }
            return false;
        }
    }
    else if(piece->type == 'N')
    {
        switch (type)
        {
        case 0:
            if(x+2==a && y+1==b && (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;   
        case 1:
            if(x+2==a && y-1==b && (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false; 
        case 2:
            if(x+1==a && y+2==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false; 
        case 3:
            if(x+1==a && y-2==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false; 
        case 4:
            if(x-1==a && y+2==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;
        case 5:
            if(x-1==a && y-2==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;
        case 6:
            if(x-2==a && y+1==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;
        case 7:
            if(x-2==a && y-1==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;
        
        }
    }
    else if(piece->type == 'K')
    {
        switch (type)
        {
        case 0:
            if(x+1==a && y+1==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;   
        case 1:
            if(x+1==a && y==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false; 
        case 2:
            if(x+1==a && y-1==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false; 
        case 3:
            if(x==a && y-1==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false; 
        case 4:
            if(x-1==a && y-1==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;
        case 5:
            if(x-1==a && y==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;
        case 6:
            if(x-1==a && y+1==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;
        case 7:
            if(x==a && y+1==b&& (!exist(a, b) || (*find(a, b)).color != color))
                return true;
            return false;
        
        }
    }
    else if(piece->type == 'B')
    {
        switch (type)
        {
        case 0:
            if((a>x && b>y) && (a-x == b-y))
            {
                for(int i = 1; i<a-x; i++)
                {
                    if(exist(x+i, y+i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        
        case 1:
            if((a>x && b<y) && (a-x == y-b))
            {
                for(int i = 1; i<a-x; i++)
                {
                    if(exist(x+i, y-i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        case 2:
            if((a<x && b<y) && (a-x == b-y))
            {
                for(int i = 1; i<x-a; i++)
                {
                    if(exist(x-i, y-i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        case 3:
            if((a<x && b>y) && (x-a == b-y))
            {
                for(int i = 1; i<x-a; i++)
                {
                    if(exist(x-i, y+i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        }
    }
    else if(piece->type == 'Q')
    {
        switch (type)
        {
        case 0:
            if((a>x && b>y) && (a-x == b-y))
            {
                for(int i = 1; i<a-x; i++)
                {
                    if(exist(x+i, y+i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        
        case 1:
            if((a>x && b<y) && (a-x == y-b))
            {
                for(int i = 1; i<a-x; i++)
                {
                    if(exist(x+i, y-i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        case 2:
            if((a<x && b<y) && (a-x == b-y))
            {
                for(int i = 1; i<x-a; i++)
                {
                    if(exist(x-i, y-i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        case 3:
            if((a<x && b>y) && (x-a == b-y))
            {
                for(int i = 1; i<x-a; i++)
                {
                    if(exist(x-i, y+i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        case 4:
            if(a==x && b>y)
            {
                for(int i = 1; i<b-y; i++)
                {
                    if(exist(x, y+i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        
        case 5:
            if(a==x && b<y)
            {
                for(int i = 1; i<y-b; i++)
                {
                    if(exist(x, y-i))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        case 6:
            if(b==y && a>x)
            {
                for(int i = 1; i<a-x; i++)
                {
                    if(exist(x+i, y))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        case 7:
            if(b==y && a<x)
            {
                for(int i = 1; i<x-a; i++)
                {
                    if(exist(x-i, y))
                        return false;
                }
                if((!exist(a, b) || (*find(a, b)).color != color))
                    return true;
            }
            return false;
        }
    }
}
bool Board::check(Piece* piece)
{
    const int&x = piece->x;
    const int&y = piece->y;
    for(int i = 0; i<pieces.size(); i++)
        {
            if(inGame(pieces[i]) == 1)
            {
                int x1 = (*pieces[i]).x;
                int y1 = (*pieces[i]).y;
                if((*pieces[i]).color != (*piece).color)
                {
                    Piece* p1 = deleted.size()!=0?deleted[0]:0;
                    if(move(x, y, pieces[i]))
                    {
                        Piece* p2 = deleted.size()!=0?deleted[0]:0;
                        Piece* p3 = p2!=p1 ? p2:0;
                        undo(x1, y1, x, y, pieces[i], p3);
                        return true;
                    }
                }
            }
        }
    return false;
}
bool Board::mate(Piece* piece)
{
    const int&x = piece->x;
    const int&y = piece->y;
    if(check(piece))
    {
        for(int i = 0; i<pieces.size(); i++)
        {
            if(inGame(pieces[i]) == 1)
            {
                if((*pieces[i]).color == piece->color)
                {
                    for(int j = 0; j<8; j++)
                    {
                        for(int k = 0; k<8; k++)
                        {
                            int x1 = (*pieces[i]).x;
                            int y1 = (*pieces[i]).y;
                            Piece* p1 = deleted.size()!=0?deleted[0]:0;
                            if(move(j, k, pieces[i]))
                            {
                                Piece* p2 = deleted.size()!=0?deleted[0]:0;
                                Piece* p3 = p2!=p1 ? p2:0;
                                if(!check(piece))
                                {
                                    undo(x1, y1, x, y, pieces[i], p3);
                                    return false;
                                }
                                else
                                {
                                    undo(x1, y1, x, y, pieces[i], p3);
                                }
                            }
                        }
                    }
                }
            }
        }
        return true; 
    }
    else
        return false;
} 
bool Board::inGame(Piece* piece)
    {
        for(int i = 0; i<deleted.size(); i++)
        {
            if(piece == deleted[i])
                return false;
        }
        return true;
    }
string Board::num2str(int&i, int&j)
{
    string res;
    res += (char)(97+j);
    res += (char)(56-i);
    return res;
}
bool Board::exist(const int&x, const int&y)
{
    for(int i = 0; i<pieces.size(); i++)
    {
        if(((*pieces[i]).x == x && (*pieces[i]).y == y) && inGame(pieces[i]) == 1)
            return true;
    }
    return false;
}
Piece* Board::find(const int&x, const int&y)
{
    if(exist(x, y))
    {
        for(int i = 0; i<pieces.size(); i++)
        {
            if((*pieces[i]).x == x && (*pieces[i]).y == y && inGame(pieces[i]) == 1)
                return (pieces[i]);
        }
    }
    else
        return 0;
}
void Board::sort(vector <string>& list)
{
    for(int i = 0; i<list.size()-1; i++)
    {
        for(int j = i+1; j<list.size(); j++)
        {
            if(list[i]>list[j])
            {
                string y = list[i];
                list[i] = list[j];
                list[j] = y;
            }
        }
    }
}
bool Board::checkIn(int&i, int&j, Piece& piece)
{
    for(int k = 0; k<piece.danger.size(); k++)
    {
        if(piece.danger[k][0] == i  && piece.danger[k][1] == j)
            return true;
    }
    return false;
}
bool Board::checkFull(Piece& piece)
{
    int x1 = piece.x;
    int y1 = piece.y;
    for(int i = 0; i<8; i++)
    {
        for(int j = 0; j<8; j++)
        {
            Piece* p1 = deleted.size()!=0?deleted[0]:0;
            if(move(i, j, &piece))
            {
                Piece* p2 = deleted.size()!=0?deleted[0]:0;
                Piece* p3 = p2!=p1 ? p2:0;
                undo(x1, y1, i, j, &piece, p3);
                if(!(checkIn(i, j, piece)))
                    return false;
            }
        }
    }
    return true;
}
void Board::dangerPos(const char& color)
{
    for(int i = 0; i<pieces.size(); i++)
    {
        string res;
        if((*pieces[i]).color == color)
        {
            if(!(*pieces[i]).danger.empty())
            {
                if(checkFull((*pieces[i])))
                {
                    res += num2str((*pieces[i]).x, (*pieces[i]).y);
                    res += (*pieces[i]).type;
                    res += (*pieces[i]).color;
                    dPos.push_back(res);
                }
                else
                {
                    for(int j = 0; j<(*pieces[i]).danger.size(); j++)
                    {
                        res = "";
                        res += num2str((*pieces[i]).x, (*pieces[i]).y);
                        res += (*pieces[i]).type;
                        res += (*pieces[i]).color;
                        res += num2str((*pieces[i]).danger[j][0], (*pieces[i]).danger[j][1]);
                        dPos.push_back(res); 
                    }
                }
            }
        }
    }
    if(dPos.size()!=0)
        sort(dPos);
}
void Board::attackPos(const char& color)
{
    for(int i = 0; i<pieces.size(); i++)
    {
        string res;
        if(!(*pieces[i]).attacking.empty())
        {
            if((*pieces[i]).color == color)
            {
                for(int j = 0; j<(*pieces[i]).attacking.size(); j++)
                {
                    res = "";
                    res += num2str((*pieces[i]).x, (*pieces[i]).y);
                    res += (*pieces[i]).type;
                    res += (*pieces[i]).color;
                    res += num2str((*pieces[i]).attacking[j][0], (*pieces[i]).attacking[j][1]);
                    aPos.push_back(res); 
                }
            }
        }
    }
    if(aPos.size()!=0)
        sort(aPos);
}
void Board::show(const char& type)
{
    if(type == 'D')
    {
        if(dPos.size() == 0)
            cout<<"No Answer!";
        else
        {
            for(int i = 0; i<dPos.size(); i++)
            {
                cout<<dPos[i]<<'\n';
            }
        }
    }
    else if(type == 'A')
    {
        if(aPos.size() == 0)
            cout<<"No Answer!";
        else
        {
            for(int i = 0; i<aPos.size(); i++)
            {
                cout<<aPos[i]<<'\n';
            }
        }
    }
}
void Board::undo(int x1, int y1, int x2, int y2, Piece* p1, Piece* changed)
{
    if(changed!=0)
    {
        for(int i = 0; i<deleted.size(); i++)
        {
            if(changed == deleted[i])
            {
                deleted.erase(deleted.begin()+i);
                break;
            }
        }
    }
    (*p1).x = x1;
    (*p1).y = y1;
}
void Board::defence(const char& color)
{
    Piece* king;
    Piece* minor = 0;
    for(int i = 0; i<pieces.size(); i++)
    {
        if((*pieces[i]).color== color && (*pieces[i]).type == 'K')
        {
            king = pieces[i];
        }
        else if((*pieces[i]).color!= color && (*pieces[i]).type == 'K')
        {
            minor = pieces[i];
        }
    }
    for(int i = 0; i<pieces.size(); i++)
    {
        if((*pieces[i]).color== color)
        {
            BB(minor, king, 0, pieces[i]);
        }
    }
}
void Board::attack(const char& color)
{
    Piece* king;
    Piece* minor = 0;
    for(int i = 0; i<pieces.size(); i++)
    {
        if((*pieces[i]).color!= color && (*pieces[i]).type == 'K')
        {
            king = pieces[i];
        }
        else if((*pieces[i]).color== color && (*pieces[i]).type == 'K')
        {
            minor = pieces[i];
        }
    }
    for(int i = 0; i<pieces.size(); i++)
    {
        if((*pieces[i]).color== color)
        {
            AA(minor, king, 0, pieces[i]);
        }
    }  
}
bool Board::AA(Piece* minor, Piece* king, int level, Piece* starter)
{
    if(level == 0)
    {
        int x1 = (*starter).x;
        int y1 = (*starter).y;
        for(int i = 0; i<8; i++)
        {
            for(int j = 0; j<8; j++)
            {
                bool checked = false;
                Piece* p1 = deleted.size()!=0?deleted[0]:0;
                if(move(i, j, starter))
                {
                    Piece* p2 = deleted.size()!=0?deleted[0]:0;
                    Piece* p3 = p2!=p1 ? p2:0;
                    if(mate(king) && (minor == 0 || !check(minor)))
                    {
                        (*starter).attacking.push_back({i, j});
                        checked = true;
                    }
                    if(checked == false&& (minor == 0 || !check(minor)))
                    {
                        bool next = AA(minor, king, level+1);
                        if(next == true)
                        {
                            (*starter).attacking.push_back({i, j});  
                        }
                    }
                    undo(x1, y1, i, j, starter, p3);
                }
            }
        }
    }
    if(level == 1)
    {
        for(int i = 0; i<pieces.size(); i++)
        {
            if((*pieces[i]).color == (*king).color && inGame(pieces[i]) == 1)
            {
                int x1 = (*pieces[i]).x;
                int y1 = (*pieces[i]).y;
                for(int j = 0; j<8; j++)
                {
                    for(int k = 0; k<8; k++)
                    {
                        Piece* p1 = deleted.size()!=0?deleted[0]:0;
                        if(move(j, k, pieces[i]))
                        {
                            Piece* p2 = deleted.size()!=0?deleted[0]:0;
                            Piece* p3 = p2!=p1 ? p2:0;
                            if(!check(king)) 
                            {
                                bool next = AA(minor, king, level+1);
                                if(next == false)
                                {
                                    undo(x1, y1, j, k, pieces[i], p3);
                                    return false; 
                                }
                            }
                            undo(x1, y1, j, k, pieces[i], p3);
                        }

                    }
                }
            }
        }
        return true;
    }
    if(level == 2)
    {
        for(int i = 0; i<pieces.size(); i++)
        {
            if((*pieces[i]).color != (*king).color && inGame(pieces[i]) == 1)
            {
                int x1 = (*pieces[i]).x;
                int y1 = (*pieces[i]).y;
                for(int j = 0; j<8; j++)
                {
                    for(int k = 0; k<8; k++)
                    {
                        Piece* p1 = deleted.size()!=0?deleted[0]:0;
                        if(move(j, k, pieces[i]))
                        {
                            Piece* p2 = deleted.size()!=0?deleted[0]:0;
                            Piece* p3 = p2!=p1 ? p2:0;
                            if(mate(king) && (minor == 0 || !check(minor))) 
                            {    
                                undo(x1, y1, j, k, pieces[i], p3);
                                return true;
                            }
                            undo(x1, y1, j, k, pieces[i], p3);
                        }

                    }
                }
            }
        }
        return false;
    }
}
bool Board::BB(Piece* minor, Piece* king, const int& level, Piece* starter)
    {
        if(level == 0)
        {
            int x1 = (*starter).x;
            int y1 = (*starter).y;
            for(int i = 0; i<8; i++)
            {
                for(int j = 0; j<8; j++)
                {
                    bool checked = false;
                    bool next = false;
                    Piece* p1 = deleted.size()!=0?deleted[0]:0;
                    if(move(i, j, starter))
                    {
                        Piece* p2 = deleted.size()!=0?deleted[0]:0;
                        Piece* p3 = p2!=p1 ? p2:0;
                        if(check(king))
                        {
                            (*starter).danger.push_back({i, j});
                            checked = true;
                        }
                        if(checked == false)
                        {
                            next = BB(minor, king, level+1);
                            if(next == true)
                            {
                                (*starter).danger.push_back({i, j});  
                            }
                        }
                        undo(x1, y1, i, j, starter, p3);
                    }
                }
            }
        }
        if(level == 1)
        {
            for(int i = 0; i<pieces.size(); i++)
            {
                if((*pieces[i]).color!= (*king).color && inGame(pieces[i]) == 1)
                {
                    int x1 = (*pieces[i]).x;
                    int y1 = (*pieces[i]).y;
                    for(int j = 0; j<8; j++)
                    {
                        for(int k = 0; k<8; k++)
                        {
                            Piece* p1 = deleted.size()!=0?deleted[0]:0;
                            if(move(j, k, pieces[i]))
                            {
                                Piece* p2 = deleted.size()!=0?deleted[0]:0;
                                Piece* p3 = p2!=p1 ? p2:0;
                                if(mate(king)&& (minor == 0 || !check(minor))) 
                                {
                                    undo(x1, y1, j, k, pieces[i], p3);
                                    return true;
                                }
                                else if((minor == 0 || !check(minor)))    
                                {
                                    bool next = BB(minor, king, level+1);
                                    if(next == true)
                                    {
                                        undo(x1, y1, j, k, pieces[i], p3);
                                        return true; 
                                    }
                                }
                                undo(x1, y1, j, k, pieces[i], p3);
                            }

                        }
                    }
                }
            }
            return false;
        }
        if(level == 2)
        {
            for(int i = 0; i<pieces.size(); i++)
            {
                if((*pieces[i]).color == (*king).color && inGame(pieces[i]) == 1)
                {
                    int x1 = (*pieces[i]).x;
                    int y1 = (*pieces[i]).y;
                    for(int j = 0; j<8; j++)
                    {
                        for(int k = 0; k<8; k++)
                        {
                            Piece* p1 = deleted.size()!=0?deleted[0]:0;
                            if(move(j, k, pieces[i]))
                            {
                                Piece* p2 = deleted.size()!=0?deleted[0]:0;
                                Piece* p3 = p2!=p1 ? p2:0;
                                if(!check(king)) 
                                {    
                                    bool next = BB(minor, king, level+1);
                                    if(next == false)
                                    {
                                        undo(x1, y1, j, k, pieces[i], p3); 
                                        return false; 
                                    } 
                                }
                                undo(x1, y1, j, k, pieces[i], p3);
                            }

                        }
                    }
                }
            }
            return true;
        }
        if(level == 3)
        {
            for(int i = 0; i<pieces.size(); i++)
            {
                if((*pieces[i]).color != (*king).color && inGame(pieces[i]) == 1)
                {
                    int x1 = (*pieces[i]).x;
                    int y1 = (*pieces[i]).y;
                    for(int j = 0; j<8; j++)
                    {
                        for(int k = 0; k<8; k++)
                        {
                            Piece* p1 = deleted.size()!=0?deleted[0]:0;
                            if(move(j, k, pieces[i]))
                            {
                                Piece* p2 = deleted.size()!=0?deleted[0]:0;
                                Piece* p3 = p2!=p1 ? p2:0;
                                if(mate(king)&& (minor == 0 || !check(minor))) 
                                {    
                                    undo(x1, y1, j, k, pieces[i], p3);
                                    return true;
                                }
                                undo(x1, y1, j, k, pieces[i], p3);
                            }

                        }
                    }
                }
            }
            return false;
        }
    }
vector<Piece*>Board:: pieces = {};
vector<Piece*>Board:: deleted = {};
#include "helper.h"
#include "iostream"
sf::Vector2f get_cell_position(int row,int column)
{
    return sf::Vector2f(
        setting::cell_offset + column * ( setting::cell_size + setting::line_space),
        setting::cell_offset + row * ( setting::cell_size + setting::line_space));
}

string get_Piece_path(const char&type, const char&color)
{
    string path = "resources/images/"; 
    path += type;
    path += color;
    return path + ".png";
}

int get_cell_index(int x)
{
    x -= setting::cell_offset;
    if (x<0)
        return -1;
    int cell_plus_space = setting::cell_size + setting::line_space;
    int index = x / cell_plus_space;
    if (index > 8 || x > index * cell_plus_space + setting::cell_size)
        return -1;
    return index;
}

string get_name_of_id(char color)
{
    return color == 'W' ? "White" : "Black";
}

#include "Piece.h"
#include "iostream"
#include "vector"
Piece::Piece(const int &x,const int &y)
{
    this->x = x;
    this->y = y;
}
void Piece::move(const int&x, const int&y)
{
    this->x = x;
    this->y = y;
}
void Piece::setMoveNum()
{}
std::map<vector<char>, sf::Texture> Piece_textures;
void Piece::load_texture()
{
    if (Piece_textures.find({type, color}) == Piece_textures.end())
        Piece_textures[{type, color}].loadFromFile(get_Piece_path(type, color));
    this->sprite.setTexture(Piece_textures[{type, color}]);
    float piece_scale_x = (float)setting::cell_size / this->sprite.getTexture()->getSize().x;
    float piece_scale_y = (float)setting::cell_size / this->sprite.getTexture()->getSize().y;
    this->sprite.setScale(piece_scale_x, piece_scale_y);
}
void Pawn::setMoveNum()
{
    if((x == 6 && color == 'W') || (x==1 && color == 'B'))
    {
        moveNum = 2;
    }
    else
        moveNum = 1;
}
Pawn::Pawn(const int &x,const int &y, const char& col): Piece(x, y)
{
    type = 'P';
    color = col;
    setMoveNum();
    load_texture();
}
void Rook::setMoveNum()
{
    moveNum = 4;
}
Rook::Rook(const int &x,const int &y, const char& col): Piece(x, y)
{
    type = 'R';
    color = col;
    setMoveNum();
    load_texture();
} 
void Knight::setMoveNum()
{
    moveNum = 8;
}
Knight::Knight(const int &x,const int &y, const char& col): Piece(x, y)
{
    type = 'N';
    color = col;
    setMoveNum();
    load_texture();
} 
void King::setMoveNum()
{
    moveNum = 8;
}
King::King(const int &x,const int &y, const char& col): Piece(x, y)
{
    type = 'K';
    color = col;
    setMoveNum();
    load_texture();
} 
void Bishop::setMoveNum()
{
    moveNum = 4; 
}
Bishop::Bishop(const int &x,const int &y, const char& col): Piece(x, y)
{
    type = 'B';
    color = col;
    setMoveNum();
    load_texture();
} 
void Queen::setMoveNum()
{
    moveNum = 8;
}
Queen::Queen(const int &x,const int &y, const char& col): Piece(x, y)
{
    type = 'Q';
    color = col;
    setMoveNum();
    load_texture();
}  
#include "setting.h"

int setting::cell_size = 105;
int setting::cell_offset = 35;
sf::Color setting::cell_color = sf::Color::Transparent;
int setting::line_space = 0;
int setting::board_x = 1100;
int setting::board_y = 1000;

#include "user.h"

User::User(const char& _color) : color(_color)
{}