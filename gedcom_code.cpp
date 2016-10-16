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

#define CURRYEAR 2016

using namespace std;

int monthToInteger(string monthString) {
    string allMonthStrings[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
    for(int i = 0; i < 12; i++) {
            if(monthString == allMonthStrings[i]) {
                           return (i+1);
            }
    }
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
    
    //List dead people - please keep this before printing people
    output << "-----------------------------------------------" << endl;
    output << "Hall of the Dead" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "Hall of the Dead" << endl;
    cout << "-----------------------------------------------" << endl;
    
    for(int i = 0; i < numOfPeople; i++) {
            //If the death date is filled in then person is dead.
            if(listPeople[i].deathDate[0] != '\0') {
                output << listPeople[i].peopleName << endl;
                cout << listPeople[i].peopleName << endl;
                //Also calculate age
                listPeople[i].age = listPeople[i].deathInt.year - listPeople[i].birthInt.year;
            } else {
                //Calculate age if not dead
                listPeople[i].age = CURRYEAR - listPeople[i].birthInt.year;
            }
    }
    
    //Print people and Family

    output << "-----------------------------------------------" << endl;
    output << "People" << endl;
    output << "-----------------------------------------------" << endl;
    
    cout << "-----------------------------------------------" << endl;
    cout << "People" << endl;
    cout << "-----------------------------------------------" << endl;
    
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
        if(listFamily[i].divInt.year != 0) {
           if(listFamily[i].marryInt.year > listFamily[i].divInt.year || (listFamily[i].marryInt.year == listFamily[i].divInt.year && listFamily[i].marryInt.month > listFamily[i].divInt.month) || (listFamily[i].marryInt.year == listFamily[i].divInt.year && listFamily[i].marryInt.month == listFamily[i].divInt.month && listFamily[i].marryInt.day > listFamily[i].divInt.day)) {
                 output << "Error: Marriage is after divorce in family: " << listFamily[i].familyID << endl;
                 cout << "Error: Marriage is after divorce in family: " << listFamily[i].familyID << endl;
           }
        }
		
        for(int j = 0; j < numOfPeople; j++) {
        	//Must be born before dying
        	if(listPeople[j].deathInt.year != 0) {
        		if(listPeople[j].birthInt.year > listPeople[j].deathInt.year || (listPeople[j].birthInt.year == listPeople[j].deathInt.year && listPeople[j].birthInt.month > listPeople[j].deathInt.month) || (listPeople[j].birthInt.year == listPeople[j].deathInt.year && listPeople[j].birthInt.month == listPeople[j].deathInt.month && listPeople[j].birthInt.day > listPeople[j].deathInt.day)) {
					output << "Error: Birth is after death: " << listPeople[j].uniqueID << endl;
					output << "Error: Birth is after death: " << listPeople[j].uniqueID << endl;
				}
			}
        	
        	// Must be born before marriage and must be at least 14 years old
        	if(listFamily[i].husbandoID == listPeople[j].uniqueID) {
               if(listFamily[i].marryInt.year != 0) {
                  if(listPeople[j].birthInt.year > listFamily[i].marryInt.year || (listFamily[i].marryInt.year == listPeople[j].birthInt.year && listPeople[j].birthInt.month > listFamily[i].marryInt.month) || (listFamily[i].marryInt.year == listPeople[j].birthInt.year && listFamily[i].marryInt.month == listPeople[j].birthInt.month && listPeople[j].birthInt.day > listFamily[i].marryInt.day)) {
                      output << "Error: Birth is after Husband's marriage in family: " << listFamily[i].familyID << endl;
                      cout << "Error: Birth is after Husband's marriage in family: " << listFamily[i].familyID << endl;
                  }
                  if(listFamily[i].marryInt.month < listPeople[j].birthInt.month) {
                  	  if (listFamily[i].marryInt.year - listPeople[j].birthInt.year - 1 < 14) {
                  	  	output << "Error: Marriage is before Husband's 14th birthday: " << listFamily[i].familyID << endl;
                      	cout << "Error: Marriage is before Husband's 14th birthday: " << listFamily[i].familyID << endl;
					}
				  } else {
				  	if (listFamily[i].marryInt.year - listPeople[j].birthInt.year < 14) {
                  	  	output << "Error: Marriage is before Husband's 14th birthday: " << listFamily[i].familyID << endl;
                      	cout << "Error: Marriage is before Husband's 14th birthday: " << listFamily[i].familyID << endl;
					}
				  }
               }
            }
            if(listFamily[i].waifuID == listPeople[j].uniqueID) {
               if(listFamily[i].marryInt.year != 0) {
                  if(listPeople[j].birthInt.year > listFamily[i].marryInt.year || (listFamily[i].marryInt.year == listPeople[j].birthInt.year && listPeople[j].birthInt.month > listFamily[i].marryInt.month) || (listFamily[i].marryInt.year == listPeople[j].birthInt.year && listFamily[i].marryInt.month == listPeople[j].birthInt.month && listPeople[j].birthInt.day > listFamily[i].marryInt.day)) {
                      output << "Error: Birth is after Wife's marriage in family: " << listFamily[i].familyID << endl;
                      cout << "Error: Birth is after Wife's marriage in family: " << listFamily[i].familyID << endl;
                  }
                  if(listFamily[i].marryInt.month < listPeople[j].birthInt.month) {
                  	  if (listFamily[i].marryInt.year - listPeople[j].birthInt.year - 1 < 14) {
                  	  	output << "Error: Marriage is before Wife's 14th birthday: " << listFamily[i].familyID << endl;
                      	cout << "Error: Marriage is before Wife's 14th birthday: " << listFamily[i].familyID << endl;
					}
				  } else {
				  	if (listFamily[i].marryInt.year - listPeople[j].birthInt.year < 14) {
                  	  	output << "Error: Marriage is before Wife's 14th birthday: " << listFamily[i].familyID << endl;
                      	cout << "Error: Marriage is before Wife's 14th birthday: " << listFamily[i].familyID << endl;
					}
				  }
               }
            }
        	
        	// Must marry before die
        	if(listFamily[i].husbandoID == listPeople[j].uniqueID) {
               if(listPeople[j].deathInt.year != 0) {
                  if(listFamily[i].marryInt.year > listPeople[j].deathInt.year || (listFamily[i].marryInt.year == listPeople[j].deathInt.year && listFamily[i].marryInt.month > listPeople[j].deathInt.month) || (listFamily[i].marryInt.year == listPeople[j].deathInt.year && listFamily[i].marryInt.month == listPeople[j].deathInt.month && listFamily[i].marryInt.day > listPeople[j].deathInt.day)) {
                      output << "Error: Marriage is after Husband's death in family: " << listFamily[i].familyID << endl;
                      cout << "Error: Marriage is after Husband's death in family: " << listFamily[i].familyID << endl;
                  }
               }
            }
            if(listFamily[i].waifuID == listPeople[j].uniqueID) {
               if(listPeople[j].deathInt.year != 0) {
                  if(listFamily[i].marryInt.year > listPeople[j].deathInt.year || (listFamily[i].marryInt.year == listPeople[j].deathInt.year && listFamily[i].marryInt.month > listPeople[j].deathInt.month) || (listFamily[i].marryInt.year == listPeople[j].deathInt.year && listFamily[i].marryInt.month == listPeople[j].deathInt.month && listFamily[i].marryInt.day > listPeople[j].deathInt.day)) {
                      output << "Error: Marriage is after Wife's death in family: " << listFamily[i].familyID << endl;
                      cout << "Error: Marriage is after Wife's death in family: " << listFamily[i].familyID << endl; 
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
                        output << "Error: Divorce is after Husband's death in family: " << listFamily[i].familyID << endl;
                        cout << "Error: Divorce is after Husband's death in family: " << listFamily[i].familyID << endl;
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
                        output << "Error: Divorce is after Wife's death in family: " << listFamily[i].familyID << endl;
                        cout << "Error: Divorce is after Wife's death in family: " << listFamily[i].familyID << endl;
                    }
                }
            }
            
            //Husband must be Male
        	//Wife must be Female
            if(listFamily[i].husbandoID == listPeople[j].uniqueID) {
                //cout << listPeople[j].peopleName << endl;
                if(listPeople[j].sexFlag != true) {
                    output << "Error: Husband is not a Male in family: " << listFamily[i].familyID << endl;
                    cout << "Error: Husband is not a Male in family: " << listFamily[i].familyID << endl;
                }
            }
            
            if(listFamily[i].waifuID == listPeople[j].uniqueID) {
                //cout << listPeople[j].peopleName << endl;
                if(listPeople[j].sexFlag != false) {
                    output << "Error: Wife is not a Female in family: " << listFamily[i].familyID << endl;
                    cout << "Error: Wife is not a Female in family: " << listFamily[i].familyID << endl;
                }
            }
        }
    }
    
    gedcomFile.close();
    output.close(); 

    cin.ignore();
    return 0;
}
