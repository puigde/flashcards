#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>
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

private:
    double importance = 1; // indicates the importance a flashcard has, 1 default, 2 twice more importance, etc.

    // implement penalization for missed answers so harder concepts get consolidated
    void mod_importance(double factor)
    {
        importance *= factor;
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
            cards[current_id].printlabel();
            cout << "Input 1 to show answer" << endl;
            cout << "Input 2 to show a hint" << endl;
        }
        cin >> action_number;
        if (action_number == 1)
        {
            cards[current_id].printanswer();
            cout << "---------------------------------------------" << endl;
            cout << endl;
            cout << endl;
        }
        else
        {
            cards[current_id].showhint();
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