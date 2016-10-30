#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Dates.h"
#include "Child.cpp"

using namespace std;

class Family {
      public:
         int IDNumber;
         string familyID;
         string husbando;
         string husbandoID;
         string waifu;
         string waifuID;
         
         //Yeah we can't have more than 5000 people so this is overkill.
         Child* listChild; // = new Child[5000]; //Name, ID.
         int numOfChild;// = 0;
         
         string marryDate;
         string divDate;
         bool marryFlag;
         bool divFlag;
         
         num_date marryInt;
         num_date divInt;
         
         //Room for expansion
};
