#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

// classes for error exceptions.
struct BadSuit
{
};
struct BadNumberDecks
{
};

// Some global constants.
const int PLAYER_CHIP = 100;   // Initial number of player's chips.
const int DEALER_CHIP = 10000; // Initial number of dealer's chips.
const int MAX_BET = 5;         // Maximum number of chips for a bet in a round.
const int MAX_CHARACTER = 100; // Maximum number of characters in a line
// to be used for standard input.

// Class that represents a card. It needs a value (rank) and a suit.
class Card
{
public:
    // Constructor.
    // num should be in the range of 1<= num <=13
    // (1: Ace, 11:J, 12:Q, 13:K),
    // suits: 'c' (club), 's' (spade), 'h' (heart), 'd' (diamond).
    Card(int num1 = 1, char suit1 = 's')
    {
        // Assigning the rank of the card using a number.
        if (num1 < 1)
            num = 1;
        else if (num1 > 13)
            num = 13;
        else
            num = num1;
        // Assigning the suit of the card.
        if (suit1 == 'c' || suit1 == 'C')
            suit = 'c';
        else if (suit1 == 's' || suit1 == 'S')
            suit = 's';
        else if (suit1 == 'h' || suit1 == 'H')
            suit = 'h';
        else if (suit1 == 'd' || suit1 == 'D')
            suit = 'd';
        else
            throw BadSuit(); // Error exception for bad input.
    }

    // Get the value of the card (1~13).
    int getValue() const
    {
        return num;
    }

    // Get the rank of the card (A, 2,..., 10, J, Q, K) as strings.
    string getRank() const
    {
        if (num == 1)
            return "A";
        else if (num == 2)
            return "2";
        else if (num == 3)
            return "3";
        else if (num == 4)
            return "4";
        else if (num == 5)
            return "5";
        else if (num == 6)
            return "6";
        else if (num == 7)
            return "7";
        else if (num == 8)
            return "8";
        else if (num == 9)
            return "9";
        else if (num == 10)
            return "10";
        else if (num == 11)
            return "J";
        else if (num == 12)
            return "Q";
        else if (num == 13)
            return "K";
    }

    // Get the suit of the given card.
    char getSuit() const
    {
        return suit;
    }

private:
    int num;   // The value (rank) of the card.
    char suit; // The suit of the card (c, s, h, d).
};

// outstream operator overloading for the Card class.
ostream &operator<<(ostream &s, const Card &c)
{
    return s << c.getRank() << "(" << c.getSuit() << ')';
}

// Class that represents the decks of cards for the game of blackjack.
// Number of decks allowed here are 1, 2, and 4.
class Decks
{
public:
    // Constructor. The number of decks should be given (default=1).
    Decks(int nDeck = 1) : cards(), current(0)
    {
        create(nDeck); // create all cards.
    }

    // Create all cards of given number of decks.
    void create(int nDeck)
    {
        if (nDeck != 1 && nDeck != 2 && nDeck != 4)
            throw BadNumberDecks();
        cards.resize(nDeck * 52); // size of the array resized.
        int index = 0;
        for (int i = 0; i < nDeck; i++)
            // Creating and storing cards for nDeck decks.
            for (int n = 1; n <= 13; n++)
            {
                cards[index++] = Card(n, 'c');
                cards[index++] = Card(n, 's');
                cards[index++] = Card(n, 'h');
                cards[index++] = Card(n, 'd');
            }
    }

    // Randomly shuffle all cards in the deck and set current as 0.
    void shuffle()
    {
        // using the standard library.
        random_shuffle(cards.begin(), cards.end());
        current = 0;
    }

    // Deal a card that are pointed by current and add 1 to current.
    Card deal()
    {
        // If all cards are dealt, shuffle cards again.
        if (current == cards.size())
            shuffle();
        return cards[current++];
    }

    // Print all cards at the current shuffled state.
    void print() const
    {
        for (int i = 0; i < cards.size(); i++)
            cout << cards[i] << endl;
    }

private:
    vector<Card> cards; // Array for all cards.
    int current;        // The pointer for the current card.
};

// Class that represents a hand of a player or a dealer.
class Hand
{
public:
    // Constructor.
    Hand() : cardsAtHand() {}

    // Add a card to the hand.
    void addCard(Card card)
    {
        cardsAtHand.push_back(card);
    }

