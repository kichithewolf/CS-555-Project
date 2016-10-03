#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Dates.h"

using namespace std;

class Family {
      private:
         bool marryFlag;
         bool divFlag;

      public:
         int IDNumber;
         string familyID;
         string husbando;
         string husbandoID;
         string waifu;
         string waifuID;
         string marryDate;
         string divDate;

         num_date marryInt;
         num_date divInt;
         
         //Room for expansion
         //getter/setter methods for private flags
          bool getMarryFlag();
          bool getDivFlag();
          void setMarryFlag(bool b);
          void setDivFlag(bool b);
};

bool Family::getMarryFlag() {
  return marryFlag;
}

bool Family::getDivFlag() {
  return divFlag;
}

void Family::setMarryFlag(bool b) {
  marryFlag = b;
}

void Family::setDivFlag(bool b) {
  divFlag = b;
}

