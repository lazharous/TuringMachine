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

#ifndef _MDT_H_
#define _MDT_H_

#define HALT -1
#define ERR -2

typedef struct _state
{
  long currentState;
  char currentChar;
  long newState;
  char newChar;
  int direction;
} mdt_state;


// classe per utilizzare una
// macchina di turing
class MdT
{
  public:
    MdT(void);
    ~MdT(void);
    void printTape(void);
    void loadProgram(void);
    void runCode(int step);
    void moveTape(void);
    long setState(int state, char c);
    
  private:
    long n_instruction;
    FILE *prog;
    long tape_pos;
    long len_tape;
    char *tape;
    char *data;
    long n_data;
    mdt_state *code;
};

#endif // _MDT_H_