    // Compute the value of the hand for the blackjack using S17.
    // This function will be used to determine whether the dealer
    // should hit or stand (S17 rule is used here), and to find out
    // the value of the player's hand,
    // for example,  after the player decides to stand.
    int getValue() const
    {
        int value = 0;
        // true if an ace exists in the hand.
        bool ifAce = false;
        for (int i = 0; i < cardsAtHand.size(); i++)
        {
            int cardValue = cardsAtHand[i].getValue();
            // Give 10 for all face cards (J, Q, K).
            if (cardValue > 10)
                cardValue = 10;
            if (cardValue == 1)
                ifAce = true;
            value += cardValue;
        }
        // Ace is always counted as 11 if doing so dose not
        // make the hand bust (S17 rule for dealer's hand).
        if (ifAce && value < 12)
            value += 10;
        return value;
    }

    // Return the number of cards of the hand.
    int size() const
    {
        return cardsAtHand.size();
    }

    // Returns true if the hand is "blackjack"
    // (getting the value 21 with 2 cards).
    bool blackjack() const
    {
        return (cardsAtHand.size() == 2 && getValue() == 21);
    }

    // Remove all cards of the hand.
    void removeAllCards()
    {
        cardsAtHand.clear();
    }

    // print the cards in the hand.
    // (if hideFirst==true, the first card will not be shown).
    void print(bool hideFirst = false) const
    {
        if (cardsAtHand.size() == 0)
            cout << "No card." << endl;
        for (int i = 0; i < cardsAtHand.size(); i++)
            if (i == 0 && hideFirst)
                cout << "?(?) ";
            else
                cout << cardsAtHand[i] << ' ';
        cout << endl;
    }

private:
    std::vector<Card> cardsAtHand; // Array of cards in a hand.
};

// Class that represents the game (displaying texts using standard output,
// and managing the flow of the game using 5 stages).
// User inputs are obtained also using standard input, and only the first
// character (excluding white spaces) of an input (a line ending with \n)
// is assumed to be the only user input to reduce confusions.
class Game
{

private:
    Decks myDecks;                // All cards for the game.
    Hand playerHand, dealerHand;  // Hands for the game.
    int nPlayerChip, nDealerChip; // Numbers of chips for players.
    int nBet;                     // Bet of the current round.

    char temp[MAX_CHARACTER + 1]; // C-string for temporary storages.
public:
    // Constructor.
    Game() : myDecks(), playerHand(), dealerHand()
    {
        nPlayerChip = PLAYER_CHIP;
        nDealerChip = DEALER_CHIP;
    }

    // Manage the flow of the game.
    void play()
    {
        // Setting the random seed.
        srand(time(NULL));
        // Starting the game (stage1).
        char input = beginGame(); // one-character user input.
        // Keep playing rounds if the player wants.
        while (input == 'n')
        {
            beginRound(); // (stage2)
            do
            {
                input = inRound(); // (stage3)
            } while (input == 'h');
            // There are three cases for ending a round:
            // (1) player busting (input == 'b'),
            // (2) player stands (input == 's').
            // (3) player forces to quit while playing.
            input = endRound(input); // (stage4)
        }
        endGame(); // (stage5)
    }

private:
    // Beginning of the game (stage 1).
    // Returns the input (n, r, q).
    // (n: next round, r: rule, q: quit.)
    char beginGame()
    {
        cout << "###########################" << endl;
        cout << "#  The Game of Blackjack  #" << endl;
        cout << "###########################" << endl;
        cout << endl;

        int nDeck;
        // Get the input, the number of decks (nDeck).
        do
        { // (will be repeated until a right input is given).
            cout << "Choose the number of decks to use ";
            cout << "[1/2/4] (default: 1):  ";
            cin.getline(temp, MAX_CHARACTER); // getline is the member function of cin
            // extract a number from the first char.
            nDeck = firstChar(temp, MAX_CHARACTER) - '0';
            // default value is 1 when no input.
            if (nDeck == 0)
                nDeck = 1;
        } while (nDeck != 1 && nDeck != 2 && nDeck != 4);
        cout << endl;

        myDecks.create(nDeck); // Creating the decks of cards.
        cout << nDeck << " deck" << (nDeck == 1 ? "" : "s");
        cout << " (" << 52 * nDeck << " cards) ";
        cout << (nDeck == 1 ? "has" : "have");
        cout << " been created and shuffled." << endl;
        cout << "You are given " << PLAYER_CHIP;
        cout << " chips now, and you can bet";
        cout << " upto " << MAX_BET;
        cout << " chips for each round.\n"
             << endl;

        char input; // For user input.
        do
        { // (will be repeated until a right input is given).
            cout << "Type n for a new round, r for rules, ";
            cout << "and q to quit [n/r/q] (default: n): ";
            cin.getline(temp, MAX_CHARACTER);
            // extract the first char.
            input = firstChar(temp, MAX_CHARACTER);
            if (input == '0')
                input = 'n';
            if (input == 'r')
                displayRules();
        } while (input != 'n' && input != 'q');
        cout << endl;
        return input;
    }

