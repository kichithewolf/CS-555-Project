/*
Julia Kim, Michelle Wong, Rafal Poniatowski
CS-555
GEDCOM Project
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <list>
#include <sstream>
#include <vector>
#include "People.cpp"
#include "Family.cpp"
#include "Dates.h"

#define CURRYEAR 2016
#define CURRMONTH 10
#define CURRDAY 30

using namespace std;

int monthToInteger(string monthString) {
    string allMonthStrings[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
    for(int i = 0; i < 12; i++) {
            if(monthString == allMonthStrings[i]) {
                           return (i+1);
            }
    }
}

bool checkIfBefore(num_date date1, num_date date2) {
	if (date1.year > date2.year || (date1.year == date2.year && date1.month > date2.month) || (date1.year == date2.year && date1.month == date2.month && date1.day > date2.day))
		return false;
	return true;
}

int main(int argc, char *argv[]) {
    ifstream gedcomFile;
    ofstream output;
    string gedcomLine;
    string levelNum;
    string tag;
    string tagHolder;
    string tagP;
    string delimiter = " ";
    int level;
    //Can assume <5000 people, <1000 families
    People* listPeople = new People[5000];
    Family* listFamily = new Family[1000];
    //Will need to count #of People/Families for output later.
    int numOfPeople = 0;
    int numOfFamilies = 0;
    map<string, string> familyNames;
    map<string, list<Family> > families;
    map<string, list<Family> >::iterator it;
    map<string, string> nameIDList; //used to check that all IDs are unique
    
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
    
    /*
    output << "-----------------------------------------------" << endl;
    output << "File \"" << argv[1] << "\" opened!"<< endl;
    output << "Reading and Printing file..." << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "File \"" << argv[1] << "\" opened!"<< endl;
    cout << "Reading and Printing file..." << endl;
    cout << "-----------------------------------------------" << endl;
    */
    
    // Read file line by line.
    while(getline(gedcomFile, gedcomLine)) {
           //Eliminate trailing character causing string compare issue.
           gedcomLine.erase(gedcomLine.find_last_not_of("\n\r") + 1);
           //output << gedcomLine << endl;
           //cout << gedcomLine << endl;
           
           // Get and print level number.
           levelNum = gedcomLine.substr(0, gedcomLine.find(delimiter));
           level = atoi(levelNum.c_str());
           //output << "Level Number: " << level << endl;
           //cout << "Level Number: " << level << endl;
           
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
                       listFamily[numOfFamilies].numOfChild = 0;
                       listFamily[numOfFamilies].listChild = new Child[5000];
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
                   tagP = gedcomLine.substr(0);
                   numOfPeople--; //There has got to be a better way to do this.
                   listPeople[numOfPeople].peopleName = tagP;
                   numOfPeople++; //There has got to be a better way to do this.
                } else if((tag.compare("SEX") == 0)) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tagP = gedcomLine.substr(0);
                    numOfPeople--;
                    listPeople[numOfPeople].sex = tagP;
                    if((tagP.compare("M") == 0)) 
                        listPeople[numOfPeople].sexFlag = true;
                    else 
                        listPeople[numOfPeople].sexFlag = false;
                    numOfPeople++; 
                } else if((tag.compare("BIRT") == 0)) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tagP = gedcomLine.substr(0);
                    numOfPeople--;
                    listPeople[numOfPeople].birthFlag = true;
                    numOfPeople++;
                } else if((tag.compare("DEAT") == 0)) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tagP = gedcomLine.substr(0);
                    numOfPeople--;
                    listPeople[numOfPeople].deathFlag = true;
                    numOfPeople++;
                } else if((tag.compare("MARR") == 0)) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tagP = gedcomLine.substr(0);
                    numOfFamilies--;
                    listFamily[numOfFamilies].marryFlag = true;
                    numOfFamilies++;
                } else if((tag.compare("HUSB") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tagP = gedcomLine.substr(0);
                   
                   for(int i = 0; i < numOfPeople; i++) {
                           if(tagP.compare(listPeople[i].uniqueID) == 0) {
                                numOfFamilies--; //There has got to be a better way to do this.
                                listFamily[numOfFamilies].husbando = listPeople[i].peopleName;
                                listFamily[numOfFamilies].husbandoID = listPeople[i].uniqueID;
                                numOfFamilies++; //There has got to be a better way to do this.
                           
                           }
                   }
                } else if((tag.compare("WIFE") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tagP = gedcomLine.substr(0);
                   
                   for(int i = 0; i < numOfPeople; i++) {
                           if(tagP.compare(listPeople[i].uniqueID) == 0) {
                                numOfFamilies--; //There has got to be a better way to do this.
                                listFamily[numOfFamilies].waifu = listPeople[i].peopleName;
                                listFamily[numOfFamilies].waifuID = listPeople[i].uniqueID;
                                numOfFamilies++; //There has got to be a better way to do this.
                           
                           }
                   }
                } else if((tag.compare("CHIL") == 0)) {
                   gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                   tagP = gedcomLine.substr(0);
                   
                   for(int i = 0; i < numOfPeople; i++) {
                           if(tagP.compare(listPeople[i].uniqueID) == 0) {
                                numOfFamilies--; //There has got to be a better way to do this.
                                
                                listFamily[numOfFamilies].listChild[listFamily[numOfFamilies].numOfChild].child = listPeople[i].peopleName;
                                listFamily[numOfFamilies].listChild[listFamily[numOfFamilies].numOfChild].childID = listPeople[i].uniqueID;
                                //You know what this is way faster and easier.
                                listFamily[numOfFamilies].listChild[listFamily[numOfFamilies].numOfChild].childBirthday = listPeople[i].birthInt;
                                listFamily[numOfFamilies].numOfChild++;
                                
                                numOfFamilies++; //There has got to be a better way to do this.
                           }
                   }
                } else if((tag.compare("DIV") == 0)) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tagP = gedcomLine.substr(0);
                    numOfFamilies--;
                    listFamily[numOfFamilies].divFlag = true;
                    numOfFamilies++;
                }
           } else if (level == 2) {
                if(!(tag.compare("DATE") == 0)) {
                   tag = "Invalid tag!";
                }
                //Decrement/Increment before doing things to prevent crashes.
                numOfPeople--;
                numOfFamilies--;
                //Dates for things.
                if(listPeople[numOfPeople].birthFlag == true) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tag = gedcomLine.substr(0);
                    listPeople[numOfPeople].birthDate = tag;
                    //Set false again just in case.
                    listPeople[numOfPeople].birthFlag = false;
                    //Parse string into integers while we're at it.
                    listPeople[numOfPeople].birthInt.day = atoi((tag.substr(0, tag.find(" "))).c_str());
                    tag.erase(0,tag.find(" ")+1);
                    listPeople[numOfPeople].birthInt.month = monthToInteger(tag.substr(0, tag.find(" ")).c_str());
                    tag.erase(0,tag.find(" ")+1);
                    listPeople[numOfPeople].birthInt.year = atoi(tag.substr(0, tag.find(" ")).c_str());
                } else if(listPeople[numOfPeople].deathFlag == true) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tag = gedcomLine.substr(0);
                    listPeople[numOfPeople].deathDate = tag;
                    //Set false again just in case.
                    listPeople[numOfPeople].deathFlag = false;
                    //Parse string into integers while we're at it.
                    listPeople[numOfPeople].deathInt.day = atoi((tag.substr(0, tag.find(" "))).c_str());
                    tag.erase(0,tag.find(" ")+1);
                    listPeople[numOfPeople].deathInt.month = monthToInteger(tag.substr(0, tag.find(" ")).c_str());
                    tag.erase(0,tag.find(" ")+1);
                    listPeople[numOfPeople].deathInt.year = atoi(tag.substr(0, tag.find(" ")).c_str());
                } else if(listFamily[numOfFamilies].marryFlag == true) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tag = gedcomLine.substr(0);
                    listFamily[numOfFamilies].marryDate = tag;
                    //Set false again just in case.
                    listFamily[numOfFamilies].marryFlag = false;
                    //Parse string into integers while we're at it.
                    listFamily[numOfFamilies].marryInt.day = atoi((tag.substr(0, tag.find(" "))).c_str());
                    tag.erase(0,tag.find(" ")+1);
                    listFamily[numOfFamilies].marryInt.month = monthToInteger(tag.substr(0, tag.find(" ")).c_str());
                    tag.erase(0,tag.find(" ")+1);
                    listFamily[numOfFamilies].marryInt.year = atoi(tag.substr(0, tag.find(" ")).c_str());
                } else if(listFamily[numOfFamilies].divFlag == true) {
                    gedcomLine.erase(0, gedcomLine.find(delimiter)+1);
                    tag = gedcomLine.substr(0);
                    listFamily[numOfFamilies].divDate = tag;
                    //Set false again just in case.
                    listFamily[numOfFamilies].divFlag = false;
                    //Parse string into integers while we're at it.
                    listFamily[numOfFamilies].divInt.day = atoi((tag.substr(0, tag.find(" "))).c_str());
                    tag.erase(0,tag.find(" ")+1);
                    listFamily[numOfFamilies].divInt.month = monthToInteger(tag.substr(0, tag.find(" ")).c_str());
                    tag.erase(0,tag.find(" ")+1);
                    listFamily[numOfFamilies].divInt.year = atoi(tag.substr(0, tag.find(" ")).c_str());
                }
                numOfPeople++;
                numOfFamilies++;
                //Reset tag for initial printout.
                tag = "DATE";
           }
           
           //output << "Tag: " << tag << endl;
           //cout << "Tag: " << tag << endl;
           
           // Make output look neat and pretty.
           //output << "---------" << endl;
           //cout << "---------" << endl;
    }
    
    //List dead people - please keep this before printing people (US29)
    output << "-----------------------------------------------" << endl;
    output << "US29: Hall of the Dead" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "US29: Hall of the Dead" << endl;
    cout << "-----------------------------------------------" << endl;
    
    for(int i = 0; i < numOfPeople; i++) {
            //If the death date is filled in then person is dead.
            if(listPeople[i].deathDate[0] != '\0') {
                output << listPeople[i].peopleName << endl;
                cout << listPeople[i].peopleName << endl;
                //Also calculate age (For US27)
                listPeople[i].age = listPeople[i].deathInt.year - listPeople[i].birthInt.year;
            } else {
                //Calculate age if not dead
                listPeople[i].age = CURRYEAR - listPeople[i].birthInt.year;
            }
    }
    
    //List living and married people - please keep this before printing people (US30)
    output << "-----------------------------------------------" << endl;
    output << "US30: Hall of the Living and Married" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "US30: Hall of the Living and Married" << endl;
    cout << "-----------------------------------------------" << endl;
    //these loops and ifs are horrendous... im sorry.
    for(int i = 0; i < numOfFamilies; i++) {
            //show married but not divorced
            if((listFamily[i].marryDate[0] != '\0') && (listFamily[i].divDate[0] == '\0')){           
                for(int j = 0; j < numOfPeople; j++){
                        //show both husband and wife
                        if((listFamily[i].husbandoID == listPeople[j].uniqueID) || (listFamily[i].waifuID == listPeople[j].uniqueID)){
                               //show only if alive                           
                               if(listPeople[j].deathDate[0] == '\0') {
                                   output << listPeople[j].peopleName << endl;
                                   cout << listPeople[j].peopleName << endl;
                                   }
                        }
                }
            }           
    }
    
    //List living and single people - please keep this before printing people (US31)
    output << "-----------------------------------------------" << endl;
    output << "US31: Hall of the Living and Single" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "US31: Hall of the Living and Single" << endl;
    cout << "-----------------------------------------------" << endl;
    
    for(int i = 0; i < numOfPeople; i++) {
            //if person was ever husband or wife, set the married flag
            bool mFlag = false;
            for(int j = 0; j < numOfFamilies; j++)
                    if((listFamily[j].husbandoID == listPeople[i].uniqueID) || (listFamily[j].waifuID == listPeople[i].uniqueID))
                           mFlag = true;       
            //show people that did set have the married flag
            if(!(mFlag)) {             
                    //output alive and over 30             
                    if((listPeople[i].deathDate[0] == '\0') && (listPeople[i].age > 30)) {
                           output << listPeople[i].peopleName << endl;
                           cout << listPeople[i].peopleName << endl;
                    }
            }
    }
    
    //List birthdays in next 30 days (US38)
    output << "-----------------------------------------------" << endl;
    output << "US38: Upcoming Birthdays" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "US38: Upcoming Birthdays" << endl;
    cout << "-----------------------------------------------" << endl;
    
    num_date tempCurrDate;
    for(int i = 0; i < numOfPeople; i++) {
            //If the death date is filled in then person is dead.
            //This means they can't celebrate!
            if(listPeople[i].deathDate[0] == '\0') {
                //Reset date
                tempCurrDate.day = CURRDAY;
                tempCurrDate.month = CURRMONTH;
                tempCurrDate.year = CURRYEAR;

                //Check next 30 days
                //cout << "NEW TARGET ==================" << endl;
                //output << "NEW TARGET ==================" << endl;
                for(int j = 0; j < 30; j++) {
                        //cout << "TARG DATE: " << listPeople[i].birthInt.month << "-" << listPeople[i].birthInt.day << endl;
                        //output << "TARG DATE: " << listPeople[i].birthInt.month << "-" << listPeople[i].birthInt.day << endl;
                        
                    if((listPeople[i].birthInt.month == tempCurrDate.month && listPeople[i].birthInt.day > tempCurrDate.day)){
                     //Print name
                        output << listPeople[i].peopleName << endl;
                        cout << listPeople[i].peopleName << endl;
                        break;
                    } else {
                        //cout << "TEMP CURR DATE: " << tempCurrDate.month << "-" << tempCurrDate.day << endl;
                        //output << "TEMP CURR DATE: " << tempCurrDate.month << "-" << tempCurrDate.day << endl;
                           
                      //Increment day
                      tempCurrDate.day++;
                      //Check month rollover
                      if(tempCurrDate.month == 2 && tempCurrDate.day > 28) {
                         tempCurrDate.day = 1;
                         tempCurrDate.month++;
                      } else if((tempCurrDate.month == 1 || tempCurrDate.month == 3 || tempCurrDate.month == 5 || tempCurrDate.month == 7 || tempCurrDate.month == 8 || tempCurrDate.month == 10 || tempCurrDate.month == 12)&& tempCurrDate.day > 31) {
                        tempCurrDate.day = 1;
                        tempCurrDate.month++;
                        if(tempCurrDate.month > 12) {
                           tempCurrDate.year++;
                           tempCurrDate.month = 1;
                        }
                      } else if((tempCurrDate.month == 4 || tempCurrDate.month == 6 || tempCurrDate.month == 9 || tempCurrDate.month == 11) && tempCurrDate.day > 30) {
                         tempCurrDate.day = 1;
                         tempCurrDate.month++;
                      }
                   }
                }
            }
    }
    
    //Print people and Family

    output << "-----------------------------------------------------------------------------" << endl;
    output << "People" << endl;
    output << "-----------------------------------------------------------------------------" << endl;
    
    cout << "-------------------------------------------------------------------------------" << endl;
    cout << "People" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    
    output << "ID\tUID\tName\tSex\tAge\tBirth\t\tDeath" << endl;
    cout << "ID\tUID\tName\t\tSex\tAge\tBirth\t\tDeath" << endl;
    
    for(int i = 0; i < numOfPeople; i++) {
        output << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << listPeople[i].sex << "\t" << listPeople[i].age << "\t" << listPeople[i].birthDate << "\t" << listPeople[i].deathDate << endl;
        cout << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << listPeople[i].sex << "\t" << listPeople[i].age << "\t" << listPeople[i].birthDate << "\t" << listPeople[i].deathDate << endl;
    }

    output << "-----------------------------------------------------------------------------" << endl;
    output << "Family" << endl;
    output << "-----------------------------------------------------------------------------" << endl;
    
    cout << "-------------------------------------------------------------------------------" << endl;
    cout << "Family" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    
    output << "ID\tUID\tHusb\t\tWife\t\tMarry\t\tDiv\t" << endl;
    cout << "ID\tUID\tHusb\t\tWife\t\tMarry\t\tDiv\t" << endl;

    for(int i = 0; i < numOfFamilies; i++) {
        output << listFamily[i].IDNumber << "\t" << listFamily[i].familyID << "\t" << listFamily[i].husbando << "\t" << listFamily[i].waifu << "\t" << listFamily[i].marryDate << "\t" << listFamily[i].divDate << endl;
        cout << listFamily[i].IDNumber << "\t" << listFamily[i].familyID << "\t" << listFamily[i].husbando << "\t" << listFamily[i].waifu << "\t" << listFamily[i].marryDate << "\t" << listFamily[i].divDate << endl;
    }
    
    // List Recent Births, Recent Deaths (people born and people who died in the last 30 days)
    output << "-----------------------------------------------------------------------------" << endl;
    output << "Recent Births and Deaths (within last 30 days)" << endl;
    output << "-----------------------------------------------------------------------------" << endl;
    
    cout << "-------------------------------------------------------------------------------" << endl;
    cout << "Recent Births and Deaths (within last 30 days)" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    
    output << "ID\tUID\tName\tSex\tAge\tBirth\t\tDeath" << endl;
    cout << "ID\tUID\tName\t\tSex\tAge\tBirth\t\tDeath" << endl;
    
    for(int i = 0; i < numOfPeople; i++) {
    	//get current date
    	time_t t = time(0);
    	struct tm * now = localtime( & t );
    	num_date current;
    	current.day = now->tm_mday;
    	current.month = now->tm_mon + 1;
    	current.year = now->tm_year + 1900;
    	//get 30 days from now date
		now->tm_mday -= 30;
		mktime(now);
		num_date monthBefore;
		monthBefore.day = now->tm_mday;
    	monthBefore.month = now->tm_mon + 1;
    	monthBefore.year = now->tm_year + 1900;
		if(checkIfBefore(listPeople[i].birthInt, current) && !checkIfBefore(listPeople[i].birthInt, monthBefore)) {
			output << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << listPeople[i].sex << "\t" << listPeople[i].age << "\t(" << listPeople[i].birthDate << ")\t " << listPeople[i].deathDate << endl;
	        cout << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << listPeople[i].sex << "\t" << listPeople[i].age << "\t(" << listPeople[i].birthDate << ")\t " << listPeople[i].deathDate << endl;
         }
        if(checkIfBefore(listPeople[i].deathInt, current) && !checkIfBefore(listPeople[i].deathInt, monthBefore)) {
			output << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << listPeople[i].sex << "\t" << listPeople[i].age << "\t " << listPeople[i].birthDate << "\t(" << listPeople[i].deathDate << ")" << endl;
	        cout << "" << listPeople[i].IDNumber << "\t" << listPeople[i].uniqueID << "\t"<< listPeople[i].peopleName << "\t" << listPeople[i].sex << "\t" << listPeople[i].age << "\t " << listPeople[i].birthDate << "\t(" << listPeople[i].deathDate << ")" << endl;
         }	
    }
    
    //Error checking for gedcom file. Error # corresponds to User Story Requirement
    
    output << "-----------------------------------------------" << endl;
    output << "Errors" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "Errors" << endl;
    cout << "-----------------------------------------------" << endl;
    
    for(int i = 0; i < numOfFamilies; i++) {
        // Must marry before divorce
        if(listFamily[i].divInt.year != 0) {
           if(listFamily[i].marryInt.year > listFamily[i].divInt.year || (listFamily[i].marryInt.year == listFamily[i].divInt.year && listFamily[i].marryInt.month > listFamily[i].divInt.month) || (listFamily[i].marryInt.year == listFamily[i].divInt.year && listFamily[i].marryInt.month == listFamily[i].divInt.month && listFamily[i].marryInt.day > listFamily[i].divInt.day)) {
                 output << "Error 04: Marriage is after divorce in family: " << listFamily[i].familyID << endl;
                 cout << "Error 04: Marriage is after divorce in family: " << listFamily[i].familyID << endl;
           }
        }
        
        //No more than 5 children born on the same day.
        if(listFamily[i].numOfChild > 5) {
               //More than 5 kids, check birthdays.
               int sameBDay = 0;
               //LOOPS WHY
               for(int k = 0; k < listFamily[i].numOfChild; k++) {
                       for(int l = 0; l < listFamily[i].numOfChild; l++) {
                               //Prevent checking same kid.
                               if(listFamily[i].listChild[k].childID != listFamily[i].listChild[l].childID) {
                                   if((listFamily[i].listChild[k].childBirthday.year == listFamily[i].listChild[l].childBirthday.year) && (listFamily[i].listChild[k].childBirthday.month == listFamily[i].listChild[l].childBirthday.month) && (listFamily[i].listChild[k].childBirthday.day == listFamily[i].listChild[l].childBirthday.day)) {
                                      sameBDay++;
                                      break;
                                   }
                               }
                       }
                       
               }
               cout << "Error 14: " << sameBDay << " kids born at same time in " << listFamily[i].familyID << endl;
               output << "Error 14: " << sameBDay << " kids born at same time in " << listFamily[i].familyID << endl;
        }
        		
        for(int j = 0; j < numOfPeople; j++) {
        	//IDs must be unique
        	if(nameIDList.find(listPeople[j].uniqueID) == nameIDList.end()) {
//        		cout << "Error 22: " << listPeople[j].peopleName << " adding ID:  " << listPeople[j].uniqueID << endl;
        		nameIDList[listPeople[j].uniqueID] = listPeople[j].peopleName;
			} else if (nameIDList.find(listPeople[j].uniqueID)->second != listPeople[j].peopleName) {
				output << "Error 22: " << listPeople[j].peopleName << "'s ID is not unique. ID: " << listPeople[j].uniqueID << endl;
	            cout << "Error 22: " << listPeople[j].peopleName << "'s ID is not unique ID: " << listPeople[j].uniqueID << endl;
			}
			
        	//Age must be less than 150 years old
        	if(listPeople[j].age > 149) {
	            output << "Error 07: " << listPeople[j].peopleName << "'s age is bigger than 150 (" << listPeople[j].age << ")" << endl;
	            cout << "Error 07: " << listPeople[j].peopleName << "'s age is bigger than 150 (" << listPeople[j].age << ")" << endl;
	        }
         
         	//Cannot Marry if currently married
        	if(listFamily[i].husbandoID == listPeople[j].uniqueID && listFamily[i].marryInt.year != 0) {
				families[listPeople[j].uniqueID].push_back(listFamily[i]);
				it = families.find(listPeople[j].uniqueID);
				if(it != families.end()) {
					list<Family> famList = families.find(listPeople[j].uniqueID)->second;
					for(list<Family>::iterator it2 = famList.begin(); it2 != famList.end(); it2++) {
						Family otherFam = *it2;
						Family currentFam = listFamily[i];
						if(checkIfBefore(currentFam.marryInt, otherFam.marryInt)) {
							if(currentFam.divInt.year == 0 || !checkIfBefore(currentFam.divInt, otherFam.marryInt)){
								output << "Error 11: Husband remarried during another marriage: " << listFamily[i].familyID << endl;
                 				cout << "Error 11: Husband remarried during another marriage: " << listFamily[i].familyID << endl;
							}
						} else {
							if(otherFam.divInt.year == 0 || !checkIfBefore(otherFam.divInt, currentFam.marryInt)){
								output << "Error 11: Husband remarried during another marriage: " << listFamily[i].familyID << endl;
                 				cout << "Error 11: Husband remarried during another marriage: " << listFamily[i].familyID << endl;
							}
						}
					}
				}
			}
			if(listFamily[i].waifuID == listPeople[j].uniqueID && listFamily[i].marryInt.year != 0) {
				families[listPeople[j].uniqueID].push_back(listFamily[i]);
				it = families.find(listPeople[j].uniqueID);
				if(it != families.end()) {
					list<Family> famList = families.find(listPeople[j].uniqueID)->second;
					for(list<Family>::iterator it2 = famList.begin(); it2 != famList.end(); it2++) {
						Family otherFam = *it2;
						Family currentFam = listFamily[i];
						if(checkIfBefore(currentFam.marryInt, otherFam.marryInt)) {
							if(currentFam.divInt.year == 0 || !checkIfBefore(currentFam.divInt, otherFam.marryInt)){
								output << "Error 11: Wife remarried during another marriage: " << listFamily[i].familyID << endl;
                 				cout << "Error 11: Wife remarried during another marriage: " << listFamily[i].familyID << endl;
							}
						} else {
							if(otherFam.divInt.year == 0 || !checkIfBefore(otherFam.divInt, currentFam.marryInt)){
								output << "Error 11: Wife remarried during another marriage: " << listFamily[i].familyID << endl;
                 				cout << "Error 11: Wife remarried during another marriage: " << listFamily[i].familyID << endl;
							}
						}
					}
				}
			}
//        	//Males in the same family must have same last name
//        	if(listPeople[j].sexFlag){
//        		cout <<"found " << listPeople[j].peopleName <<"; famid: " << listFamily[i].familyID << endl;
//        		vector<string> name;
//        		istringstream iss(listPeople[j].peopleName);
//        		string temp;
//        		while(iss >> temp) name.push_back(temp);
//        		string lastName = name.back();
//        		cout << "found male: " << name.front()<< lastName << " with key " << listFamily[i].familyID << endl;
//        		if (familyNames.count(listFamily[i].familyID) == 0){
//        			cout << "added name to samily names: " << name.front()<< lastName << " with key " << listFamily[i].IDNumber << endl;
//					familyNames[listFamily[i].familyID] = lastName;
//				} else {
//					if(familyNames.find(listFamily[i].familyID)->second != lastName) {
//						cout << "last name: " << lastName << "; family name: " << familyNames.find(listFamily[i].IDNumber)->second << endl;
//						output << "Error: Male last name different from family: " << listFamily[i].familyID << endl;
//                 		cout << "Error: Male last name different from family: " << listFamily[i].familyID << endl;
//					}
//				}
//			}
        	
        	//Must be born before dying
        	if(listPeople[j].deathInt.year != 0) {
        		if(listPeople[j].birthInt.year > listPeople[j].deathInt.year || (listPeople[j].birthInt.year == listPeople[j].deathInt.year && listPeople[j].birthInt.month > listPeople[j].deathInt.month) || (listPeople[j].birthInt.year == listPeople[j].deathInt.year && listPeople[j].birthInt.month == listPeople[j].deathInt.month && listPeople[j].birthInt.day > listPeople[j].deathInt.day)) {
					cout << "Error 03: Birth is after death: " << listPeople[j].uniqueID << endl;
					output << "Error 03: Birth is after death: " << listPeople[j].uniqueID << endl;
				}
			}
        	
        	// Must be born at least 14 years before marriage
        	if(listFamily[i].husbandoID == listPeople[j].uniqueID) {
               if(listFamily[i].marryInt.year != 0) {
                  if(listPeople[j].birthInt.year > listFamily[i].marryInt.year || (listFamily[i].marryInt.year == listPeople[j].birthInt.year && listPeople[j].birthInt.month > listFamily[i].marryInt.month) || (listFamily[i].marryInt.year == listPeople[j].birthInt.year && listFamily[i].marryInt.month == listPeople[j].birthInt.month && listPeople[j].birthInt.day > listFamily[i].marryInt.day)) {
                      output << "Error 02: Birth is after Husband's marriage in family: " << listFamily[i].familyID << endl;
                      cout << "Error 02: Birth is after Husband's marriage in family: " << listFamily[i].familyID << endl;
                  }
                  if(listFamily[i].marryInt.month < listPeople[j].birthInt.month) {
                  	  if (listFamily[i].marryInt.year - listPeople[j].birthInt.year - 1 < 14) {
                  	  	output << "Error 10: Marriage is before Husband's 14th birthday: " << listFamily[i].familyID << endl;
                      	cout << "Error 10: Marriage is before Husband's 14th birthday: " << listFamily[i].familyID << endl;
					}
				  } else {
				  	if (listFamily[i].marryInt.year - listPeople[j].birthInt.year < 14) {
                  	  	output << "Error 10: Marriage is before Husband's 14th birthday: " << listFamily[i].familyID << endl;
                      	cout << "Error 10: Marriage is before Husband's 14th birthday: " << listFamily[i].familyID << endl;
					}
				  }
               }
            }
            if(listFamily[i].waifuID == listPeople[j].uniqueID) {
               if(listFamily[i].marryInt.year != 0) {
                  if(listPeople[j].birthInt.year > listFamily[i].marryInt.year || (listFamily[i].marryInt.year == listPeople[j].birthInt.year && listPeople[j].birthInt.month > listFamily[i].marryInt.month) || (listFamily[i].marryInt.year == listPeople[j].birthInt.year && listFamily[i].marryInt.month == listPeople[j].birthInt.month && listPeople[j].birthInt.day > listFamily[i].marryInt.day)) {
                      output << "Error 02: Birth is after Wife's marriage in family: " << listFamily[i].familyID << endl;
                      cout << "Error 02: Birth is after Wife's marriage in family: " << listFamily[i].familyID << endl;
                  }
                  if(listFamily[i].marryInt.month < listPeople[j].birthInt.month) {
                  	  if (listFamily[i].marryInt.year - listPeople[j].birthInt.year - 1 < 14) {
                  	  	output << "Error 10: Marriage is before Wife's 14th birthday: " << listFamily[i].familyID << endl;
                      	cout << "Error 10: Marriage is before Wife's 14th birthday: " << listFamily[i].familyID << endl;
					}
				  } else {
				  	if (listFamily[i].marryInt.year - listPeople[j].birthInt.year < 14) {
                  	  	output << "Error 10: Marriage is before Wife's 14th birthday: " << listFamily[i].familyID << endl;
                      	cout << "Error 10: Marriage is before Wife's 14th birthday: " << listFamily[i].familyID << endl;
					}
				  }
               }
            }
    	    
        	// Must marry before death
        	if(listFamily[i].husbandoID == listPeople[j].uniqueID) {
               if(listPeople[j].deathInt.year != 0) {
                  if(listFamily[i].marryInt.year > listPeople[j].deathInt.year || (listFamily[i].marryInt.year == listPeople[j].deathInt.year && listFamily[i].marryInt.month > listPeople[j].deathInt.month) || (listFamily[i].marryInt.year == listPeople[j].deathInt.year && listFamily[i].marryInt.month == listPeople[j].deathInt.month && listFamily[i].marryInt.day > listPeople[j].deathInt.day)) {
                      output << "Error 05: Marriage is after Husband's death in family: " << listFamily[i].familyID << endl;
                      cout << "Error 05: Marriage is after Husband's death in family: " << listFamily[i].familyID << endl;
                  }
               }
            }
            if(listFamily[i].waifuID == listPeople[j].uniqueID) {
               if(listPeople[j].deathInt.year != 0) {
                  if(listFamily[i].marryInt.year > listPeople[j].deathInt.year || (listFamily[i].marryInt.year == listPeople[j].deathInt.year && listFamily[i].marryInt.month > listPeople[j].deathInt.month) || (listFamily[i].marryInt.year == listPeople[j].deathInt.year && listFamily[i].marryInt.month == listPeople[j].deathInt.month && listFamily[i].marryInt.day > listPeople[j].deathInt.day)) {
                      output << "Error 05: Marriage is after Wife's death in family: " << listFamily[i].familyID << endl;
                      cout << "Error 05: Marriage is after Wife's death in family: " << listFamily[i].familyID << endl; 
                  }
               }
            }
            
            // Must divorce before die
            if(listFamily[i].husbandoID == listPeople[j].uniqueID) {
                if(listPeople[j].deathInt.year != 0) { 
                    if(listFamily[i].divInt.year > listPeople[j].deathInt.year
                            || (listFamily[i].divInt.year == listPeople[j].deathInt.year
                                && listFamily[i].divInt.month > listPeople[j].deathInt.month)
                            || (listFamily[i].divInt.year == listPeople[j].deathInt.year
                                && listFamily[i].divInt.month == listPeople[j].deathInt.month
                                && listFamily[i].divInt.day > listPeople[j].deathInt.day)) {
                        output << "Error 06: Divorce is after Husband's death in family: " << listFamily[i].familyID << endl;
                        cout << "Error 06: Divorce is after Husband's death in family: " << listFamily[i].familyID << endl;
                    }
                }
            }
            
            if(listFamily[i].waifuID == listPeople[j].uniqueID) {
                if(listPeople[j].deathInt.year != 0) {
                    if(listFamily[i].divInt.year > listPeople[j].deathInt.year
                            || (listFamily[i].divInt.year == listPeople[j].deathInt.year
                                && listFamily[i].divInt.month > listPeople[j].deathInt.month)
                            || (listFamily[i].divInt.year == listPeople[j].deathInt.year
                                && listFamily[i].divInt.month == listPeople[j].deathInt.month
                                && listFamily[i].divInt.day > listPeople[j].deathInt.day)) {
                        output << "Error 06: Divorce is after Wife's death in family: " << listFamily[i].familyID << endl;
                        cout << "Error 06: Divorce is after Wife's death in family: " << listFamily[i].familyID << endl;
                    }
                }
            }
            
            //Husband must be Male
        	//Wife must be Female
            if(listFamily[i].husbandoID == listPeople[j].uniqueID) {
                //cout << listPeople[j].peopleName << endl;
                if(listPeople[j].sexFlag != true) {
                    output << "Error 21: Husband is not a Male in family: " << listFamily[i].familyID << endl;
                    cout << "Error 21: Husband is not a Male in family: " << listFamily[i].familyID << endl;
                }
            }
            
            if(listFamily[i].waifuID == listPeople[j].uniqueID) {
                //cout << listPeople[j].peopleName << endl;
                if(listPeople[j].sexFlag != false) {
                    output << "Error 21: Wife is not a Female in family: " << listFamily[i].familyID << endl;
                    cout << "Error 21: Wife is not a Female in family: " << listFamily[i].familyID << endl;
                }
            }
        }
    }
    
    gedcomFile.close();
    output.close(); 

    cin.ignore();
    return 0;
}
