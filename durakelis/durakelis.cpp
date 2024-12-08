#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <conio.h>
#include <time.h>

enum class suitType : unsigned int
{
    Clubs,
    Diamonds,
    Hearts,
    Spades,
    OP //special
};

enum class rankType : unsigned int
{
    badcard = 1,
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
    Nine = 9,
    Ten = 10,
    Jack = 11,
    Queen = 12,
    King = 13,
    Ace = 14,
    Super = 15 //special
};

struct Card
{
    suitType suit;
    rankType rank;
};
class Deck {
private:
    static int const cardQuant = 52;
    Card cards[cardQuant + 1];

public:
    Deck()
    {
        for (int i = (int)suitType::Clubs; i <= (int)suitType::Spades; i++)
        {
            for (int j = (int)rankType::Two; j <= (cardQuant / 4) + 1; j++)
            {
                Card c;
                c.suit = (suitType)i;
                c.rank = (rankType)j;

                int index = (cardQuant / 4) * i + j - 2;

                cards[index] = c;
            }
        }
        cards[cardQuant].rank = rankType::Super;
        cards[cardQuant].suit = suitType::OP;
        std::random_shuffle(std::begin(cards), std::end(cards));
        std::random_shuffle(std::begin(cards), std::end(cards));
        std::random_shuffle(std::begin(cards), std::end(cards));
        std::random_shuffle(std::begin(cards), std::end(cards));
    }
    ~Deck() {}
    Card returnCard(int i) {
        return cards[i];
    }
    int const getCardQuant() {
        return cardQuant + 1;
    }
};

