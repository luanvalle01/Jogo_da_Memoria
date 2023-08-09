#include "suport.h"

/******************/
/*JOGO DA MEMORIA */
/* (tradicional)  */
/******************/

void cards_define(card cards[4][4],card card_list[8]){

  /*Preenche uma matriz de cards 4x4*
  /*De maneira pseudo-aleatória     */

  //Quantidade de cartas disponíveis por ID
  int card_free[8] = {2,2,2,2,2,2,2,2};
  int luck_ID = 0, luck_ID2 = 0;
  int loading = 0;

  for(int i=0;i<4;i++){
    for(int j=0;j<2;j++){

      //Seleciona uma carta da lista
      random_range(7,card_free,&luck_ID,&luck_ID2);

      while(1){
         //Verifica se ainda tem cartas disponíveis
        if(card_free[luck_ID] >= 0 && card_free[luck_ID2] >= 0){
          //Adiciona as cartas na mesa
          cards[i][j].id = card_list[luck_ID].id;
          cards[i][j].turn = card_list[luck_ID].turn;
          strcpy(cards[i][j].text,card_list[luck_ID].text);

          cards[i][j+2].id = card_list[luck_ID2].id;
          cards[i][j+2].turn = card_list[luck_ID2].turn;
          strcpy(cards[i][j+2].text,card_list[luck_ID2].text);

          loading+= 2;
          break;
        }
        else{
          //Seleciona uma carta da lista
          random_range(7,card_free,&luck_ID,&luck_ID2);
        }
      }
      clear();
      load_bar(loading,16);
    }
   
  }
}

void cards_draw(card cards[4][4]){
  
  int line = 5*4;

  //Desenha o número da coluna com espaçamento
  for(int i=0;i<4;i++){
    printf("  %i    ",i);
  }
  printf("\n");

  for(int i=0;i<line;i++){

    /*Define a linha de referência*
     *para a matriz de cartas     *
     *com base na linha que está  *
     *sendo impressa              */
    int card_y = 0;
    int print_name = 0;

    switch(i){
      case 2:
        card_y = 0;
        print_name = 1;
        break;
      case 7:
        card_y = 1;
        print_name = 1;
        break;
      case 12:
        card_y = 2;
        print_name = 1;
        break;
      case 17:
        card_y = 3;
        print_name = 1;
        break;
      default:
        print_name = 0;
        break;
    }

    //Desenha o número da linha
    if(print_name){
      printf("%i",card_y);
    }
    else{
       printf(" ");
    }

    //Desenha todas as cartas linha a linha
    for(int j=0;j<4;j++){
      wait_time(15);

      if(print_name){
        if(cards[card_y][j].turn == 1){
          //Desenha a carta na posição específica
          printf("|%s| ",cards[card_y][j].text);
        }
        else{
          printf("|XXXX| ");
        }
      }

      else if(i%5 == 0){
        printf(" ____  ");
      }
      else if(!(i%4 == 0) || !(i-5%3 == 0)){
        printf("|    | ");
      }
      else{
        printf("       ");
      }

    }
    printf("\n");
  }
  printf("\n");
}

int verify_choice(int now_playing,card cards_desk[4][4]){
  int Y1,X1,Y2,X2;
  scanf("%i,%i %i,%i",&Y1,&X1,&Y2,&X2);
  clear();

  /*Verifica se ambas estão viradas     *
   *caso contrário o jogador perde a vez*/
  if((cards_desk[Y1][X1].turn) || (cards_desk[Y2][X2].turn)){
      printf(" Jogada invalida! Perdeu a vez...\n");
      return 0;
  }
  else{
    cards_desk[Y1][X1].turn = 1;
    cards_desk[Y2][X2].turn = 1;
  }

  cards_draw(cards_desk);

  //Verifica o par de Id's
  if(cards_desk[Y1][X1].id == cards_desk[Y2][X2].id){
    if(now_playing == 0){
      strcpy(cards_desk[Y1][X1].text," J1 ");
      strcpy(cards_desk[Y2][X2].text," J1 ");
    }
    else{
      strcpy(cards_desk[Y1][X1].text," J2 ");
      strcpy(cards_desk[Y2][X2].text," J2 ");
    }

    return 1;
  }
  else{
    //Desvira as cartas
    cards_desk[Y1][X1].turn = 0;
    cards_desk[Y2][X2].turn = 0;
    return 0;
  }
}

int game(){
  /*Funcão principal. Encapsulada *
   * para facilitar reestart      */

  int J1 = 0, J2 = 0;
  int now_playing = 0;
  int pair_performed = 0;

  card cards_desk[4][4];
  card card_list[8];

  cards_setup(card_list);
  cards_define(cards_desk,card_list);

  //Caso o carregamento falhe
  if(REESTART){
    printf("Carregamento falhou! reiniciando programa...");
    return 1;
  }

  //ENQUANTO HOUVEREM PARES NÃO REVELADOS
  while(J1 + J2 < 8){
    wait_time(4000);
    clear();

    cards_draw(cards_desk);

    fflush(stdin);
    if(now_playing == 0){
      printf("J1 selecione as cartas y1,x1 y2,x2\n ");
      pair_performed = verify_choice(now_playing, cards_desk);
      if(pair_performed){
        J1++;
      }
      else{
        now_playing = 1;
      }
    }
    else{
      printf("J2 selecione as cartas y1,x1 y2,x2\n ");
      pair_performed = verify_choice(now_playing, cards_desk);
      if(pair_performed){
        J2++;
      }
      else{
        now_playing = 0;
      }
    }
  }

  clear();

  //MENSAGEM FINAL
  if(J1 > J2){
    printf("  >>JOGADOR 1<<\n");
    printf("    >VENCEU<");
  }
  else if(J2 > J1){
    printf("  >>JOGADOR 2<<\n");
    printf("    >VENCEU<");
  }
  else{
    printf("  >>EMPATOU<<\n");
    printf(" >DOIS CRÂNIOS!<");
  }

  wait_time(4000);

  return 0;
}