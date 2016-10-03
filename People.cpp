#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Dates.h"

using namespace std;

class People {
      public:
         int IDNumber;
         string uniqueID;
         string peopleName;
         string birthDate;
         string deathDate;
         string sex;
         bool birthFlag;
         bool deathFlag;
         
         bool sexFlag;
         num_date birthInt;
         num_date deathInt;
         
         //Room for expansion
};