class player
{
protected:
    std::vector <Card> playerHand[6];
public:
    player() {}
    ~player() {}
    void operator &(int i) {
        if ((*playerHand)[i].suit == suitType::Clubs)
            std::cout << "C";

        else if ((*playerHand)[i].suit == suitType::Diamonds)
            std::cout << "D";

        else if ((*playerHand)[i].suit == suitType::Hearts)
            std::cout << "H";

        else if ((*playerHand)[i].suit == suitType::Spades)
            std::cout << "S";

        else if ((*playerHand)[i].suit == suitType::OP)
            std::cout << "O";
    }
    void operator %(int i) {
        if ((*playerHand)[i].rank == rankType::Jack)
            std::cout << "J ";

        else if ((*playerHand)[i].rank == rankType::Queen)
            std::cout << "Q ";

        else if ((*playerHand)[i].rank == rankType::King)
            std::cout << "K ";

        else if ((*playerHand)[i].rank == rankType::Ace)
            std::cout << "A ";

        else if ((*playerHand)[i].rank == rankType::Super)
            std::cout << "SP";

        else if ((*playerHand)[i].rank == rankType::Ten)
            std::cout << "10";

        else std::cout << (int)(*playerHand)[i].rank << ' ';
    }
    int const getNumberOfCards()
    {
        return playerHand->size();
    }
    Card returnCard(int i) {
        return playerHand->at(i);
    }
    void deleteCards(int i)
    {
        playerHand->erase(playerHand->begin() + (i));
    }
    void deleteAll()
    {
        playerHand->clear();
    }
};
class humanplay : public player
{
public:
    humanplay() {}
    ~humanplay() {}
    void operator = (Card a)
    {
        playerHand->push_back(a);
    }
    bool isvalid(int i)
    {
        if (i < 0 || i + 1 > playerHand->size())
            return 0;
        else return 1;
    }
    void refill(Deck deck, int& usedCards)
    {
        while (getNumberOfCards() < 6 && usedCards != deck.getCardQuant())
        {
            playerHand->push_back(deck.returnCard(usedCards));
            usedCards++;
        }
    }
};
class computer : public player
{
public:
    computer() {}
    ~computer() {}
    void operator = (Card a)
    {
        playerHand->push_back(a);
    }
    void refill(Deck deck, int& usedCards)
    {
        while (getNumberOfCards() < 6 && usedCards != deck.getCardQuant())
        {
            playerHand->push_back(deck.returnCard(usedCards));
            usedCards++;
        }
    }
    Card findMin(suitType rt)//find lowest card when board is empty
    {
        int min = 16;
        int which = -2;

        for (int i = 0; i < playerHand->size(); i++)
            if ((int)(*playerHand)[i].rank < min && (*playerHand)[i].suit != rt) //looking for lowest card which is not trump
            {
                min = (int)(*playerHand)[i].rank;
                which = i;
            }

        if (which == -2) //card which is not trump was not found
            for (int i = 0; i < playerHand->size(); i++)
                if ((int)(*playerHand)[i].rank < min) //looking for lowest card but it can be trump
                {
                    min = (int)(*playerHand)[i].rank;
                    which = i;
                }

        Card c;
        c = (*playerHand)[which];
        deleteCards(which);
        return c;
    }
    Card playable(Card beat, suitType rt)//find card to defend with
    {
        int min = 16;
        int which = -2;
        std::vector <Card> temp;
        if (beat.suit == rt) //if card on board is trump card
        {
            for (int i = 0; i < playerHand->size(); i++)
                if ((*playerHand)[i].rank > beat.rank && (*playerHand)[i].suit == rt) //looking for the possible cards
                    temp.push_back((*playerHand)[i]);


            if (temp.size() == 0) //no playable card was found
            {
                Card c;
                c.rank = rankType::badcard;
                return c;
            }
            else
            {
                int suit;
                for (int i = 0; i < temp.size(); i++)
                    if ((int)temp[i].rank < min)//find the lowest card out of all playable cards
                    {
                        min = (int)temp[i].rank;
                        suit = (int)temp[i].suit;
                    }

                for (int i = 0; i < playerHand->size(); i++) //find it in vector so it can delete it once it has been picked
                    if ((int)(*playerHand)[i].rank == min && (*playerHand)[i].suit == rt)
                        which = i;

                Card c;
                c = (*playerHand)[which];
                deleteCards(which);
                temp.clear();
                return c;
            }
        }
        else // card on board is not trump card
        {
            for (int i = 0; i < playerHand->size(); i++)
                if ((*playerHand)[i].rank > beat.rank && (*playerHand)[i].suit != rt && (*playerHand)[i].suit == beat.suit) // find all possible cards that are not trump cards
                    temp.push_back((*playerHand)[i]);


            if (temp.size() == 0) //if no card was found
                for (int i = 0; i < playerHand->size(); i++)
                    if ((*playerHand)[i].suit == rt) //find all trump cards
                        temp.push_back((*playerHand)[i]);

            if (temp.size() == 0)// no card was found return badcard
            {
                Card c;
                c.rank = rankType::badcard;
                return c;
            }


            int suit;
            for (int i = 0; i < temp.size(); i++)// find the lowest possible card out of the good ones
                if ((int)temp[i].rank < min)
                {
                    min = (int)temp[i].rank;
                    suit = (int)temp[i].suit;
                }

            for (int i = 0; i < playerHand->size(); i++)
                if ((int)(*playerHand)[i].rank == min && (int)(*playerHand)[i].suit == suit)
                    which = i;

            Card c;
            c = (*playerHand)[which];
            deleteCards(which);
            temp.clear();
            return c;
        }
    }
    Card throwCard(Card compare, suitType ts) //throw in extra cards while attacking
    {
        for (int i = 0; i < playerHand->size(); i++)
            if ((*playerHand)[i].rank == compare.rank && (*playerHand)[i].suit != ts)//find cards that you can throw and they are not trump cards
            {
                Card c;
                c = (*playerHand)[i];
                deleteCards(i);
                return c;
            }
        Card c;
        c.rank = rankType::badcard;
        return c;
    }
    bool isSpecial()//looking if computer has special card
    {
        for (int i = 0; i < playerHand->size(); i++)
            if ((*playerHand)[i].rank == rankType::Super)
            {
                deleteAll();
                return 1;
            }
        return 0;
    }
};
class gameboard : public player
{
public:
    gameboard() {}
    ~gameboard() {}
    void operator = (Card a)
    {
        playerHand->push_back(a);
    }
    Card getrank()
    {
        return (*playerHand)[playerHand->size()];
    }

};
class trump
{
private:
    Card trumpCard;
public:
    trump() {}
    ~trump() {}
    void operator = (Card given)
    {
        trumpCard = given;
    }
    void printSuit() { //prints name of the trump

        if (trumpCard.suit == suitType::Clubs)
            std::cout << "Clubs";

        else if (trumpCard.suit == suitType::Diamonds)
            std::cout << "Diamonds";

        else if (trumpCard.suit == suitType::Hearts)
            std::cout << "Hearts";

        else if (trumpCard.suit == suitType::Spades)
            std::cout << "Spades";

        else if (trumpCard.suit == suitType::OP)
            std::cout << "no trump";
    }
    suitType trumpSuit()
    {
        return trumpCard.suit;
    }
};

