/******************/
/*      GERAL     */
/******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
  #include <Windows.h>
#elif _WIN64
  #include <Windows.h>
#else
  #include <unistd.h>
#endif

int REESTART = 0;

/*       Limpa a tela     */
/* independente do sistema*/
void clear() {
    #ifdef __WIN32
      system("cls");
    #elif __WIN64
      system("cls");
    #else
      system("clear");
    #endif
}

/* Pausa o programa em milisegundos */
/*     independente do sistema      */
void wait_time(int time){
  #ifdef __WIN32
    Sleep(time);
  #elif __WIN64
    Sleep(time);
  #else
    usleep(time * 1000);
  #endif
}

/******************/
/*JOGO DA MEMORIA */
/* (tradicional)  */
/******************/

//CARTA INDIVIDUAL
typedef struct{
  int id;
  int turn;
  char text[6];
}card;

//DEFINIÇÃO DAS CARTAS
void cards_setup(card card_list[8]){
  /*Atribui as características
  *     de cada carta
  */
  card_list[0].id = 0;
  strcpy(card_list[0].text,"mola");

  card_list[1].id = 1;
  strcpy(card_list[1].text,"urso");

  card_list[2].id = 2;
  strcpy(card_list[2].text,"amor");

  card_list[3].id = 3;
  strcpy(card_list[3].text,"pneu");

  card_list[4].id = 4;
  strcpy(card_list[4].text,"isca");

  card_list[5].id = 5;
  strcpy(card_list[5].text,"cano");

  card_list[6].id = 6;
  strcpy(card_list[6].text,"teia");

  card_list[7].id = 7;
  strcpy(card_list[7].text,"cone");

  for(int i =0;i<8;i++){
    card_list[i].turn = 0;
  }
}

int get_seconds(){
  /*Base do tempo para valores
  *      pseudoaleatórios    */
  time_t time_now = time(NULL);
  time(&time_now);
  struct tm setup_time;

  /*Obtém o segundo atual da máquina
  *     0 - 59                    */
  setup_time = *localtime(&time_now);
  return setup_time.tm_sec;
}

//Sorteia dois números em um segundo
int random_range(int max_number, int *numbers_free , int *out1, int *out2){
  //Indica quantos números foram sorteados
  int complete = 0;
  //Obtém o segundo atual
  int seconds = get_seconds();

  //Seleciona dois números de acordo com o segundo atual
  int select_number = (int)(seconds/(max_number+1));
  int select_number2 = (int)(select_number/2);

  /*Determina o prazo para selecionar um
  /*pseudo-aleatório em 1 segundo     */
  int max_seconds = seconds+1;

  if(max_seconds == 60){max_seconds = 0;}

  /*   Enquanto estiver no prazo
  * os números sorteados serão somados  */
  while(seconds != max_seconds){
    //Incremento número 1
    if(select_number +1 <= max_number){
      select_number++;
    }
    else{
      select_number = 0;
    }
    
    //Incremento número 2
    if(select_number2 -1 >= 0){
      select_number2--;
    }
    else{
      select_number2 = max_number;
    }
    seconds = get_seconds();
  }
  
  while(seconds != max_seconds+1){

    /*Caso o número 1 esteja disponível*
    *   o mesmo é retornado            */
    if(numbers_free[select_number] > 0 && complete == 0){
      numbers_free[select_number] --; 
      *out1 = select_number;
      
      complete = 1;
    }
    /* Caso falhe, tentará encontrar *
    * o próximo número disponível    */
    else if(complete == 0){
      if(select_number +1 <= max_number){
        select_number++;
      }
      else{
        select_number = 0;
      }
    }

    /*Caso o número 2 esteja disponível*
    *   o mesmo é retornado            */
    if(numbers_free[select_number2] > 0 && complete == 1){
      numbers_free[select_number2] --;
      *out2 = select_number2;

      complete = 2;
    }
    else if (complete == 1){
      if(select_number2 -1 >= 0){
        select_number2--;
      }
      else{
        select_number2 = max_number;
      }
    }
    if(complete == 2){
      return 0;
    }
    seconds = get_seconds();
  }
  /*Se nada der certo dentro de 2 segundos *
  *       o programa reinicia              */
  REESTART = 1;
  return REESTART;

  //Ps: muito melhor que o algorítimo padrão de random.
  //Pelo menos 10x mais rápido e "aleatório"
}

void load_bar(int actual, int max){
  if(actual < max){
    printf("Carregando ");
    for(int i=0;i<actual;i++){
      printf("o");
    }
    for(int i=0;i<max - actual;i++){
      printf(".");
    }
    printf("\n");
  }
  else{//Animação
    max += 12;
    char text[max];
    strcpy(text,"Carregando ");
    for(int i=0;i<max;i++){
      strcat(text,"o");
    }

    while(1){
      for(int i=0;i<max;i++){
        printf("%c", text[i]);
      }
      wait_time(50);
      clear();
      fflush(stdout);

      max--;

      if(max < 0){
        break;
      }
    }
  }
}