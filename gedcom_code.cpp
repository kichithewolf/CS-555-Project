/*
Julia Kim, Michelle Wong, Rafal Poniatowski
CS-555
GEDCOM Project
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "People.cpp"
#include "Family.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    ifstream gedcomFile;
    ofstream output;
    string gedcomLine;
    string levelNum;
    string tag;
    string tagHolder;
    string delimiter = " ";
    int level;
    //Can assume <5000 people, <1000 families
    People* listPeople = new People[5000];
    Family* listFamily = new Family[1000];
    //Will need to count #of People/Families for output later.
    int numOfPeople = 0;
    int numOfFamilies = 0;
    
    output.open("output.txt");
    
    // Check for command line args.
    if(argc != 2) {
            output << "Invalid file!" << endl;
            cout << "Invalid file!" << endl;
    }
    
    // Open file.
    gedcomFile.open(argv[1]);
    
    // Check file.
    if(!gedcomFile.is_open()) {
         output << "Failed to open file! Exiting..." << endl;
         cout << "Failed to open file! Exiting..." << endl;
         cin.ignore();
         return 0;
    }
    
    output << "-----------------------------------------------" << endl;
    output << "File \"" << argv[1] << "\" opened!"<< endl;
    output << "Reading and Printing file..." << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "File \"" << argv[1] << "\" opened!"<< endl;
    cout << "Reading and Printing file..." << endl;
    cout << "-----------------------------------------------" << endl;
    
    // Read file line by line.
    while(getline(gedcomFile, gedcomLine)) {
           //Eliminate trailing character causing string compare issue.
           gedcomLine.erase(gedcomLine.find_last_not_of("\n\r") + 1);
           output << gedcomLine << endl;
           cout << gedcomLine << endl;
           
           // Get and print level number.
           levelNum = gedcomLine.substr(0, gedcomLine.find(delimiter));
           level = atoi(levelNum.c_str());
           output << "Level Number: " << level << endl;
           cout << "Level Number: " << level << endl;
           
           // Don't care about preserving the string.
           gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
           
           // Get and print tag/invalid tag.
           tag = gedcomLine.substr(0, gedcomLine.find(delimiter));
           // Check tag/level
           
           /*
            *
            * Project 3 Note: Feel free to change the tag parsing to something better.
            *
           */

           /* BUG: tag recognition doesn't work on Ubuntu/Linux machines. Will be fixed for next project.
                   Following is debug code from an attempt to find the problem
              Program works fine on Windows environment.

           cout << tag << endl;
           output << tag << endl;
           if(tag.compare("HEAD") == 0)
             cout << "1" << endl;
           else
             cout << "2" << endl;

           if(tag.compare("HEAD ") == 0)
             cout << "3" << endl;
           else
             cout << "4" << endl;

           if(tag.compare("HEAD\n") == 0)
             cout << "5" << endl;
           else
             cout << "6" << endl;


           */

           
           if(level == 0) {
                if(!(tag.compare("HEAD") == 0) && !(tag.compare("TRLR") == 0) && !(tag.compare("NOTE") == 0)) {
                    // Check for/skip xrefid
                    // Temp storage in case we need that ID.
                    tagHolder = tag;
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tag = gedcomLine.substr(0, gedcomLine.find(delimiter));
					//cout << "check: " << tag << endl;

                    
                    if(!(tag.compare("INDI") == 0) && !(tag.compare("FAM") == 0)) {
                       tag = "Invalid tag!";
					   //cout << "check: " << tag << endl;

                    }
                    //Tag is either INDI or FAM at this point.
                    if((tag.compare("INDI") == 0)) {
                       listPeople[numOfPeople].IDNumber = numOfPeople;
                       listPeople[numOfPeople].uniqueID = tagHolder;
                       numOfPeople++; //+1 Person
                    }
                    if((tag.compare("FAM") == 0)) {
                       listFamily[numOfFamilies].IDNumber = numOfFamilies;
                       listFamily[numOfFamilies].familyID = tagHolder;
                       numOfFamilies++; //+1 Family
                    }
					//cout << "check: " << tag << endl;

                }
           } else if (level == 1) {
                if(!(tag.compare("NAME") == 0) &&
                   !(tag.compare("SEX") == 0) &&
                   !(tag.compare("BIRT") == 0)&&
                   !(tag.compare("DEAT") == 0) &&
                   !(tag.compare("FAMC") == 0) &&
                   !(tag.compare("FAMS") == 0) &&
                   !(tag.compare("MARR") == 0) &&
                   !(tag.compare("HUSB") == 0) &&
                   !(tag.compare("WIFE") == 0) &&
                   !(tag.compare("CHIL") == 0) &&
                   !(tag.compare("DIV") == 0)) {
                   tag = "Invalid tag!";
                }
                //Tag is NAME, SEX, BIRT, DEAT, FAMC, FAMS, MARR, HUSB, WIFE, CHIL, or DIV.
                if((tag.compare("NAME") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tag = gedcomLine.substr(0);
                   numOfPeople--; //There has got to be a better way to do this.
                   listPeople[numOfPeople].peopleName = tag;
                   numOfPeople++; //There has got to be a better way to do this.
                }
                if((tag.compare("HUSB") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tag = gedcomLine.substr(0);
                   
                   for(int i = 0; i < numOfPeople; i++) {
                           if(tag.compare(listPeople[i].uniqueID) == 0) {
                                numOfFamilies--; //There has got to be a better way to do this.
                                listFamily[numOfFamilies].husbando = listPeople[i].peopleName;
                                numOfFamilies++; //There has got to be a better way to do this.
                           
                           }
                   }
                }
                if((tag.compare("WIFE") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tag = gedcomLine.substr(0);
                   
                   for(int i = 0; i < numOfPeople; i++) {
                           if(tag.compare(listPeople[i].uniqueID) == 0) {
                                numOfFamilies--; //There has got to be a better way to do this.
                                listFamily[numOfFamilies].waifu = listPeople[i].peopleName;
                                numOfFamilies++; //There has got to be a better way to do this.
                           
                           }
                   }
                }
           } else if (level == 2) {
                if(!(tag.compare("DATE") == 0)) {
                   tag = "Invalid tag!";
                }
           }
           
           output << "Tag: " << tag << endl;
           cout << "Tag: " << tag << endl;
           
           // Make output look neat and pretty.
           output << "---------" << endl;
           cout << "---------" << endl;
    }
    
    //Print people and Family

    output << "-----------------------------------------------" << endl;
    output << "People" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "People" << endl;
    cout << "-----------------------------------------------" << endl;
    
    output << "ID\tUID\tName\t\t" << endl;
    cout << "ID\tUID\tName\t\t" << endl;
    
    for(int i = 0; i < numOfPeople; i++) {
        output << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << endl;
        cout << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << endl;
    }

    output << "-----------------------------------------------" << endl;
    output << "Family" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "Family"<< endl;
    cout << "-----------------------------------------------" << endl;
    
    output << "ID\tUID\tHusb\t\tWife\t\t" << endl;
    cout << "ID\tUID\tHusb\t\tWife\t\t" << endl;

    for(int i = 0; i < numOfFamilies; i++) {
        output << listFamily[i].IDNumber << "\t" << listFamily[i].familyID << "\t" << listFamily[i].husbando << "\t" << listFamily[i].waifu << endl;
        cout << listFamily[i].IDNumber << "\t" << listFamily[i].familyID << "\t" << listFamily[i].husbando << "\t" << listFamily[i].waifu << endl;
    }
    
    gedcomFile.close();
    output.close();

    cin.ignore();
    return 0;
}