bool isPossible(trump trumpcard, Card attacking, Card defending);
bool isThereAWinner(trump trumpCard, Deck deck, computer computerPlayer, gameboard board, humanplay humanPlayer, int usedCards);
bool allowedCard(Card boardCard, Card humanCard);

void visualiseComputerCards(int numCards);
void visualiseHumanCards(player humanPlayer);
void visualisewholeboard(trump trumpCard, Deck deck, computer computerPlayer, gameboard board, humanplay humanPlayer, int usedCards);


//main-----------------------------------------------------------
int main()
{
    srand(time(0));

    std::cout << "Do you want to play Durak? Press y to play \n";
    int pressedKey = _getch();
    if (pressedKey == 'y')
    {
        Deck deck;
        int remainingCards = deck.getCardQuant();
        int usedCards = 0;
        bool turn = 0;
        bool attack = 0;

        humanplay humanPlayer;
        computer computerPlayer;
        gameboard board;
        trump trumpCard;

        for (usedCards = 0; usedCards < 6; usedCards++) //give each player 6 cards
            humanPlayer = deck.returnCard(usedCards);
        for (usedCards = 6; usedCards < 12; usedCards++)
            computerPlayer = deck.returnCard(usedCards);
        trumpCard = deck.returnCard(deck.getCardQuant() - 1); //get trump card

        turn = rand() % 2;

        while (!isThereAWinner(trumpCard, deck, computerPlayer, board, humanPlayer, usedCards)) //Looking if there is a winner
        {
            if (turn == 1) //Human player turn
            {
                if (board.getNumberOfCards() == 0)
                    humanPlayer.refill(deck, usedCards); //refill cards if needed


                if (board.getNumberOfCards() % 2 == 0) //human player has to attack
                {
                    system("cls");
                    visualisewholeboard(trumpCard, deck, computerPlayer, board, humanPlayer, usedCards); //draw whole board

                    if (board.getNumberOfCards() > 0)
                        std::cout << "\n\nPress [0] to finish attack";

                    std::cout << "\nChoose card to attack: \n";

                    if (humanPlayer.getNumberOfCards() > 9) //let player choose card
                    {
                        std::cin >> pressedKey;
                        pressedKey--;
                    }
                    else pressedKey = _getch() - 49;


                    if (pressedKey == -1 && board.getNumberOfCards() > 0) //player finishes attack
                    {
                        board.deleteAll();
                        turn = 0;
                        if (humanPlayer.getNumberOfCards() < 6)
                            humanPlayer.refill(deck, usedCards);
                    }
                    else { //player doesnt finish attack
                        if (humanPlayer.isvalid(pressedKey) == 0)
                        {
                            std::cout << "\nNot a valid playing card! Choose again: \n";
                            while (humanPlayer.isvalid(pressedKey) == 0) //checking if card is valid
                            {
                                if (humanPlayer.getNumberOfCards() > 9)
                                {
                                    std::cin >> pressedKey;
                                    pressedKey--;
                                }
                                else pressedKey = _getch() - 49;
                                if (pressedKey == -1 && board.getNumberOfCards() > 0)
                                {
                                    board.deleteAll();
                                    turn = 0;
                                    humanPlayer.refill(deck, usedCards);
                                    break;
                                }
                            }
                        }
                        if (turn == 1)
                        {
                            if (board.getNumberOfCards() == 0) //player plays any card if board is empty
                            {

                                Card c;
                                c = humanPlayer.returnCard(pressedKey);
                                if (c.rank == rankType::Super)
                                {
                                    board.deleteAll();
                                    humanPlayer.deleteAll();
                                    humanPlayer.refill(deck, usedCards);
                                }
                                else
                                {
                                    board = humanPlayer.returnCard(pressedKey);
                                    humanPlayer.deleteCards(pressedKey);
                                }
                                turn = 0;
                            }
                            else //board is not empty
                            {
                                Card attack, boardCard;
                                bool wrong = 0;
                                attack = humanPlayer.returnCard(pressedKey);
                                if (attack.rank == rankType::Super)
                                {
                                    board.deleteAll();
                                    humanPlayer.deleteAll();
                                    turn = 0;
                                    humanPlayer.refill(deck, usedCards);

                                }
                                else {
                                    for (int i = 0; i < board.getNumberOfCards(); i++)
                                    {
                                        boardCard = board.returnCard(i);
                                        if (allowedCard(boardCard, attack))
                                        {
                                            board = attack;
                                            humanPlayer.deleteCards(pressedKey);
                                            wrong = 1;
                                            turn = 0;
                                            break;
                                        }
                                    }
                                    if (wrong == 0) //checking if card can be played
                                    {
                                        std::cout << "\nCan't use that card! Pick again\n";
                                        while (wrong == 0)
                                        {
                                            if (humanPlayer.getNumberOfCards() > 9)
                                            {
                                                std::cin >> pressedKey;
                                                pressedKey--;
                                            }
                                            else pressedKey = _getch() - 49;

                                            if (pressedKey == -1)
                                            {
                                                board.deleteAll();
                                                turn = 0;
                                                break;
                                            }
                                            if (humanPlayer.isvalid(pressedKey))
                                            {
                                                attack = humanPlayer.returnCard(pressedKey);
                                                for (int i = 0; i < board.getNumberOfCards(); i++)
                                                {
                                                    boardCard = board.returnCard(i);
                                                    if (allowedCard(boardCard, attack))
                                                    {

                                                        board = attack;
                                                        humanPlayer.deleteCards(pressedKey);
                                                        wrong = 1;
                                                        turn = 0;
                                                        break;
                                                    }
                                                }
                                                if (wrong == 1) break;
                                            }
                                        }
                                        if (humanPlayer.getNumberOfCards() < 6)
                                            humanPlayer.refill(deck, usedCards);
                                    }
                                }
                            }
                        }
                    }
                }
                else { //human player has to defend
                    system("cls");
                    //computerPlayer.showHand();

                    visualisewholeboard(trumpCard, deck, computerPlayer, board, humanPlayer, usedCards);
                    std::cout << "\n\nPress [0] to take cards";
                    std::cout << "\nChoose card to defend: \n";
                    if (humanPlayer.getNumberOfCards() > 9)
                    {
                        std::cin >> pressedKey;
                        pressedKey--;
                    }
                    else pressedKey = _getch() - 49;



                    if (pressedKey == -1) //player takes cards from board
                    {
                        while (board.getNumberOfCards() > 0)
                        {
                            humanPlayer = board.returnCard(board.getNumberOfCards() - 1);
                            board.deleteCards(board.getNumberOfCards() - 1);
                        }
                        turn = 0;
                    }
                    else  //player wants to defend
                    {
                        if (humanPlayer.isvalid(pressedKey) == 0)
                        {
                            std::cout << "\n";
                            std::cout << "Not a valid playing card! Choose again: \n";
                        }
                        while (humanPlayer.isvalid(pressedKey) == 0) //checking if card is valid
                        {
                            if (humanPlayer.getNumberOfCards() > 9)
                            {
                                std::cin >> pressedKey;
                                pressedKey--;
                            }
                            else pressedKey = _getch() - 49;
                            if (pressedKey == -1)
                            {
                                while (board.getNumberOfCards() > 0)
                                {
                                    humanPlayer = board.returnCard(board.getNumberOfCards() - 1);
                                    board.deleteCards(board.getNumberOfCards() - 1);
                                }
                                turn = 0;
                                break;
                            }
                        }
                        if (turn == 1)
                        {
                            int toDefend = board.getNumberOfCards() - 1;
                            Card c;
                            c = humanPlayer.returnCard(pressedKey);
                            if (c.rank == rankType::Super) //super card is played?
                            {
                                board.deleteAll();
                                humanPlayer.deleteAll();
                                turn = 0;
                                if (humanPlayer.getNumberOfCards() < 6)
                                    humanPlayer.refill(deck, usedCards);
                            }
                            else {
                                if (!isPossible(trumpCard, humanPlayer.returnCard(pressedKey), board.returnCard(toDefend))) //checking if card can be played
                                {

                                    std::cout << "\n";
                                    std::cout << "Not a valid playing card! Choose again: \n";
                                    while (humanPlayer.isvalid(pressedKey))
                                    {
                                        if (humanPlayer.isvalid(pressedKey))
                                            if (isPossible(trumpCard, humanPlayer.returnCard(pressedKey), board.returnCard(toDefend)))
                                                break;

                                        if (humanPlayer.getNumberOfCards() > 9)
                                        {
                                            std::cin >> pressedKey;
                                            pressedKey--;
                                        }
                                        else pressedKey = _getch() - 49;
                                        if (pressedKey == -1)
                                        {
                                            while (board.getNumberOfCards() > 0)
                                            {
                                                humanPlayer = board.returnCard(board.getNumberOfCards() - 1);
                                                board.deleteCards(board.getNumberOfCards() - 1);
                                            }
                                            turn = 0;
                                            break;
                                        }
                                    }
                                }
                                if (turn == 1) //play the card
                                {
                                    board = humanPlayer.returnCard(pressedKey);
                                    humanPlayer.deleteCards(pressedKey);
                                }
                                turn = 0;
                            }
                        }
                    }
                }
            }
            else if (turn == 0) //computer turn
            {
                if (board.getNumberOfCards() == 0) //if computer has to attack computer chooses card and places
                {
                    if (computerPlayer.getNumberOfCards() < 6)
                        computerPlayer.refill(deck, usedCards);

                    if (computerPlayer.isSpecial())//if has special then plays it
                    {
                        board.deleteAll();
                        computerPlayer.refill(deck, usedCards);
                    }
                    else board = computerPlayer.findMin(trumpCard.trumpSuit());//finds lowest card
                    turn = 1;
                }

                else if (board.getNumberOfCards() > 0 && board.getNumberOfCards() % 2 != 0) //computer has to defend
                {
                    if (computerPlayer.isSpecial()) // if has special then plays it
                    {
                        board.deleteAll();
                        computerPlayer.refill(deck, usedCards);
                    }
                    else {
                        Card c;
                        c = computerPlayer.playable(board.returnCard(board.getNumberOfCards() - 1), trumpCard.trumpSuit()); //looking if he can defend
                        if (c.rank == rankType::badcard)// he cant
                        {
                            while (board.getNumberOfCards() > 0)
                            {
                                computerPlayer = board.returnCard(board.getNumberOfCards() - 1);// takes cards
                                board.deleteCards(board.getNumberOfCards() - 1);
                            }
                        }
                        else
                        {
                            board = c; //plays card
                        }
                    }
                    turn = 1;
                }
                else //computer throws in cards
                {
                    int temp = board.getNumberOfCards();
                    Card c;
                    for (int i = 0; i < temp; i++)
                    {
                        c = computerPlayer.throwCard(board.returnCard(i), trumpCard.trumpSuit());//looking if he has any cards to throw in
                        if (c.rank != rankType::badcard)//he does
                        {
                            board = c;
                            break;
                        }
                    }
                    if (c.rank == rankType::badcard)// he doesnt
                    {
                        board.deleteAll();
                        computerPlayer.refill(deck, usedCards);
                    }
                    turn = 1;
                }
            }
        }
    }
    else
    {
        system("cls");
        std::cout << "Goodbye! \n";
    }
}
//free functions------------------------------------------------------------------------