    // Beginning of a round (stage 2)
    // Returns the input (amount of bet; 1-5).
    void beginRound()
    {
        cout << "===================================" << endl;
        cout << "* Starting a New Round (your chips: ";
        cout << nPlayerChip << ").\n"
             << endl;

        bool insufficient = false;
        // Used when checking (remaining chips) < nBet.
        do
        { // (will be repeated until a right input is given).
            if (insufficient)
            {
                cout << "You only have " << nPlayerChip;
                cout << " chip(s)." << endl;
            };
            cout << "How many chips do you want to bet? ";
            cout << "[1-5] (default: 1): ";
            insufficient = false; // Set it for a new input.

            // Read a line and store in temp.
            cin.getline(temp, MAX_CHARACTER);
            // extract a number from the first char.
            nBet = firstChar(temp, MAX_CHARACTER) - '0';
            if (nBet == 0)
                nBet = 1; // Default is 1.
            // Check if chips are sufficient.
            if (nBet > nPlayerChip || nBet > nDealerChip)
                insufficient = true;
        } while (nBet < 1 || nBet > MAX_BET || insufficient);

        cout << "You bet " << nBet << " chip";
        cout << (nBet == 1 ? "." : "s.") << endl;

        dealerHand.removeAllCards(); // return all cards
        playerHand.removeAllCards(); // return all cards
        myDecks.shuffle();           // Shuffle the cards.

        // Dealing two cards to each player.
        dealerHand.addCard(myDecks.deal());
        playerHand.addCard(myDecks.deal());
        dealerHand.addCard(myDecks.deal());
        playerHand.addCard(myDecks.deal());
    }

    // Middle of a round (stage 3).
    // Returns a character (h, s, b, j, q).
    // (h: hit, s: stand, b: busted, j: blackjack, q: quit).
    char inRound()
    {
        int playerValue = playerHand.getValue();
        // Check for the blackjack.
        // Then stop the round automatically, and return 'j'
        if (playerHand.blackjack())
        {
            cout << "You got the blackjack!" << endl;
            return 'j';
        };

        // Check for the player busting
        // (if busted, stop the round and return 'b').
        if (playerValue > 21)
            return 'b';

        // Show hands (with dealer's first card hidden).
        showHands(true);

        char input; // user input in this stage.
        do
        { // (will be repeated until a right input is given).
            cout << "Type h for Hit, s for Stand, r for ";
            cout << "rules, q to quit [h/s/r/q] ";
            cout << "(default: h): ";
            cin.getline(temp, MAX_CHARACTER);
            // extract the first char.
            input = firstChar(temp, MAX_CHARACTER);
            // Default is h(Hit).
            if (input == '0')
                input = 'h';
            if (input == 'r')
                displayRules(); // for rules.
        } while (input != 'h' && input != 's' && input != 'q');

        // Add a card for "Hit".
        if (input == 'h')
            playerHand.addCard(myDecks.deal());
        return input;
    }

