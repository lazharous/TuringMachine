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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MdT.h"

// costruttore della classe, inizializza alcune
// variabili che sono controllate (fondamentale)
MdT::MdT( void )
{
  prog = NULL;
  n_data = 0;
  len_tape = 0;
  n_instruction = 0;
  tape_pos = 0;
  tape = NULL;
}


// distruttore della classe
MdT::~MdT(void)
{
  if (code)
    free( code );
    
  if (tape)
    free( tape );
}

// Stampa il valore contenuto
// in quella posizione del nastro
void MdT::printTape(void)
{
    // se ho tutti e 3 i valori
    if ((tape_pos-1 >= 0) && (tape_pos+1 <= len_tape))
    {
      printf(" ___ ___ ___\n");
      printf("| %c | %c | %c | : %d, %d, %d\n", tape[tape_pos-1],
                                                tape[tape_pos],
                                                tape[tape_pos+1],
                                                tape_pos-1,
                                                tape_pos,
                                                tape_pos+1);
      printf(" ___ ___ ___\n");
      return;
    }
    else if (tape_pos-1 >= 0)
    {
      printf(" ___ ___\n");
      printf("| %c | %c | : %d, %d\n", tape[tape_pos-1],
                                       tape[tape_pos],
                                       tape_pos-1,
                                       tape_pos);
      printf(" ___ ___\n");
      return;
    }
    else if (tape_pos+1 <= len_tape)
    {
      printf(" ___ ___\n");
      printf("| %c | %c | : %d, %d\n", tape[tape_pos],
                                       tape[tape_pos+1],
                                       tape_pos,
                                       tape_pos+1);
      printf(" ___ ___\n");
      return;
    }
}


// carica un programma in memoria
void MdT::loadProgram( void )
{
  int i;
  char fname[255] = "";
  printf("inserire il nome del file da aprire: ");
  scanf("%s", &fname);
  prog = fopen(fname, "rt");
  
  if (!prog) {
    printf("Errore nell'apertura.\n");
    return;
  }

  printf("Programma aperto e pronto per essere eseguito.\n");
  
  // carica la lunghezza del nastro
  fscanf(prog, "tape_of: %d\n\n", &len_tape);
  // crea il nastro in memoria
  tape = (char *)malloc(sizeof(char) * len_tape);
  // setta il nastro al valore vuoto
  memset( tape, '/', len_tape );
  
  
  // prende la grandezza dei dati da immettere nel nastro
  fscanf(prog, "data: %d\n\n", &n_data);
  // alloca memoria per la stringa
  data = (char *)malloc(sizeof(char) * n_data);
  // carica il dato da mettere nel nastro
  fscanf(prog, "%s\n", data);
  printf("Dati sul nastro: %s\n", data);
  // carica il dato sul nastro
  for ( i=0; i<strlen(data); i++)
    tape[i] = data[i];
    
  // legge il numero di istruzioni del programma
  fscanf(prog, "n: %d\n", &n_instruction);
  // alloca la grandezza per il codice da eseguire
  if (code) free( code );
  code = (mdt_state *)malloc(sizeof(mdt_state) * n_instruction);
  
  // carica il numero di istruzioni letto prima
  for ( i=0; i<n_instruction; i++)
  {
    fscanf(prog, "%d,%c,%d,%c,%c\n", &code[i].currentState,
                                     &code[i].currentChar,
                                     &code[i].newState,
                                     &code[i].newChar,
                                     &code[i].direction);
  }
  
  printf("Caricate %d istruzioni\n", n_instruction);
  
  printTape();
}


// esegue, linea dopo linea, il programma
void MdT::runCode( int step )
{
  long index = 0;
  
  if (!prog) {
    printf("Caricare prima un programma.\n");
    return;
  }
  
  // usata nel caso la funzione Step-by-Step sia attiva
  char *wait;
  
  // trova il primo stato
  index = setState( 1, tape[0] );

  // esce dall'esecuzione se trova il segnale di HALT
  while (index != HALT && index != ERR)
  {
    // controlla che quello che c'e' su nastro e' uguale
    // al carattere che avevevamo previsto o se c'e' un else
    if ((tape[tape_pos] == code[index].currentChar) || (code[index].currentChar == '?'))
    {
                       
      printf("Tupla: %d,%c,%d,%c,%c\n", code[index].currentState,
                                        code[index].currentChar,
                                        code[index].newState,
                                        code[index].newChar,
                                        code[index].direction);
                                     
      printf("Stato: %d\n", code[index].currentState);
      
	  if (code[index].newChar != '#')
        // scrive il carattere per quella posizione
        tape[tape_pos] = code[index].newChar;

      // controlla la direzione e sposta il nastro
      if (code[index].direction == '>')
        tape_pos++;
      else if (code[index].direction == '<')
        tape_pos--;
        
      // controlla se esiste uno stato che ha il carattere
      // attuale identico a quello che c'e' nel nastro
      // nella posizione appena modificata
      index = setState( code[index].newState, tape[tape_pos] );
      
      // mostra lo stato del nastro
      printTape();
      
      if (step)
        scanf("%s", &wait);
         
    } else
      printf("Errore: carattere inatteso.\n");
  }
  
  tape_pos = 0;
  
  if (index == HALT)
    printf("Programma terminato, stato della macchina: HALT\n");
  else
    printf("Programma terminato, stato della macchina: ERR\n");
  
  printf("Premere un tasto per uscire.\n");
  scanf("%s", &wait);
  
  exit(0);
}


// modifica lo stato all'interno della macchina
// ma solo se esiste un nuovo stato che abbia
// determinate caratteristiche:
// currentState = x
// currentChar = carattere nel nastro alla posizione attuale ( dopo lo spostamento )
long MdT::setState( int state, char c )
{
  int i;
  
  // se siamo alla fine esci direttamente, senza controllare tutto
  // il codice alla ricerca dello stato di uscita
  if (state == HALT)
    return HALT;
    
  // controlla che esista lo stato con il carattere richiesto
  for (i=0; i<n_instruction; i++)
    if ((code[i].currentState == state) && (code[i].currentChar == c))
      return i;
  // se non lo trova, controlla se sia un carattere jolly
  for (i=0; i<n_instruction; i++)
    if ((code[i].currentState == state) && (code[i].currentChar == '?'))
      return i;
      
  // lo stato cercato non esiste, blocchiamo la macchina
  // c'e' stato un errore a livello di programma
  return ERR;
}
