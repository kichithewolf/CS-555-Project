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
#include "Dates.h"

using namespace std;

//compare dates; return true if first date occurs before second and false if the dates are equal or the second occurs first
bool compareDates(num_date date1, num_date date2) {
    if(date1.year != 0 && date2.year !=0) {
        if(date1.year > date2.year || (date1.year == date2.year && date1.month > date2.month) || (date1.year == date2.year && date1.month == date2.month && date1.day > date2.day)) {
        	return false;
		}
		return true;
	} else {
		return false;
	}

};

int monthToInteger(string monthString) {
    string allMonthStrings[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
    for(int i = 0; i < 12; i++) {
          if(monthString == allMonthStrings[i]) {
                         return (i+1);
          }
    }
};

//parse date string to create num_date object
num_date strToNumDate(string date) {
    num_date result;

    result.day = atoi((date.substr(0, date.find(" "))).c_str());
    date.erase(0, date.find(" ")+1);
    result.month = monthToInteger(date.substr(0, date.find(" ")).c_str());
    date.erase(0, date.find(" ")+1);
    result.year = atoi(date.substr(0, date.find(" ")).c_str());
    
    cout << "Converting date: " << result.year << " " << result.month << " " << result.day << endl;
    
    return result;
};

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
                    } else if((tag.compare("FAM") == 0)) {
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
               } else if((tag.compare("BIRT") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tag = gedcomLine.substr(0);
                   numOfPeople--;
                   listPeople[numOfPeople].setBirthFlag(true);
                   numOfPeople++;
                } else if((tag.compare("DEAT") == 0)) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tag = gedcomLine.substr(0);
                    numOfPeople--;
                    listPeople[numOfPeople].setDeathFlag(true);
                    numOfPeople++;
                } else if((tag.compare("MARR") == 0)) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tag = gedcomLine.substr(0);
                    numOfFamilies--;
                    listFamily[numOfFamilies].setMarryFlag(true);
                    numOfFamilies++;
                } else if((tag.compare("HUSB") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tag = gedcomLine.substr(0);
                   
                   for(int i = 0; i < numOfPeople; i++) {
                           if(tag.compare(listPeople[i].uniqueID) == 0) {
                                numOfFamilies--; //There has got to be a better way to do this.
                                listFamily[numOfFamilies].husbando = listPeople[i].peopleName;
                                listFamily[numOfFamilies].husbandoID = listPeople[i].uniqueID;
                                numOfFamilies++; //There has got to be a better way to do this.
                           
                           }
                   }
                } else if((tag.compare("WIFE") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tag = gedcomLine.substr(0);
                   
                   for(int i = 0; i < numOfPeople; i++) {
                           if(tag.compare(listPeople[i].uniqueID) == 0) {
                                numOfFamilies--; //There has got to be a better way to do this.
                                listFamily[numOfFamilies].waifu = listPeople[i].peopleName;
                                listFamily[numOfFamilies].waifuID = listPeople[i].uniqueID;
                                numOfFamilies++; //There has got to be a better way to do this.
                           
                           }
                   }
                } else if((tag.compare("DIV") == 0)) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tag = gedcomLine.substr(0);
                    numOfFamilies--;
                    listFamily[numOfFamilies].setDivFlag(true);
                    numOfFamilies++;
                }
           } else if (level == 2) {
                if(!(tag.compare("DATE") == 0)) {
                   tag = "Invalid tag!";
                } else {
                	//Decrement/Increment before doing things to prevent crashes.
	                numOfPeople--;
	                numOfFamilies--;
	                //Dates for things.
	                if(listPeople[numOfPeople].getBirthFlag() == true) {
	                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
	                    tag = gedcomLine.substr(0);
	                    cout << "birth: " << tag << endl;
	                    listPeople[numOfPeople].birthDate = tag;
	                    //Parse string into integers while we're at it.
	                    listPeople[numOfPeople].birthInt = strToNumDate(tag);
	                } else if(listPeople[numOfPeople].getDeathFlag() == true) {
	                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
	                    tag = gedcomLine.substr(0);
	                    cout << "death: " << tag << endl;
	                    listPeople[numOfPeople].deathDate = tag;
	                    //Parse string into integers while we're at it.
	                    listPeople[numOfPeople].deathInt = strToNumDate(tag);
	                } else if(listFamily[numOfFamilies].getMarryFlag() == true) {
	                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
	                    tag = gedcomLine.substr(0);
	                    cout << "marry: " << tag << endl;
	                    listFamily[numOfFamilies].marryDate = tag;
	                    //Set false again just in case.
	                    //Parse string into integers while we're at it.
	                    listFamily[numOfFamilies].marryInt = strToNumDate(tag);
	                } else if(listFamily[numOfFamilies].getDivFlag() == true) {
	                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
	                    tag = gedcomLine.substr(0);
	                    cout << "div: " << tag << endl;
	                    listFamily[numOfFamilies].divDate = tag;
	                    //Set false again just in case.
	                    //Parse string into integers while we're at it.
	                    listFamily[numOfFamilies].divInt = strToNumDate(tag);
	                }
	                numOfPeople++;
	                numOfFamilies++;
	                //Reset tag for initial printout.
	                tag = "DATE";
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
    
    output << "ID\tUID\tName\t\tDeath\t" << endl;
    cout << "ID\tUID\tName\t\tDeath\t" << endl;
    
    for(int i = 0; i < numOfPeople; i++) {
        output << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << listPeople[i].deathDate << "\t" << endl;
        cout << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << listPeople[i].deathDate << "\t" << endl;
    }

    output << "-----------------------------------------------" << endl;
    output << "Family" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "Family" << endl;
    cout << "-----------------------------------------------" << endl;
    
    output << "ID\tUID\tHusb\t\tWife\t\tMarry\t\tDiv\t" << endl;
    cout << "ID\tUID\tHusb\t\tWife\t\tMarry\t\tDiv\t" << endl;

    for(int i = 0; i < numOfFamilies; i++) {
        output << listFamily[i].IDNumber << "\t" << listFamily[i].familyID << "\t" << listFamily[i].husbando << "\t" << listFamily[i].waifu << "\t" << listFamily[i].marryDate << "\t" << listFamily[i].divDate << endl;
        cout << listFamily[i].IDNumber << "\t" << listFamily[i].familyID << "\t" << listFamily[i].husbando << "\t" << listFamily[i].waifu << "\t" << listFamily[i].marryDate << "\t" << listFamily[i].divDate <<endl;
    }
    
    //Error checking for gedcom file.
    
    output << "-----------------------------------------------" << endl;
    output << "Errors" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "Errors" << endl;
    cout << "-----------------------------------------------" << endl;
    
    for(int i = 0; i < numOfFamilies; i++) {
        // Must marry before divorce
        if(listFamily[i].getMarryFlag() && listFamily[i].getDivFlag()){
        	cout << "compare date: "<< compareDates(listFamily[i].marryInt, listFamily[i].divInt) << "; famid: "<< listFamily[i].familyID << endl;
        	cout << "Marriage: " << listFamily[i].marryInt.year << listFamily[i].marryInt.month << listFamily[i].marryInt.day << "; Divorce: " << listFamily[i].divInt.year << listFamily[i].divInt.month << listFamily[i].divInt.day << endl;
	        if(compareDates(listFamily[i].marryInt, listFamily[i].divInt)) {
	           output << "Error: Marriage is after divorce in family: " << listFamily[i].familyID << endl;
	           cout << "Error: Marriage is after divorce in family: " << listFamily[i].familyID << endl;
	        }
		} else {
			cout << "married: " << listFamily[i].getMarryFlag() << endl;
			cout << "divorced: " << listFamily[i].getDivFlag() << endl;
		}

        for(int j = 0; j < numOfPeople; j++) {
                //Must have birth before death
                if (listPeople[j].getDeathFlag() && listPeople[j].getBirthFlag()) {
                	cout << listPeople[j].birthDate << endl;
                   if(compareDates(listPeople[j].birthInt, listPeople[j].deathInt)) {
                       output << "Error: Birth is after person's Death: " << listFamily[i].familyID << "; Birth: " << listPeople[j].birthDate << "; Death: " << listPeople[j].deathDate << endl;
                       cout << "Error: Birth is after person's Death: " << listFamily[i].familyID << "; Birth: " << listPeople[j].birthDate << "; Death: " << listPeople[j].deathDate << endl;
                   }
                }
                //Must have been born before marrying

                if(listFamily[i].husbandoID == listPeople[j].uniqueID) {
                     // Must have been born before marriage
                      if(compareDates(listPeople[j].birthInt, listFamily[i].marryInt)) {
                         output << "Error: Husband's birth is after Marriage in family: " << listFamily[i].familyID << endl;
                         cout << "Error: Husband's birth is after Marriage in family: " << listFamily[i].familyID << endl;
                      }
                     // Must marry before die
                     if(compareDates(listFamily[i].marryInt, listPeople[j].deathInt)) {
                        output << "Error: Marriage is after Husband's death in family: " << listFamily[i].familyID << endl;
                        cout << "Error: Marriage is after Husband's death in family: " << listFamily[i].familyID << endl;
                     }
                }
                if(listFamily[i].waifuID == listPeople[j].uniqueID) {
                      // Must have been born before marriage
                      if(compareDates(listPeople[j].birthInt, listFamily[i].marryInt)) {
                         output << "Error: Husband's birth is after Marriage in family: " << listFamily[i].familyID << endl;
                         cout << "Error: Husband's birth is after Marriage in family: " << listFamily[i].familyID << endl;
                      }
                    // Must marry before die
                    if(compareDates(listFamily[i].marryInt, listPeople[j].deathInt)) {
                        output << "Error: Marriage is after Wife's death in family: " << listFamily[i].familyID << endl;
                        cout << "Error: Marriage is after Wife's death in family: " << listFamily[i].familyID << endl;
                    }
                }
        }
    }
    
    
    gedcomFile.close();
    output.close();

    cin.ignore();
    return 0;
}