    // End of a round (stage 4)
    // (busted=true if the player got busted, false if not).
    char endRound(char in)
    {
        int playerValue = playerHand.getValue();
        if (in == 'b')
        { // if the player is busted.
            cout << "\nYou have:\t ";
            playerHand.print();
            cout << "You got busted, and lost ";
            cout << nBet << " chips." << endl;
            takeChips(-nBet);
        }
        else if (in == 'q')
        { // if the player quits the game.
            cout << "\nYou lost " << nBet << " chips.";
            cout << endl;
            takeChips(-nBet);
            return in;
        }
        else if (in == 'j')
        {                // if player gets the blackjack.
            showHands(); // Show cards.
            if (dealerHand.blackjack())
            {
                cout << "It is tied, and the bet is ";
                cout << "returned." << endl;
            }
            else
            {
                cout << "You won, and gained ";
                cout << nBet << " chips." << endl;
                takeChips(nBet);
            };
        }
        else
        { // if the player stands. We need to look at the
            // dealer's hand. The dealer hits until (
            // the value of the hand) >= 17 (S17 rule).
            while (dealerHand.getValue() < 17)
                dealerHand.addCard(myDecks.deal());
            showHands(); // Show cards.

            int dealerValue = dealerHand.getValue();
            // If the dealer gets busted.
            if (dealerValue > 21)
            {
                cout << "Dealer got busted, and you ";
                cout << "gained " << nBet;
                cout << " chips." << endl;
                takeChips(nBet);
            }
            // Dealer wins.
            else if (dealerValue > playerValue)
            {
                if (dealerHand.blackjack())
                {
                    cout << "Dealer got the ";
                    cout << "blackjack!\n";
                }
                cout << "Dealer won, and you lost ";
                cout << nBet << " chips." << endl;
                takeChips(-nBet);
            }
            // Player wins.
            else if (dealerValue < playerValue)
            {
                cout << "You won, and you gained ";
                cout << nBet << " chips." << endl;
                takeChips(nBet);
            }
            // A tie.
            else
            {
                // If the dealer has the blackjack and
                // the score is tied, the dealer wins.
                if (dealerHand.blackjack())
                {
                    cout << "Dealer got the ";
                    cout << "blackjack! ";
                    cout << "You lost " << nBet;
                    cout << " chips." << endl;
                    takeChips(-nBet);
                }
                // the score is tied, and the dealer
                // doesn't have blackjack.
                else
                {
                    cout << "It is tied, and the ";
                    cout << "bet is returned.";
                    cout << endl;
                }
            };
        }
        cout << "\n* End of the Round (your chips: ";
        cout << nPlayerChip << ")." << endl;
        cout << "===================================" << endl;

        // If all chips are used up, the game ends.
        if (nPlayerChip == 0 || nDealerChip == 0)
            return 'q';

        char input; // character for the user input.
        do
        { // (will be repeated until a right input is given).
            cout << "\nType n for a new round, r for rules";
            cout << ", q to quit [n/r/q] (default: n): ";
            cin.getline(temp, MAX_CHARACTER);
            // extract the first char.
            input = firstChar(temp, MAX_CHARACTER);
            // Default is n(new round).
            if (input == '0')
                input = 'n';
            if (input == 'r')
                displayRules(); // Rules.
        } while (input != 'n' && input != 'q');
        return input;
    }

    // End of the game (Stage 5)
    void endGame()
    {
        cout << "\nYour remaining chips: " << nPlayerChip;
        cout << " (you ";
        int diff = nPlayerChip - PLAYER_CHIP;
        if (diff > 0)
            cout << "gained " << diff << " chips).";
        else if (diff < 0)
            cout << "lost " << -diff << " chips).";
        else
            cout << "have the same number of chips as started).";
        cout << endl
             << endl;
        cout << "###########################" << endl;
        cout << "#     End of the Game     #" << endl;
        cout << "###########################" << endl;
    }

