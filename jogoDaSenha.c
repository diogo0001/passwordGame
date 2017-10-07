/* -----------------------------------------------------------------------------
 * Project:     Jogo da Senha
 * File:        jogoDaSenha.c
 * Author:      Diogo Tavares
 * Created:		
 * Modified:	24/09/2017
 * Version:     1.0
 * Purpose:     
 * Clock: 16 Mhz
 * -------------------------------------------------------------------------- */


// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#include "lcd4f.h"
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

struct times{
	uint8 n;
	uint8 x;
	uint8 y;
};

typedef struct times times_t;
// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

void rand_generate(uint8 *vector, volatile uint8 level);
uint8 compare(times_t score, uint8 *rand_password, uint8 *user_password);
uint8 valid(uint8 *vector);

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

attachLcd(display);

volatile uint8 level;
volatile uint8 selectCase;		
volatile uint8 debugFlag;
volatile uint8 aux;
volatile uint8 counter;
volatile uint8 last_PIND = 0xff;

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------
int main()
{
	DDRB = 0b00001111;
	DDRC = 0b00001101;
	DDRD = 0b00000000;
	PORTD = 0b00000111;
	PORTC = 0b00000000;
	PORTB = 0b00000000;
	TCCR0B = 0b00000001;  
	PCICR = 0b00000100;   
	PCMSK2 = 0b00000111;

	_delay_ms(2000);
	
	sei();

	times_t score;
	score.n = 0;
	score.x = 0;
	score.y = 0;

	uint8 user_password[4];		//Buffer de entrada
    uint8 rand_password[4];

    debugFlag = 0;
	selectCase = 0;
	level = 4;
	aux = 0;

	rand_password[0] = 1;
	rand_password[1] = 1;
	rand_password[2] = 1;
	rand_password[3] = 1;
	
	user_password[0] = 1;
	user_password[1] = 1;
	user_password[2] = 1;
	user_password[3] = 1;
	
	clrBit(PORTB, PC1);
	lcdSetDataPort(&display, &DDRB, &PORTB,&PINB, PB0);
	lcdSetControlPort(&display, &DDRC, &PORTC,PC2,PC0,PC1);
	lcdInit(&display, LCD_16X2, LCD_FONT_5X8);
	lcdStdio(&display);
	printf(" PASSWORD GAME!\n");
	printf(" Diogo Tavares");
	_delay_ms(1000);
	
	while(1){	
		
		switch(selectCase){			// Máquina de estados

			case 0:					
				_delay_ms(1000);							// estado inicial
				lcdClearScreen(&display);
				selectCase = 1;							
				break;

			case 1:						
				lcdCursorGoTo(&display, 1, 1);				// seta o nivel e chama a função que gera a senha aleatoria		
				printf("NIVEL: %d",level);
				if(selectCase == 2)	
					rand_generate(rand_password, level);
				score.n = 1;
				counter = 1;
				break;

			case 2:											// Inserção do digitio 1 pelo usuario
				lcdCursorGoTo(&display, 1, 1);						
				printf("NIVEL: %d",level);				
				lcdCursorGoTo(&display, 2, 1);				
				printf("%d\n",counter);
				user_password[0] = counter;
				if(selectCase == 3)							// Seta o contador para 1 quando ocorrer o select
					counter = 1;
				break;

			case 3:											// Inserção do digitio 2 pelo usuario
				lcdCursorGoTo(&display, 2, 2);				
				printf("%d\n",counter);
				user_password[1] = counter;
				if(selectCase == 4)
					counter = 1;
				break;

			case 4:											// Inserção do digitio 3 pelo usuario
				lcdCursorGoTo(&display, 2, 3);				
				printf("%d\n",counter);
				user_password[2] = counter;
				if(selectCase == 5)
					counter = 1;
				break;

			case 5:											// Inserção do digitio 4 pelo usuario
				lcdCursorGoTo(&display, 2, 4);				
				printf("%d\n",counter);
				user_password[3] = counter;
				if(selectCase == 6)
					counter = 1;
				break;

			case 6:											// testa se a senha é valida 
			    if(valid(user_password))
			    	score.n = compare(score, rand_password,user_password);
				break;

			case 7:											// quado o botão debug for pressionado, mostra a senha gerada
				lcdCursorGoTo(&display, 2, 12);
				printf("%d%d%d%d",rand_password[0],rand_password[1],rand_password[2],rand_password[3]);
				selectCase = 0xff;
				break;

			case 8:											// apaga a senha apenas quando o botão é solto
				lcdCursorGoTo(&display, 2, 12);
				printf("    ");	         
				selectCase = aux;
				break;

			default:
				break;
		}		

	}
	return 0 ;
}


// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------
ISR(PCINT2_vect){
	
	_delay_ms(10);
	uint8 changed_bits;
	changed_bits = PIND^last_PIND;  //seta onde houve mudança de estado
	last_PIND = PIND;

	if(changed_bits & (1 << PIND0))		// seleção
		if(PIND & (1 << PIND0)){
			if(selectCase > 6)
				selectCase = 0;

			selectCase++;
		}
			
	if(changed_bits & (1 << PIND1))		// Incremento
		if(PIND & (1 << PIND1)){
			if(selectCase == 1){		// Habilita somente a contagem do level enquanto estiver nesta função do select
				level++;
				if(level > 9)
					level = 4;
			}
			else if(selectCase != 0){
				if(counter > level -1)
					counter = 1;
				else
					counter ++;
			}
		}
	
	if(changed_bits & (1 << PIND2)){
		if(PIND & (1 << PIND2))			// botão solto (pull up)
			selectCase = 8;
		
		else{
			aux = selectCase;			// botão pressionado
			selectCase = 7;
		}
	}

}

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// REALCE FUNÇÃO RAND GENERATE  ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* -----------------------------------------------------------------------------
 * Function:	rand_generate
 * Purpose:		gera um numero aleatório dentro de uma faixa de 1 -> level
 * Arguments:	vetor senha, level
 * Returns:		void
 * -------------------------------------------------------------------------- */
void rand_generate(uint8* vector, volatile uint8 level)
{
	srand(TCNT0);				// gera uma semente a partir da contagem do timer 
	
	vector[0] = 1 + rand()%(level);
	vector[1] = 1 + rand()%(level);
	while(vector[0] == vector[1])				// verifica se são numeros distintos
		vector[1] = 1 + rand()%(level);

	vector[2] = 1 + rand()%(level);
	while((vector[2] == vector[1]) || (vector[2] == vector[0]))
		vector[2] = 1 + rand()%(level);

	vector[3] = 1 + rand()%(level);
	while((vector[3] == vector[2]) || (vector[3] == vector[1]) || (vector[3] == vector[0]))
		vector[3] = 1 + rand()%(level);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/* -----------------------------------------------------------------------------
 * Function:	compare
 * Purpose:		compara a senha fornecida pelo usuario com a senha gerada
 * Arguments:	struct score e os vetores
 * Returns:		score.n (número de tentativas)
 * -------------------------------------------------------------------------- */
uint8 compare(times_t score, uint8 *rand_password, uint8 *user_password)
{
	uint8 i, j, aux[4][4];
	
	for(i=0;i<4;i++)
		if (user_password[i]==rand_password[i]) 
			score.y++;

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			aux[i][j] = user_password[i] - rand_password[j];   
		
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if (!aux[i][j])					
				score.x++;

	score.x = score.x - score.y;		

		if(score.y == 4){						
			lcdClearScreen(&display);
			lcdCursorHome(&display);
			_delay_ms(100);
			printf("!!! PARABENS !!!!!!! PARABENS !!!!!!");
			setBit(PORTC, PC3);				
			_delay_ms(250);
			clrBit(PORTC, PC3);
			_delay_ms(250);					//BUZZER  2 bips
			setBit(PORTC, PC3);						
			_delay_ms(250);
			clrBit(PORTC, PC3);
			for(int shift=0; shift<17; shift++){
				_delay_ms(100);
				lcdDisplayShift(&display, LEFT);
			}
			_delay_ms(1000);
			selectCase = 0;
			level = 4;
			return score.n = 5;				// reseta o jogo, condição inicial para gerar o rand
		}

		else{
			lcdClearScreen(&display);
			lcdCursorHome(&display);
			_delay_ms(100);
			printf("ERROU!!!! %d->%d.%d\n",score.n,score.x,score.y);
			printf("XXXX");
			setBit(PORTC, PC3);			//BUZZER 
			_delay_ms(200);
			clrBit(PORTC, PC3);
			_delay_ms(1000);
			lcdClearScreen(&display);
			score.n++;
		} 
		selectCase = 2;

		if(score.n == 5){					// o usuário tem 4 tentativas
			lcdClearScreen(&display);
			lcdCursorHome(&display);
			printf("XXX GAME OVER XXXXXX GAME OVER XXXXX");
			setBit(PORTC, PC3);				
			_delay_ms(250);
			clrBit(PORTC, PC3);
			_delay_ms(250);					//BUZZER  2 bips
			setBit(PORTC, PC3);							
			_delay_ms(250);
			clrBit(PORTC, PC3);
			_delay_ms(250);					
			setBit(PORTC, PC3);						
			_delay_ms(250);
			clrBit(PORTC, PC3);	

			for(int shift=0; shift<18; shift++){
				_delay_ms(100);
				lcdDisplayShift(&display, LEFT);
			}
			_delay_ms(1000);
			selectCase = 0;
			level = 4;
		}

	return score.n; 						// Retorna para main para manter a contagem ao longo do jogo
}


/* -----------------------------------------------------------------------------
 * Function:	valid
 * Purpose:		testa se a senha do usuario é valida
 * Arguments:	user_password
 * Returns:		0 ou 1
 * -------------------------------------------------------------------------- */

uint8 valid(uint8 *vector)
{
	uint8 i, j, flag, *v;
	v = vector;
	flag = 0;

	for(i=0;i<4;i++){		// compara cada termo do vetor para não haver repetição
		if(flag==1)
			break;
		for(j=0;j<4;j++)
			if(i!=j){
				if(v[i]==vector[j]){
					flag = 1;
					break;
				}
			}
	}

	if(flag){
		lcdClearScreen(&display);
		lcdCursorHome(&display);
		_delay_ms(100);
		printf("SENHA INVALIDA!\n");
		printf("XXXX");
		setBit(PORTC, PC3);			//BUZZER 
		_delay_ms(1000);
		clrBit(PORTC, PC3);
		_delay_ms(500);
		lcdClearScreen(&display);
		selectCase = 2;
		return 0;
	}

	return 1;
}
