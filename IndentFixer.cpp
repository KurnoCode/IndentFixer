#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

int main()
{
    const int perIndent = 4; // hoe veel spaces per level

    std::string inpFile;

    std::cout << "Filename?\n";
    std::getline(std::cin, inpFile);

    if (!std::filesystem::exists(inpFile))
    {
        std::cout << "Not valid.";
        return -1;
    }

    // eerst backup maken (dient ook als input):

    std::string bakFile = inpFile + ".bak";

    std::filesystem::copy_file(inpFile, bakFile, std::filesystem::copy_options::overwrite_existing);

    // input en output streams openen:

    std::ifstream inpReader;
    inpReader.open(bakFile);

    std::ofstream outpWriter;
    outpWriter.open(inpFile);

    // per regel lezen en naar output wegschrijven:

    int curIndentLevel = 0; // altijd beginnen helemaal aan de linkerkant

    std::string curLine;

    int curOffset; // huidige ident huidige regel
    std::string curLine_text; // huidige regel zonder indent characters

    std::string outLine;

    while (std::getline(inpReader, curLine))
    {
        if (curLine == "")
        {
            outpWriter << '\n'; // dat nog wel even
            continue; // lege regels overslaan
        }

        // eerst start regel (eerste non spatie/tab) vinden:

        curOffset = -1; // om regels met alleen spaties/tabs te identificeren

        for (int c = 0; c < curLine.size(); c++)
        {
            if (curLine[c] != ' ' && curLine[c] != '\t')
            {
                curOffset = c; // begin regel gevonden
                break;
            }
        }

        if (curOffset == -1) // niet leeg maar alleen maar spaties/tabs, dus leeg maken en wegschrijven:
        {
            outpWriter << "\n";
            continue;
        }

        // regel minus spaties/tabs maken:

        curLine_text = curLine.substr(curOffset, curLine.npos);

        if (curLine_text[0] == '{')
        {
            curIndentLevel += 1;
        }
        else if (curLine_text[0] == '}')
        {
            curIndentLevel -= 1;
        }

        if (curIndentLevel < 0) // ZNMK
        {
            std::cout << "indent out of bounds";

            inpReader.close(); // allebei nog wel even sluiten
            outpWriter.close();

            return -2;
        }

        // regel wegschrijven met juiste indent:

        outLine = ""; // reset!

        if (curLine_text != "{") // uitzondering voor { regels
        {
            for (int c = 0; c < curIndentLevel; c++)
            {
                for (int d = 0; d < perIndent; d++) // zo veel per indent level
                {
                    outLine = outLine + " ";
                }
            }
        }
        else 
        {
            for (int c = 0; c < curIndentLevel - 1; c++)
            {
                for (int d = 0; d < perIndent; d++)
                {
                    outLine = outLine + " ";
                }
            }
        }

        outLine = outLine + curLine_text; // dit niet vergeten!

        outpWriter << outLine << '\n';
    }

    inpReader.close();
    outpWriter.close();
}
