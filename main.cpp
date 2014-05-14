/*  
    Macchina di turing ad 1 nastro e 5 istruzioni
    Copyright (C) 2009 Mainieri Paolo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include "MdT.h"

#define LOAD_PROGRAM 1
#define RUN_STEP     2
#define RUN_CODE     3
#define ESCI         4

const char *VERSION = "2.0";

int main(void)
{
  // inizializza una nuova istanza
  // della macchina di turing
  MdT *macchina = new MdT();
  
  printf(" --------------------------\n"
         "| Program: MdT %s         |\n"
         "| Author: Mainieri Paolo   |\n"
         "| Last Update: 11.03.2010  |\n"
         " --------------------------\n\n", VERSION);
  
  int scelta;
  do
  {
    printf("\n1 = Carica programma\n"
             "2 = Step-by-Step\n"
             "3 = Esegui\n"
             "4 = Esci\n\n"
             "Inserire l'operazione da svolgere: ");

    scanf("%d", &scelta);

    switch (scelta)
    {      
      case LOAD_PROGRAM:
        macchina->loadProgram();
      break;
      
      case RUN_STEP:  
        macchina->runCode(1);
      break;
      
      case RUN_CODE:
        macchina->runCode(0);
      break;
      
      case ESCI:
        exit(0);
      break;
      
      default:
        macchina->loadProgram();
    }
    
  } while (scelta != ESCI);

  return 0;
}
