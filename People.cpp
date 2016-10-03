#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Dates.h"

using namespace std;

class People {
      private:
         bool birthFlag;
         bool deathFlag;

      public:
         int IDNumber;
         string uniqueID;
         string peopleName;
         string birthDate;
         string deathDate;

         num_date birthInt;
         num_date deathInt;

         //Room for expansion
         //getter/setter methods for private flags
         bool getBirthFlag();
         bool getDeathFlag();
         void setBirthFlag(bool b);
         void setDeathFlag(bool b);
};

bool People::getBirthFlag() {
  return birthFlag;
}

bool People::getDeathFlag() {
  return deathFlag;
}

void People::setBirthFlag(bool b) {
  birthFlag = b;
}

void People::setDeathFlag(bool b) {
  deathFlag = b;
}