bool isThereAWinner(trump trumpCard, Deck deck, computer computerPlayer, gameboard board, humanplay humanPlayer, int usedCards)
{
    if (humanPlayer.getNumberOfCards() == 0 && usedCards == deck.getCardQuant())
    {
        system("cls");
        visualisewholeboard(trumpCard, deck, computerPlayer, board, humanPlayer, usedCards);
        std::cout << "\nYou won!\n";
        return 1;
    }
    else if (computerPlayer.getNumberOfCards() == 0 && usedCards == deck.getCardQuant())
    {
        system("cls");
        visualisewholeboard(trumpCard, deck, computerPlayer, board, humanPlayer, usedCards);
        std::cout << "\nComputer won! :c\n";
        return 1;
    }
    else return 0;

}
bool isPossible(trump trumpcard, Card attacking, Card defending)
{
    if (attacking.suit == trumpcard.trumpSuit() && defending.suit != trumpcard.trumpSuit())
        return 1;
    if (attacking.suit == trumpcard.trumpSuit() && defending.suit == trumpcard.trumpSuit() && attacking.rank > defending.rank)
        return 1;
    if (attacking.rank > defending.rank && defending.suit == attacking.suit)
        return 1;
    return 0;
}
bool allowedCard(Card boardCard, Card humanCard)
{
    if (boardCard.rank == humanCard.rank)
        return 1;
    return 0;
}