    // Display rules.
    void displayRules()
    {
        cout << endl;
        cout << "===================================" << endl;
        cout << "# How to play the game of Blackjack. " << endl;
        cout << "\n\tThere are two players: a dealer, ";
        cout << "played by a computer, ";
        cout << "and a player, played by you. ";
        cout << "\nThe game will be played as many ";
        cout << "rounds as the player can or wants, ";
        cout << "and the winner is determined ";
        cout << "each round. \nAt the beginning of the game, ";
        cout << "the player chooses ";
        cout << "how many decks are used for all rounds, ";
        cout << "where each deck ";
        cout << "consists of 52 cards, 13 for each suit ";
        cout << "(Club, Spade, Heart, and Diamond); ";
        cout << "here the number of decks can be 1 or 2 or 4. ";
        cout << "\nYou, the player, start with 100 chips and ";
        cout << "can bet at least 1 chip each round. ";
        cout << "\nThe maximum number of chips a player can bet ";
        cout << "at each round is set at " << MAX_BET;
        cout << " chips here. \nThe dealer is assumed to have ";
        cout << DEALER_CHIP << " chips in the beginning. ";
        cout << "\nIf either the player or the dealer loses all ";
        cout << "chips, the game ends.\n\n";
        cout << "\tAt each round, the objective of the player ";
        cout << "is to win the bet by creating a card total ";
        cout << "that is higher than the value of ";
        cout << "the dealer's hand, but not exceeding 21 ";
        cout << "(called, \"busting\"). ";
        cout << "\nThe value of a hand is determined by summing ";
        cout << "over values of all ";
        cout << "cards in a hand: 2~10 have the same values ";
        cout << "as the face values, ";
        cout << "while J, Q, and K (face cards) are counted ";
        cout << "as 10 and an ace, A, ";
        cout << "can be counted as 1 or 11. The suits of the ";
        cout << "cards don't have any meaning.\n\n";
        cout << "\tOnce the amount of the bet is chosen for ";
        cout << "each round, ";
        cout << "two cards are dealt at the beginning of the ";
        cout << "round: both cards of the player are revealed,";
        cout << " while only one card is revealed for the ";
        cout << "dealer. ";
        cout << "\nThe player has two options: Hit or Stand.\n";
        cout << "\n(1) Hit: Take another card from the dealer.";
        cout << "\nIf the player's hand ";
        cout << "is not busted by exceeding 21, ";
        cout << "the player has another chance ";
        cout << "to choose to hit or stand,\n\n";
        cout << "(2) Stand: Take no more card. ";
        cout << "\nThen, the player's value is ";
        cout << "determined by summing over all cards in ";
        cout << "the hand (An ace, A, can be either 1 or 11, ";
        cout << "whichever is better).\n\n";
        cout << "\tIf the player gets busted by exceeding 21, ";
        cout << "the dealer wins. If the player choose to ";
        cout << "stand at a value 21 or lower, ";
        cout << "the dealer should hit until the value is ";
        cout << "17 or greater (the ace, A, is counted as 11 ";
        cout << "as long as the sum is less than 21, ";
        cout << "even when the sum becomes 17, which is ";
        cout << "called \"S17\" rule). ";
        cout << "\nIf the dealer gets busted, the player wins. ";
        cout << "\nIf both are not busted, ";
        cout << "the winner is determined by comparing values;";
        cout << " the player wins ";
        cout << "if the player's value is greater, and the ";
        cout << "dealer wins if the dealer's value is greater.";
        cout << " If tied, the bet is returned to the player.";
        cout << "\n\n\tIf the first two cards has the value ";
        cout << "21 by having an ace and ";
        cout << "a 10-valued card (10 or J or Q or K), ";
        cout << "it's called the \"Blackjack\" and ";
        cout << "wins every hand except another blackjack (if ";
        cout << "both get blackjacks, it's a tie).\n\n\n";
        cout << "# Card representation.\n\n";
        cout << "The ranks: A (ace), 2, 3, 4, 5, 6, 7, 8, 9, ";
        cout << "10, J, Q, K.\n";
        cout << "The suits: c (club), s (spade), h (heart), ";
        cout << "d (diamond).\n\n";
        cout << "Then, for example, A(s) stands for the spade ";
        cout << "ace, 10(d) stands for the diamond 10, ";
        cout << "and Q(h) stands for the heart queen.\n\n\n";
        cout << "# Player inputs.\n\n";
        cout << "\tThe player can give inputs using keyboards ";
        cout << "at the prompt, and only the first character ";
        cout << "(excluding white spaces) of a line, followed ";
        cout << "by Enter, will be regarded as a valid input. ";
        cout << "Possible input characters are: n (new round),";
        cout << " r (rules), h (hit), s (stand), q (quit), ";
        cout << "and 1~5 (size of the bet, number of decks).\n";
        cout << "===================================" << endl;
        cout << endl;
    }

    // A function that extracts a first character from a c-string
    // (returns '0' if none exists).
    // temp: character array, size: size of temp.
    static char firstChar(char *temp, int size)
    {
        int i = 0;
        // skipping white spaces.
        while (i < size && temp[i] != '\0' && (temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n'))
            i++;
        // If there is no character in temp, return '0'.
        if (temp[i] == '\0' || i >= size)
            return '0';
        else
            return temp[i];
    }

    // Show hands of both players
    // (if hideFirst==true, the first card will not be shown).
    void showHands(bool hideFirst = false)
    {
        cout << "\nDealer has:\t ";
        dealerHand.print(hideFirst);
        cout << "You have:\t ";
        playerHand.print();
        cout << endl;
    }

    // Chips changing hands.
    // (amount > 0: dealer to player; < 0, player to dealer).
    void takeChips(int amount)
    {
        nPlayerChip += amount;
        nDealerChip -= amount;
    }
};

// Main function (driver).
int main()
{
    Game g;
    try
    {
        g.play();
    }
    catch (BadSuit e)
    {
        cerr << "*** Bad card suit is given.\n";
        exit(1);
    }
    catch (BadNumberDecks e)
    {
        cerr << "*** Bad number of decks is given.\n";
        exit(1);
    };
    return 0;
}
