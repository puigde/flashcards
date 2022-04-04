#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <chrono>
using namespace std;
class Flashcard
{
public:
    string label;
    string answer;
    string hint;

    void printlabel()
    {
        cout << label << endl;
    }
    void printanswer()
    {
        cout << "The answer is: " << answer << endl;
    }
    void showhint()
    {
        cout << "Here's a hint: " << hint << endl;
    }
    int importance = 1; // indicates the importance a flashcard has 1 for max importance, higher values for less importance 1 imp will apear n times more than n imp
    int turns = importance;

    // implement penalization for missed answers so harder concepts get consolidated
    void mod_importance(int newvalue)
    {
        importance = newvalue;
    }
    void turn()
    {
        turns--;
    }
    void reset_turn()
    {
        turns = importance;
    }
};
using fvect = vector<Flashcard>;
fvect cards;

// Reads the flashcards from a specified txt file which has for each line the format [Label,Answer,Hint,]
void read_flashcards(string filepath)
{
    fstream file;
    file.open(filepath, ios::in);
    if (file.is_open())
    {
        string tp;
        while (getline(file, tp))
        {
            Flashcard toinsert_card;
            int minicounter = 0;
            string delimiter = ",";
            size_t pos = 0;
            string token;
            while ((pos = tp.find(delimiter)) != std::string::npos)
            {
                token = tp.substr(0, pos);
                if (minicounter == 0)
                {
                    toinsert_card.label = token;
                    minicounter++;
                }
                else if (minicounter == 1)
                {
                    toinsert_card.answer = token;
                    minicounter++;
                }
                else
                {
                    toinsert_card.hint = token;
                    minicounter++;
                }
                tp.erase(0, pos + delimiter.length());
            }
            cards.push_back(toinsert_card);
        }
    }
    file.close();
}

// Returns a random integer between lb and ub both included
int randint(mt19937_64 &rng, int lb, int ub)
{
    return rng() % (ub - lb + 1) + lb;
}

void printdata()
{
    for (Flashcard card : cards)
    {
        card.printlabel();
        card.printanswer();
        card.showhint();
        cout << endl;
        cout << endl;
    }
}
// Enters studymode
void studymode(mt19937_64 &rng)
{
    int n = cards.size();
    int current_id = 0;
    int action_number = 1;
    while (true)
    {
        if (action_number == 1)
        {
            cout << "NEXT LABEL is: ";

            current_id = randint(rng, 0, n - 1);
            while (cards[current_id].turns > 0)
            {
                current_id = randint(rng, 0, n - 1);
                cards[current_id].turn();
            }
            cards[current_id].reset_turn();
            cards[current_id].printlabel();
            cout << "Input 1 to show answer" << endl;
            cout << "Input 2 to show a hint" << endl;
            cout << "Input 3 to modify importance of a flashcard" << endl;
        }
        if (action_number < 4 and action_number > 0) // patch cutre
            cin >> action_number;
        if (action_number == 1)
        {
            cards[current_id].printanswer();
            cout << "---------------------------------------------" << endl;
            cout << endl;
            cout << endl;
        }
        else if (action_number == 2)
        {
            cards[current_id].showhint();
        }
        else if (action_number == 3)
        {
            cout << "Select the new importance" << endl;
            int nimp;
            cin >> nimp;
            cards[current_id].mod_importance(nimp);
            cards[current_id].reset_turn();
            cout << "Importance parameter has been modified" << endl;
            cout << "Input 1 to show answer" << endl;
            cout << "Input 2 to show a hint" << endl;
        }
        else
        {
            cout << "Please Input a valid option" << endl;
            cin >> action_number;
        }
    }
}
int main()
{
    // Random module part
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 rng(seed);
    //
    cards = fvect();
    cout << "Please specify a filepath below" << endl;
    string filepath;
    cin >> filepath;
    // read the flashcards
    read_flashcards(filepath);
    //
    cout << "succesfull read" << endl;
    printdata();
    cout << "Entering study mode:" << endl;
    studymode(rng);
}