//card drawings ------------------------------------------------------------------------
void visualiseComputerCards(int numCards)
{
    std::cout << "Computer cards: \n";
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < numCards; j++)
            std::cout << "######" << std::setw(7);

        std::cout << "\n";
    }

}
void visualiseHumanCards(player humanPlayer)
{
    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
        std::cout << " ________" << std::setw(11);

    std::cout << "\n";

    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
    {
        std::cout << '|';
        humanPlayer% j;
        std::cout << std::setfill(' ') << std::setw(8);
        std::cout << "| ";
    }

    std::cout << "\n";

    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
        std::cout << '|' << std::setfill(' ') << std::setw(10) << "| ";

    std::cout << "\n";

    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
        std::cout << '|' << std::setfill(' ') << std::setw(10) << "| ";

    std::cout << "\n";

    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
    {
        std::cout << '|' << std::setfill(' ') << std::setw(5);
        humanPlayer& j;
        std::cout << std::setfill(' ') << std::setw(5) << "| ";
    }
    std::cout << "\n";

    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
        std::cout << '|' << std::setfill(' ') << std::setw(10) << "| ";

    std::cout << "\n";

    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
        std::cout << '|' << std::setfill(' ') << std::setw(10) << "| ";

    std::cout << "\n";
    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
    {
        std::cout << "|      ";
        humanPlayer% j;
        std::cout << "| ";
    }
    std::cout << "\n";

    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
        std::cout << " ________" << std::setw(11);

    std::cout << "\n";

    for (int j = 0; j < humanPlayer.getNumberOfCards(); j++)
        std::cout << std::setfill(' ') << std::setw(5) << "[" << j + 1 << "]" << std::setfill(' ') << std::setw(4) << " ";
}

void visualisewholeboard(trump trumpCard, Deck deck, computer computerPlayer, gameboard board, humanplay humanPlayer, int usedCards)
{
    std::cout << "Trump suit is:";
    trumpCard.printSuit();

    std::cout << "\nCards left in deck: " << deck.getCardQuant() - usedCards;
    std::cout << "\n";

    visualiseComputerCards(computerPlayer.getNumberOfCards());
    std::cout << "\n\n\n";

    visualiseHumanCards(board);
    std::cout << "\n\n\n\n";

    visualiseHumanCards(humanPlayer);
}