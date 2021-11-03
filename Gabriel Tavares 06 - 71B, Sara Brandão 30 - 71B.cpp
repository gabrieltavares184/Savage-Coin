//------------------------------------------------Informa��es do Projeto----------------------------------------------------//

/*

	>>> Trabalho Fase 4 de FPD
	
	Orientadora: Ariane Scarelli.
	Integrantes: Gabriel Tavares da Silva, n� 06, 71B;
				 Sara Brand�o do Amaral, n� 30, 71B.

	Projeto: Savage Coin - Fliperama (registro de jogadores).
	Requisitos atividade 1: Menu e cadastro [Feitos].
	Requisitos atividade 2: Pesquisa e Informa��es do Sistema [Feitos].
	
*/

//------------------------------------------------Bibliotecas--------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <string.h>
#include <windows.h>
#include <time.h>

//------------------------------------------------Arquivos-----------------------------------------------------------------//

FILE *fp; // Ponteiro para arquivo.

//------------------------------------------------Conio--------------------------------------------------------------------//

// Redimensiona o tamanho da tela para o uso do gotoxy.
text_info vActual = {0, 0, 99, 29, WHITE, WHITE, BW80, 30, 100, 1, 1}; // 30 linhas e 100 colunas.

//-------------------------------------------------------------------------------------------------------------------------//

void gotoxy(int x, int y) // Fun��o: Alterar a posi��o do cursor.
{
   COORD c;
   
   if(x < 1 || x > vActual.screenwidth || y < 1 || y > vActual.screenheight) 
	{
		return;
	}
	
   vActual.curx = x;
   vActual.cury = y;
   
   c.X = vActual.winleft + x - 1;
   c.Y = vActual.wintop + y - 1;
   
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
   
}

//-------------------------------------------------------------------------------------------------------------------------//

void textcolor(int newcolor) // Fun��o: Alterar a cor do texto.
{
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
	(csbi.wAttributes & 0xf0) | newcolor);
	vActual.attribute = (csbi.wAttributes & 0xf0) | newcolor;
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void textbackground(int newcolor) // Fun��o: Mudar a cor do v�deo.
{
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
	(csbi.wAttributes & 0x0f) | (newcolor << 4));
	vActual.attribute = (csbi.wAttributes & 0x0f) | (newcolor << 4);
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void _setcursortype(int cur_t) // Fun��o: Fazer o cursor aparecer ou desaparecer.
{
	
   CONSOLE_CURSOR_INFO cci;

   switch(cur_t) 
   {
   	
      case _NOCURSOR:
         cci.dwSize = 100;
         cci.bVisible = FALSE;
         break;
         
      case _SOLIDCURSOR:
         cci.dwSize = 50;
         cci.bVisible = TRUE;
         break;
         
      case _NORMALCURSOR:
         cci.dwSize = 10;
         cci.bVisible = TRUE;
         break;
         
   }
   
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

//-------------------------------------------------------------------------------------------------------------------------//

void clrscr () // Fun��o: Limpar toda a tela.
{
	DWORD escrito;
	int i;

	for( i = 0; i < vActual.screenheight; i++) 
	{
		
		FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
	 	 vActual.attribute, vActual.screenwidth,
		(COORD) {vActual.winleft, vActual.wintop+i}, &escrito);
		FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ',
		 vActual.screenwidth,
		(COORD) {vActual.winleft, vActual.wintop+i}, &escrito);
		
   }
   
   gotoxy(vActual.curx, vActual.cury);
}

//-------------------------------------------------------------------------------------------------------------------------//

void clreol () // Fun��o: Limpar apenas uma linha.
{
	
   COORD coord;
   DWORD escrito;

   coord.X = vActual.winleft+vActual.curx-1;
   coord.Y = vActual.wintop+vActual.cury-1;
   
   FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 
      vActual.screenwidth - vActual.curx + 1, coord, &escrito);
   FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
      vActual.attribute, vActual.screenwidth - vActual.curx + 1, 
      coord, &escrito);
   gotoxy(vActual.curx, vActual.cury);
}

//-------------------------------------------------------------------------------------------------------------------------//

void clearblock( int xmin, int xmax, int ymin, int ymax) // Fun��o: Limpar um bloco na tela.
{             
                                          
	int aux = xmin;
	
	while(ymin!=ymax)
	{
		
		while( xmin!=xmax)
		{
			
			gotoxy(xmin,ymin); printf(" ");
			xmin++;
			
		}
		
		ymin++;
		xmin = aux;
		
	}
	
}

//-------------------------------------------------------Vari�veis Globais-------------------------------------------------//

struct dados // Estrutura de dados do cadastro.
{
	
	int id; // C�digo - Chave prim�ria.
	char nome[40];
	char email[25];
	char nick[20];
	char status[10]; // Exclu�do ou n�o.
	
	struct nascimento
	{
		
		int dia;
		int mes;
		int ano;
		
	} data;
	
	struct tel
	{
		
		char ddd[3];
		char prefixo[6];
		char sufixo[5];	
		
	} telefone;
	
} usuario, auxiliar;

//-------------------------------------------------------Fun��es----------------------------------------------------------//

void abrir_arquivo( char *modo ) // Fun��o: Verificar erros de abertura e abrir o arquivo no modo selecionado.
{
	
	if( (fp = fopen("dados_dos_jogadores.bin", modo) )== NULL ) // Caso, a abertura for nula retorna erro.
	{ 
	
		printf("Erro na abertura do arquivo!");
		exit(1);
		
	}
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void salvar_usuario() // Fun��o: Salvar os dados do cadastro no arquivo.
{
	gotoxy(15,25); printf("Deseja salvar os dados digitados? (Digite S/N): [ ]");
	gotoxy(64,25); 
	
	char a = getche();
	while( a != 's' && a != 'S' && a != 'n' && a != 'N' ) // Consist�ncia.
	{
		
		gotoxy(64,25);printf(" ");
		gotoxy(64,25); a = getche();
		
	}
	
	if ( a == 'S' || a == 's')
	{
		
		if (fwrite(&usuario, sizeof(usuario), 1, fp) != 1) // Caso, a escrita n�o d� certo retorna erro.
			{
				gotoxy(15,27);printf("Erro na escrita do arquivo");
			}
			
		else // Caso, a escrita ocorra corretamente.
			{
				fflush(fp);
				gotoxy(25,27);printf("Dados salvos com sucesso!"); getch();
			}
		
	}
	
	gotoxy(10,25); clreol();
	gotoxy(10,27); clreol();
		
	setlocale(LC_ALL,"PORTUGUESE"); gotoxy(15,25); printf("Deseja cadastrar outro usu�rio? (Digite S/N): [ ]");
	gotoxy(62,25);  setlocale(LC_ALL,"C");
	
	char aux = getche();
	while( aux != 's' && aux != 'S' && aux != 'n' && aux != 'N' ) // Consist�ncia.
	{
		
		gotoxy(62,25); printf(" ");
		gotoxy(62,25); aux = getche();
		
	}
	
	if ( aux == 'S' || aux == 's')
	{
		Sleep(450); // A fun��o Sleep � respons�vel por causar um tempo de espera at� o pr�ximo comando na execu��o.
		int cadastro();
		cadastro();
	}
	
	else
	{
		Sleep(450);
		int menu();
		menu();
	}
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void mascara_dados() // Fun��o: Apresentar os dados que devem ser preenchidos no cadastro.
{
	
	_setcursortype(_NORMALCURSOR); setlocale(LC_ALL,"Portuguese");
	textbackground(15); clrscr();
		
	textcolor(12); gotoxy(4,3);  printf(">"); 
	textcolor(1); gotoxy(6,3); printf(" C A D A S T R O   D E   U S U � R I O S              (Digite 0 no campo do nome para encerrar)");
	textcolor(0);
	gotoxy(4,6);  printf("ID........:");
	gotoxy(4,8);  printf("Nome......:");
	gotoxy(4,10); printf("Nick......:");
	gotoxy(4,12); printf("E-mail....:");
	gotoxy(4,14); printf("Nascimento:  __/__/____");
	gotoxy(4,16); printf("Telefone..:  (  )      -");
	textcolor(12); gotoxy(60,5); printf("* Todos os campos s�o obrigat�rios *");
		
}

//-------------------------------------------------------------------------------------------------------------------------//

void verificar_chave() // Fun��o: Verificar a chave prim�ria do cadastro.
{
	
	abrir_arquivo("ab+");
	srand(time(NULL));
	int aux_id = 0, erro = 0;

	do
	{
		aux_id = rand()%1000+1;
		rewind(fp);
		erro = 0;
			
		while (fread(&usuario, sizeof(usuario),1, fp) == 1)
		{
			
			if (  usuario.id == aux_id && usuario.status[5] == 'n')
			{
			
				erro = 1;

			}

		}	
			
	} while (erro > 0);
	usuario.id = aux_id;		
	_setcursortype(_NORMALCURSOR);

	gotoxy(17,6); textcolor(1); printf("%d",usuario.id);
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void consistencia_nome() // Fun��o: Verificar os dados digitados no campo de nome.
{
	
	int erro = 0;
	
	do
	{
		
		erro = 0; _setcursortype(_NORMALCURSOR); setlocale(LC_ALL,"Portuguese");
		textcolor(1); gotoxy(17,8); gets(usuario.nome);
		
		if ( strcmp(usuario.nome,"0") == 0 )
		{
		
			int menu();
			menu();
	
		}
		
		if ( strlen(usuario.nome) > 35 || strlen(usuario.nome) < 4 || usuario.nome[0] == ' ')
		{
			
			erro++;
			
		}
		
		if ( erro > 0 )
		{
			
			textcolor(12); _setcursortype(_NOCURSOR);
			gotoxy(30,25); printf("ERRO: Nome inv�lido. Redigite!");
			textcolor(0);
			gotoxy(30,27); printf("M�ximo = 35 letras.  M�nimo = 4 letras.");
			gotoxy(30,29); printf("< Pressione qualquer tecla para continuar >");
			getch(); gotoxy(17,8); clreol();
			
		}
		gotoxy(30,25); clreol();
		gotoxy(30,27); clreol();
		gotoxy(30,29); clreol();
		
		
	} while ( erro > 0 );

}

//-------------------------------------------------------------------------------------------------------------------------//

void consistencia_nick() // Fun��o: Verificar os dados digitados no campo de nick (apelido).
{
	
	int erro = 0;
	
	do
	{
		
		erro = 0; _setcursortype(_NORMALCURSOR); setlocale(LC_ALL,"Portuguese");
		textcolor(1); gotoxy(17,10); gets(usuario.nick);
		
		if ( strlen(usuario.nick) > 15 || strlen(usuario.nick) < 4 || usuario.nick[0] == ' ')
		{

			erro++;

		}
		
		if ( erro > 0 )
		{
			
			textcolor(12); _setcursortype(_NOCURSOR);
			gotoxy(30,25); printf("ERRO: Nick inv�lido. Redigite!");
			textcolor(0);
			gotoxy(30,27); printf("M�ximo = 15 caracteres.  M�nimo = 4 caracteres.");
			gotoxy(30,29); printf("< Pressione qualquer tecla para continuar >");
			getch(); gotoxy(17,10); clreol();
			
		}
		gotoxy(30,25); clreol();
		gotoxy(30,27); clreol();
		gotoxy(30,29); clreol();
		
		
	} while ( erro > 0 );

}

//-------------------------------------------------------------------------------------------------------------------------//

void consistencia_email() // Fun��o: Verificar os dados digitados no campo de Email.
{
	
	int auxiliar1, auxiliar2;

	do 
	{
		
	   	_setcursortype(_NORMALCURSOR);
		textcolor(1);	
	   	auxiliar1 = 0; auxiliar2 = 0;
		gotoxy(17,12); gets(usuario.email);
				
		for (int x = 0; x < strlen(usuario.email); x++)
		{
					
			if (usuario.email[x] == '@')
			{
				auxiliar1++;
			}
					
			if (usuario.email[x] == '.')
			{
				auxiliar2++;
			}
					
		}
				
		if ( auxiliar1 != 1 || auxiliar2 < 1 || strlen(usuario.email) < 6 || strlen(usuario.email) > 20 || usuario.email[0] == ' ')
		{
			fflush(stdin); textcolor(0); _setcursortype(_NOCURSOR); setlocale(LC_ALL,"Portuguese");
			textcolor(12); gotoxy(30,24); printf("Erro: Email inv�lido. Redigite!");
			textcolor(0); gotoxy(30,26); printf("M�ximo = 20 caracteres.  M�nimo = 6 caracteres.");
			gotoxy(30,28); printf("< Pressione qualquer tecla para continuar >");
			getch(); gotoxy(17,12); clreol();
		}
				
		gotoxy(30,24); clreol();
		gotoxy(30,26); clreol();
		gotoxy(30,28); clreol();
				
	} while(strlen(usuario.email) < 6 || strlen(usuario.email) > 20 || usuario.email[0] == ' ' || auxiliar1 != 1 || auxiliar2 < 1);
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void consistencia_nascimento()
{
	
	int erro = 0; fflush(stdin);
	setlocale(LC_ALL,"C"); textcolor(0);
	gotoxy(60,28); printf("Pressione a tecla \"ENTER\" para transitar");
	gotoxy(60,29); printf("entre os campos de dia, m%cs e ano.", 136);
	
	   	
	do // Consist�ncia do dia.
	{
		
		_setcursortype(_NORMALCURSOR);
		textcolor(1);
		erro = 0;
		gotoxy(17,14); scanf("%d",&usuario.data.dia);
			
		if (  usuario.data.dia > 31  ||  usuario.data.dia < 1 )
		{
			
			erro++;

		}
			
		if ( erro > 0 )
		{
			
			textcolor(12); _setcursortype(_NOCURSOR); setlocale(LC_ALL,"Portuguese");
			gotoxy(30,24); printf("Erro: Dia inv�lido. Redigite!");
			textcolor(0); gotoxy(30,26); printf("< Pressione qualquer tecla para continuar >");
			getch(); gotoxy(17,14); clreol();
			gotoxy(4,14); printf("Nascimento:  __/__/____");
			gotoxy(23,14);

		}
			
		gotoxy(30,24); clreol();
		gotoxy(30,26); clreol();
			
	} while ( erro > 0 );
		
	int erro2 = 0;

	do // Consist�ncia do m�s.
	{
		
		_setcursortype(_NORMALCURSOR); erro2 = 0;
		gotoxy(20,14); scanf("%d",&usuario.data.mes);
		
		if (  usuario.data.mes > 12  ||  usuario.data.mes < 1 )
		{

			erro2++;

		}
		
		if ( usuario.data.dia >= 29 && usuario.data.mes == 2)
		{

			erro2++;

		}
		
		if ( erro2 > 0 )
		{
			
			textcolor(12); _setcursortype(_NOCURSOR); setlocale(LC_ALL,"Portuguese");
			gotoxy(30,24); printf("ERRO: M�s inv�lido. Redigite!");
			textcolor(0); gotoxy(30,26); printf("< Pressione qualquer tecla para continuar >"); setlocale(LC_ALL,"C");
			getch(); gotoxy(20,17); clreol();
			
			gotoxy(4,14); printf("Nascimento:  %d/__/____",usuario.data.dia); textcolor(1);
			gotoxy(17,14);printf("%d",usuario.data.dia);
			gotoxy(23,14);
			
		}
		
		gotoxy(30,24); clreol();
		gotoxy(30,26); clreol();
		
	} while ( erro2 > 0 );
	
	do // Consist�ncia do ano.
	{
		
		int erro3 = 0;
		_setcursortype(_NORMALCURSOR);
		erro3 = 0;
		gotoxy(23,14); scanf("%d",&usuario.data.ano);
		
		if (  usuario.data.ano > 2020  ||  usuario.data.ano < 1920 )
		{

			erro3++;

		}
		
		if ( erro3 > 0 )
		{

			textcolor(12); _setcursortype(_NOCURSOR); setlocale(LC_ALL,"Portuguese");
			gotoxy(30,24); printf("ERRO: Ano inv�lido, redigite!");
			textcolor(0); gotoxy(30,26); printf("< Pressione qualquer tecla para continuar >"); setlocale(LC_ALL,"C");
			getch();
			
			gotoxy(30,24); clreol();
			gotoxy(30,26); clreol();
			gotoxy(4,14); clreol(); textcolor(0);
			gotoxy(4,14); printf("Nascimento:  __/__/____");
			consistencia_nascimento();

			
		}
		
		gotoxy(30,24); clreol();
		gotoxy(30,26); clreol();
		gotoxy(60,28); clreol();
		gotoxy(60,29); clreol();
		
	} while ( erro2 > 0 );
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void consistencia_telefone() // Fun��o: Verificar os n�meros digitados no campo de telefone.
{ 

	_setcursortype(_NORMALCURSOR);
	   	
	// Consist�ncia do DDD.

	textcolor(1);
	   	
	for (int x = 0; x < 2; x++)
	{
	   		
		gotoxy(18 + x,16); usuario.telefone.ddd[x] = getche(); 
		setlocale(LC_ALL,"C");
	   		
	   	if (usuario.telefone.ddd[x] < 46 || usuario.telefone.ddd[x] > 57) 
	   	{

	   		do
	   		{
	   				
	   			setlocale(LC_ALL,"Portuguese");
	   			textcolor(12); gotoxy(30,26); printf("ERRO: A��o inv�lida. Redigite! (use apenas n�meros).");
				textcolor(1 );
	   			setlocale(LC_ALL,"C");
	   			gotoxy(18 + x,16);
	   			usuario.telefone.ddd[x] = getche();

	   				
			} while ( usuario.telefone.ddd[x] < 46 || usuario.telefone.ddd[x] > 57 );

		}
			
	   	gotoxy(30,26); clreol();
				
	}
		 
	// Consist�ncia prefixo.
		 
	for (int x = 0; x < 5; x++)
	{
	   		
		gotoxy(22 + x,16); usuario.telefone.prefixo[x] = getche(); 
		setlocale(LC_ALL,"C");

		if ( usuario.telefone.prefixo[x] < 46 || usuario.telefone.prefixo[x] > 57 )
		{
					
			do
		   	{
		   				
		   		setlocale(LC_ALL,"Portuguese");
		   		textcolor(12); gotoxy(30,26); printf("ERRO: A��o inv�lida. Redigite! (use apenas n�meros).");
				textcolor(1);
		   		setlocale(LC_ALL,"C");
		   		gotoxy(22 + x,16);
		   		usuario.telefone.prefixo[x] = getche();
		   				
			} while ( usuario.telefone.prefixo[x] < 46 || usuario.telefone.prefixo[x] > 57 );
					
		}
				
	   	gotoxy(30,26); clreol();
				
	}
		
	// Consist�ncia sufixo.
		
	for (int x = 0; x < 4; x++)
	{
	   		
		gotoxy(28 + x,16); usuario.telefone.sufixo[x] = getche(); 
		setlocale(LC_ALL,"C");
			
	   	if ( usuario.telefone.sufixo[x] < 46 || usuario.telefone.sufixo[x] > 57)
		{
		   	
	   		do
			{
			
	   			setlocale(LC_ALL,"Portuguese");
	   			textcolor(12); gotoxy(30,26); printf("ERRO: A��o inv�lida. Redigite! (use apenas n�meros).");
				textcolor(1);
	   			setlocale(LC_ALL,"C");
	   			gotoxy(28 + x,16);
	   			usuario.telefone.sufixo[x] = getche();		
			
			} while ( usuario.telefone.sufixo[x] < 46 || usuario.telefone.sufixo[x] > 57 );
				
		}
			
		gotoxy(30,26); clreol();
			
	}		

		
}

//-------------------------------------------------------------------------------------------------------------------------//

void cadastro() // Fun��o: Digitar os dados dos usu�rios.
{
	
	do
	{

		//---------------------------------------------------// M�scara de dados.
	
		mascara_dados(); 
	
		//---------------------------------------------------// Chave prim�ria.
	
		verificar_chave();
		fflush(stdin);
		
		//---------------------------------------------------// Nome.
		
		consistencia_nome();
		fflush(stdin);
		
		//---------------------------------------------------// Nick.
		
		consistencia_nick();
		fflush(stdin);
		
		//---------------------------------------------------// Email.
		
		consistencia_email();
		fflush(stdin);
		
		//---------------------------------------------------// Nascimento.
		
		consistencia_nascimento();
		fflush(stdin);
		
		//---------------------------------------------------// Telefone.
		
		consistencia_telefone();
		fflush(stdin);
				
		//---------------------------------------------------// Salvar usu�rio.
		
		usuario.status[3] = '0';
		
		salvar_usuario();
		
		//---------------------------------------------------//
		
	} while( strcmp(usuario.nome,"0") != 0 );
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void aviso( int x, int y) // Fun��o: Imprimir um aviso, mostrando ao usu�rio como movimentar a seta do menu.
{	
					    // X � a cor da fonte e Y � a cor da linha.
	setlocale(LC_ALL,"C");
	textcolor(x);
	
	for( int i=15; i<21; i++) // Coluna.
	{
			gotoxy(5,i); printf("%c",179);
	}
	
	for( int i=15; i<21; i++) // Coluna. 
	{
		gotoxy(34,i); printf("%c",179);
	} 
	
	// Linhas.
	gotoxy(5,15); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",218,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,191);
	gotoxy(5,21); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",192,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,217);
	
	gotoxy(7,17); printf(" Use as teclas W/A/S/D"); 
	gotoxy(7,18); printf(" ou as setas direcionais");
	gotoxy(7,19); printf(" para se mover pelo menu.");
	
}
	
//-------------------------------------------------------------------------------------------------------------------------//

int mover_seta (int x, int y, int z, int cor, int loc) // Fun��o: Inserir setas nos menus.
{													  // X � a horizontal da seta, cor � a cor da seta.
	_setcursortype(_NOCURSOR); textcolor(cor);       // Y � a vertical de in�cio e z a vertical limite.
												    // loc � pra atualizar a data toda vez que mover a seta no menu.
	setlocale(LC_ALL,"C"); gotoxy(x,y); printf("\xAF");
	char select; int seta = y;
		
	do
	{
			
		if (loc==1)
		{
				
			textcolor(15);
			gotoxy(3,1); system ( "date /t" ); // Data de acesso.
			gotoxy(93,1);system ( "time /t" ); // Hor�rio de acesso.
			
		} setlocale(LC_ALL,"C");
			
		select = getch();
		textcolor(cor);
			
		switch ( select )
		{
				
			case 119: // W
			case 87:  // w
			case 72:
				if ( seta <= y )
				{
						
					gotoxy(x,seta); printf("  ");
					seta = z;
						
				}
				else 
				{
						
					gotoxy(x,seta); printf("  ");
					seta--;
						
				}
				gotoxy(x,seta); printf("\xAF");
				break;
					
			case 115: // S
			case 83:  // s
			case 80:
				if ( seta >= z )
				{
						
					gotoxy(x,seta); printf("  ");
					seta = y;
					
				}
				else
				{
						
					gotoxy(x,seta); printf("  ");
					seta++;
						
				}
				gotoxy(x,seta); printf("\xAF");
				break;
					
			case 75 :// A 
			case 97 :// a
				gotoxy(x,seta); printf("  ");
				gotoxy(x,y); printf("\xAF");
				seta = y;
				break;
					
			case 100:// D 
			case 77: // d
				gotoxy(x,seta); printf("  ");
				gotoxy(x,z); printf("\xAF");
				seta = z;
				break;
					
			case 27:
				exit(0);
					
		}
			
		Sleep(10);
		
	} while( select != 13); // 13 significa "ENTER" na tabela ASCII.
	
	return seta; // Retorna a op��o da linha selecionada no menu.

}
		
//-------------------------------------------------------------------------------------------------------------------------//

void moldura_intro(int x, int y) // Fun��o: Inserir uma moldura na tela, com anima��o.
{
	
	setlocale(LC_ALL,"C"); textbackground(x); clrscr(); textcolor(y);  // Definir a cor da moldura, limpar o fundo e mudar a sua cor.
		
	gotoxy(2,2); printf("%c",201); // Borda.
		
	for (int i = 3; i< 98; i++) //Linha.
	{
		gotoxy(i,2); printf("%c",205);
		Sleep(5);
	}
		
	for( int i = 3; i < 30; i++) // Coluna.
	{
		gotoxy(2,i); printf("%c",186);
		Sleep(5);
	}
		
	gotoxy(2,29); printf("%c",200); // Borda.
		
	for (int i = 3; i < 98; i++) // Linha.
	{
		gotoxy(i,29); printf("%c",205);
		Sleep(5);
	}
		
	gotoxy(98,2); printf("%c",187); // Borda.

	for( int i = 3; i < 29; i++) // Coluna.
	{
		gotoxy(98,i); printf("%c",186);
		Sleep(5);
	} 
		
	gotoxy(98,29); printf("%c",188); // Borda.
		
}

//-------------------------------------------------------------------------------------------------------------------------//

void moldura(int x, int y) // Fun��o: Inserir uma moldura na tela.
{
	
	setlocale(LC_ALL,"C"); textbackground(x); clrscr(); textcolor(y);  // Definir a cor da moldura, limpar o fundo e mudar a sua cor.
		
	for( int i=2; i<30; i++) // Coluna.
	{
		gotoxy(2,i); printf("%c",186);
	}
		
	for( int i=2; i<30; i++) // Coluna.
	{
		gotoxy(98,i); printf("%c",186);
	} 
		
	// Linhas.
	gotoxy(2,2); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
	gotoxy(2,29);printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);

}

//-------------------------------------------------------------------------------------------------------------------------//

void imprimir_inimigo() // Fun��o: Imprimir uma anima��o, de um inimigo do Space Invaders.
{
	
	for(int z = 0; z < 3; z++)
	{
	
		textcolor(10); Sleep(500); clearblock(71,95,5,12);
		
		gotoxy(71,5); printf("    %c%c          %c%c",219,219,219,219);
		gotoxy(71,6); printf("%c%c    %c%c      %c%c    %c%c",219,219,219,219,219,219,219,219);
		gotoxy(71,7); printf("%c%c  %c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
		gotoxy(71,8); printf("%c%c%c%c%c%c  %c%c%c%c%c%c  %c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
		gotoxy(71,9); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
		gotoxy(71,10); printf("  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
		gotoxy(71,11); printf("    %c%c          %c%c",219,219,219,219);
		gotoxy(71,12); printf("  %c%c              %c%c",219,219,219,219);
		
		Sleep(500); clearblock(71,95,5,12);
		
		gotoxy(71,5); printf("    %c%c          %c%c",219,219,219,219);
		gotoxy(71,6); printf("      %c%c      %c%c",219,219,219,219);
		gotoxy(71,7); printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219);
		gotoxy(71,8); printf("  %c%c%c%c  %c%c%c%c%c%c  %c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219);
		gotoxy(71,9); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
		gotoxy(71,10); printf("%c%c  %c%c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
		gotoxy(71,11); printf("%c%c  %c%c          %c%c  %c%c",219,219,219,219,219,219,219,219);
		gotoxy(71,12); printf("      %c%c%c%c  %c%c%c%c      ",219,219,219,219,219,219,219,219);
			
	}
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void imprimir_nave() // Fun��o: Imprimir a nave do Space Invaders (Desenho).
{
	
	textcolor(4);
	gotoxy(11,16); printf("          %c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219);
	gotoxy(11,17); printf("      %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(11,18); printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(11,19); printf("  %c%c%c%c  %c%c  %c%c  %c%c  %c%c  %c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(11,20); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(11,21); printf("    %c%c%c%c%c%c  %c%c%c%c%c%c  %c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(11,22); printf("      %c%c      %c%c      %c%c",219,219,219,219,219,219);
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void imprimir_mario() // Fun��o: Imprimir um desenho do Mario Bros.
{
	
	textcolor(4);
	gotoxy(62,10); printf("%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219);
	gotoxy(60,11); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	
	textcolor(8);
    gotoxy(56,12); printf("    %c%c%c%c%c%c    %c%c",219,219,219,219,219,219,219,219); 
    gotoxy(56,13); printf("  %c%c  %c%c      %c%c",219,219,219,219,219,219);
    gotoxy(56,14); printf("  %c%c  %c%c%c%c      %c%c",219,219,219,219,219,219,219,219);
    gotoxy(56,15); printf("  %c%c%c%c        %c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219);
    
    textcolor(1);
    gotoxy(56,17); printf("    %c%c%c%c  %c%c%c%c",219,219,219,219,219,219,219,219);
    gotoxy(56,18); printf("  %c%c%c%c%c%c  %c%c%c%c  %c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
    gotoxy(56,19); printf("%c%c%c%c%c%c%c%c        %c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
    gotoxy(60,20); printf("%c%c",219,219);
    gotoxy(74,20); printf("%c%c",219,219);
    
    textcolor(4);
    gotoxy(64,17); printf("%c%c",219,219);
	gotoxy(64,18); printf("%c%c    %c%c",219,219,219,219);
	gotoxy(64,19); printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
	gotoxy(62,20); printf("%c%c  %c%c%c%c  %c%c",219,219,219,219,219,219,219,219);
	gotoxy(62,21); printf("%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(60,22); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(60,23); printf("%c%c%c%c%c%c    %c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219);

	textcolor(6);
	gotoxy(56,24); printf("  %c%c%c%c%c%c",219,219,219,219,219,219);
	gotoxy(56,25); printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
	
	gotoxy(72,24); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
	gotoxy(72,25); printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
    
    textcolor(6);
    gotoxy(60,13); printf("%c%c",219,219);
    gotoxy(64,13); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
    gotoxy(72,13); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
    
    gotoxy(60,14); printf("%c%c",219,219);
    gotoxy(66,14); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
    gotoxy(74,14); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
    
    gotoxy(66,12); printf("%c%c%c%c",219,219,219,219);
    gotoxy(72,12); printf("%c%c",219,219);
    gotoxy(62,15); printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
    gotoxy(62,16); printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219);
    
    textcolor(1);
    gotoxy(66,18); printf("%c%c%c%c",219,219,219,219);
    
    textcolor(4);
    gotoxy(70,17); printf("%c%c",219,219);
    
    textcolor(6);
    gotoxy(56,20); printf("%c%c%c%c",219,219,219,219);
    gotoxy(56,21); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
    gotoxy(56,22); printf("%c%c%c%c",219,219,219,219);
    
    gotoxy(76,20); printf("%c%c%c%c",219,219,219,219);
    gotoxy(74,21); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
    gotoxy(76,22); printf("%c%c%c%c",219,219,219,219);
    
    textcolor(14);
    gotoxy(64,20); printf("%c%c",219,219);
    
    gotoxy(70,20); printf("%c%c",219,219);
     
}

//-------------------------------------------------------------------------------------------------------------------------//

void imprimir_kirby() // Fun��o: Imprimir um desenho do Kirby.
{
	
	textcolor(12);
	gotoxy(54,11); printf("%c%c",219,219);
	
	textcolor(13);
	gotoxy(50,11); printf("  %c%c      %c%c%c%c%c%c",219,219,219,219,219,219,219,219);
	gotoxy(50,12); printf("%c%c%c%c  %c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,13); printf("%c%c  %c%c%c%c%c%c  %c%c  %c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,14); printf("    %c%c%c%c%c%c  %c%c  %c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,15); printf("  %c%c%c%c%c%c%c%c  %c%c  %c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,16); printf("%c%c%c%c%c%c    %c%c%c%c%c%c    %c%c%c%c%c%c  ",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,17); printf("  %c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c  %c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,18); printf("  %c%c%c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c  %c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,19); printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,20); printf("    %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,21); printf("      %c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,22); printf("        %c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219);
	
	
	textcolor(12);
	gotoxy(54,11); printf("%c%c",219,219);
	gotoxy(58,11); printf("%c%c",219,219);
	gotoxy(66,11); printf("%c%c",219,219);
	gotoxy(70,12); printf("%c%c",219,219);
	gotoxy(52,13); printf("%c%c",219,219);
	gotoxy(72,13); printf("%c%c",219,219);
	gotoxy(50,14); printf("%c%c",219,219);
	gotoxy(52,14); printf("%c%c",219,219);
	gotoxy(50,15); printf("%c%c",219,219);
	gotoxy(56,16); printf("%c%c%c%c",219,219,219,219);
	gotoxy(66,16); printf("%c%c%c%c",219,219,219,219);
	gotoxy(76,16); printf("%c%c",219,219);
	gotoxy(50,17); printf("%c%c",219,219);
	gotoxy(72,17); printf("%c%c",219,219);
	gotoxy(72,18); printf("%c%c",219,219);
	gotoxy(52,19); printf("%c%c",219,219);
	gotoxy(52,20); printf("%c%c",219,219);
	gotoxy(72,20); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
	gotoxy(54,21); printf("%c%c",219,219);
	gotoxy(70,21); printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
	gotoxy(56,22); printf("%c%c",219,219);
	gotoxy(70,22); printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
	gotoxy(52,23); printf("%c%c%c%c    %c%c%c%c%c%c  %c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
	gotoxy(50,24); printf("%c%c%c%c%c%c%c%c%c%c          %c%c%c%c",219,219,219,219,219,219,219,219,219,219);
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void title_screen() // Fun��o: Tela de t�tulo, respos�vel pela apresenta��o e introdu��o do programa.
{
	
	_setcursortype(_NOCURSOR);
	setlocale(LC_ALL, "Portuguese"); // Ir� alterar a linguagem local nas impress�es para a Linguagem C.
	for (int x = 1; x <= 3; x++) // Faz a tela piscar, entre branco e preto.
	{
		
		clrscr(); textbackground(15); Sleep(445);
		
		
		clrscr(); textbackground(0); Sleep(445);
		
	}
	
	moldura_intro(0,9);
	
	textcolor(9);
	gotoxy(4,5); printf(" %c%c%c%c%c",219,219,219,219,219); // S
	Sleep(100);
	gotoxy(4,6); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(4,7); printf("%c",219);
	Sleep(100);
	gotoxy(4,8); printf(" %c%c%c%c%c",219,219,219,219,219);
	Sleep(100);
	gotoxy(4,9); printf("      %c",219);
	Sleep(100);
	gotoxy(4,10); printf("      %c",219);
	Sleep(100);
	gotoxy(4,11); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(4,12); printf(" %c%c%c%c%c",219,219,219,219,219);
	
	
	
	textcolor(6);
	gotoxy(14,5); printf(" %c%c%c%c%c",219,219,219,219,219,219); // A
	Sleep(100);
	gotoxy(14,6); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(14,7); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(14,8); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(14,9); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(14,10); printf("%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
	Sleep(100);
	gotoxy(14,11); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(14,12); printf("%c     %c",219,219);
	
	
	
	textcolor(4);
	gotoxy(24,5); printf("%c%c      %c%c",219,219,219,219); // V
	Sleep(100);
	gotoxy(24,6); printf("%c%c      %c%c",219,219,219,219);
	Sleep(100);
	gotoxy(24,7); printf("%c%c      %c%c",219,219,219,219);
	Sleep(100);
    gotoxy(24,8); printf("  %c%c  %c%c",219,219,219,219);
    Sleep(100);
    gotoxy(24,9); printf("  %c%c  %c%c",219,219,219,219);
    Sleep(100);
    gotoxy(24,10); printf("  %c%c  %c%c",219,219,219,219);
    Sleep(100);
    gotoxy(24,11); printf("    %c%c",219,219);
    Sleep(100);
    gotoxy(24,12); printf("    %c%c",219,219);
    
    
	
	textcolor(10);
	gotoxy(37,5); printf(" %c%c%c%c%c",219,219,219,219,219,219); // A
	Sleep(100);
	gotoxy(37,6); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(37,7); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(37,8); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(37,9); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(37,10); printf("%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
	Sleep(100);
	gotoxy(37,11); printf("%c     %c",219,219);
	Sleep(100);
	gotoxy(37,12); printf("%c     %c",219,219);
	
	
	
	textcolor(6);
	gotoxy(47,5); printf("  %c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219); // G
	Sleep(100);
	gotoxy(47,6); printf("%c%c",219,219);
	Sleep(100);
	gotoxy(47,7); printf("%c%c",219,219);
	Sleep(100);
	gotoxy(47,8); printf("%c%c  %c%c%c%c",219,219,219,219,219,219);
	Sleep(100);
	gotoxy(47,9); printf("%c%c      %c%c",219,219,219,219);
	Sleep(100);
	gotoxy(47,10); printf("%c%c      %c%c",219,219,219,219);
	Sleep(100);
	gotoxy(47,11); printf("%c%c      %c%c",219,219,219,219);
	Sleep(100);
	gotoxy(47,12); printf("  %c%c%c%c%c%c",219,219,219,219,219,219);
	
	
	
	textcolor(4);
	gotoxy(60,5); printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219); // E
	Sleep(100);
	gotoxy(60,6); printf("%c%c",219,219);
	Sleep(100);
	gotoxy(60,7); printf("%c%c",219,219);
	Sleep(100);
	gotoxy(60,8); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
	Sleep(100);
	gotoxy(60,9); printf("%c%c",219,219);
	Sleep(100);
	gotoxy(60,10); printf("%c%c",219,219);
	Sleep(100);
	gotoxy(60,11); printf("%c%c",219,219);
	Sleep(100);
	gotoxy(60,12); printf("%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219);
	
	imprimir_inimigo();
	
	for(int x = 0; x < 3; x++) // Far� a impress�o de uma nave e ir� apag�-la 3, para causar um efeito na tela de t�tulo.
	{
		
		imprimir_nave();
		Sleep(500);
		clearblock(11,41,16,23);
		Sleep(500);
		imprimir_nave();
		
	} 
	
	Sleep(150);
	textcolor(10);
	gotoxy(44,16); printf(" %c%c%c%c%c",219,219,219,219,219); // C
	gotoxy(44,17); printf("%c",219);
	gotoxy(44,18); printf("%c",219);
	gotoxy(44,19); printf("%c",219);
	gotoxy(44,20); printf("%c",219);
	gotoxy(44,21); printf("%c",219);
	gotoxy(44,22); printf("%c",219);
	gotoxy(44,23); printf(" %c%c%c%c%c",219,219,219,219,219);
	
	
	
	Sleep(150);
	textcolor(6);
	gotoxy(54,16); printf(" %c%c%c%c%c",219,219,219,219,219); // O
	gotoxy(54,17); printf("%c     %c",219,219);
	gotoxy(54,18); printf("%c     %c",219,219);
	gotoxy(54,19); printf("%c     %c",219,219);
	gotoxy(54,20); printf("%c     %c",219,219);
	gotoxy(54,21); printf("%c     %c",219,219);
	gotoxy(54,22); printf("%c     %c",219,219);
	gotoxy(54,23); printf(" %c%c%c%c%c",219,219,219,219,219);
	
	
	
	Sleep(150);
	textcolor(9);
	gotoxy(64,16); printf("%c%c%c%c%c%c",219,219,219,219,219,219); // I
	gotoxy(64,17); printf("  %c%c",219,219);
	gotoxy(64,18); printf("  %c%c",219,219);
	gotoxy(64,19); printf("  %c%c",219,219);
	gotoxy(64,20); printf("  %c%c",219,219);
	gotoxy(64,21); printf("  %c%c",219,219);
	gotoxy(64,22); printf("  %c%c",219,219);
	gotoxy(64,23); printf("%c%c%c%c%c%c",219,219,219,219,219,219);
	
	
	
	Sleep(150);
	textcolor(10);
	gotoxy(74,16); printf("%c%c    %c%c",219,219,219,219); // N
	gotoxy(74,17); printf("%c%c    %c%c",219,219,219,219);
	gotoxy(74,18); printf("%c%c%c   %c%c",219,219,219,219,219);
	gotoxy(74,19); printf("%c%c %c%c %c%c",219,219,219,219,219,219);
	gotoxy(74,20); printf("%c%c   %c%c%c",219,219,219,219,219);
	gotoxy(74,21); printf("%c%c    %c%c",219,219,219,219);
	gotoxy(74,22); printf("%c%c    %c%c",219,219,219,219);
	gotoxy(74,23); printf("%c%c    %c%c",219,219,219,219);
	
	Sleep(900);textcolor(15); 
	gotoxy(32,27); printf("> Pressione \"ENTER\" para continuar <");
	
	int iniciar = getch();
	if(iniciar !=13) // Somente sair� da tela de t�tulo quando o usu�rio pressionar a tecla "ENTER".
	{
		
		do // Consist�ncia da tecla "ENTER".
		{
			
			gotoxy(32,27); printf("> Pressione \"ENTER\" para continuar <");
			iniciar = getch();
			
		} while(iniciar !=13);
		
	}
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void tela_de_carregamento () // Fun��o: Desenhar uma tela de carregamento.
{
		
	clrscr();	
	int time = 400;	
	for (int x=39; x<=61; x++)	
	{
			
		gotoxy(x,13); printf("%c",205);
			
	}
	gotoxy(42,12);printf("...CARREGANDO...");
	for (int x=39; x<=61; x++)	
	{
			
		gotoxy(x,15); printf("%c",205);
			
	}
	for (int i = 0; i<23; i+=2)
	{
		
		Sleep(time);
		textcolor(10);
		gotoxy(39+i,14);printf("%c",219);
		time-=35;
		    
	}
	Sleep(90);
	textcolor(15);
	Sleep(2000);
	system("cls");
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void tabela(int x, int y) // Fun��o: Exibir a tabela de dados.
{
	
	setlocale(LC_ALL,"Portuguese"); 
    textcolor(9);
	gotoxy(x,y);   printf("___________________________________________________________");
	gotoxy(x,y+1); printf("|      NOME      |     NICK     |  ID  |     EMAIL        |");
	gotoxy(x,y+2); printf("|________________|______________|______|__________________|");
	gotoxy(x,y+3); printf("|                |              |      |                  |");
	gotoxy(x,y+4); printf("|________________|______________|______|__________________|");
	gotoxy(x,y+5); printf("|                |              |      |                  |");
	gotoxy(x,y+6); printf("|________________|______________|______|__________________|");
	gotoxy(x,y+7); printf("|                |              |      |                  |");
	gotoxy(x,y+8); printf("|________________|______________|______|__________________|");
	gotoxy(x,y+9); printf("|                |              |      |                  |");
	gotoxy(x,y+10);printf("|________________|______________|______|__________________|");
	gotoxy(x,y+11);printf("|                |              |      |                  |");
	gotoxy(x,y+12);printf("|________________|______________|______|__________________|");
	gotoxy(x,y+13);printf("|                |              |      |                  |");
	gotoxy(x,y+14);printf("|________________|______________|______|__________________|");
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void pesquisa_geral() // Fun��o: Exibir todos os dados dos usu�rios.
{
	
	fclose(fp); 
	clrscr();
	fflush(stdin); abrir_arquivo("rb+"); // Fun��o: Abrir o arquivo em modo de leitura.
	int x = 9; int y = 0; char z;
	_setcursortype(_NORMALCURSOR); 
	textcolor(9); setlocale(LC_ALL,"Portuguese"); _setcursortype(_NORMALCURSOR);
	x = 9; y = 0;
	textbackground(15); clrscr(); 
	setlocale(LC_ALL,"Portuguese"); _setcursortype(_NORMALCURSOR);
	rewind(fp);
	while( !feof(fp) )
	{
		
		if ( fread(&usuario, sizeof(usuario), 1, fp) == 1 ) // Caso, exista um registro no arquivo.
		{
				
			if ( y == 0 )
			{
				
				tabela(10,6); // Ir� exibir a tebela.
				
			}

			textcolor(0);
			gotoxy(12,x); puts(usuario.nome);  
			gotoxy(29,x); puts(usuario.nick);  
			gotoxy(45,x); printf("%d",usuario.id);  
			if ( strlen(usuario.email) > 16 )
			{
				
				gotoxy(52,x); puts(usuario.email);
				gotoxy(64,x); printf("...|          ");
				
			}
			else
			{
				
				gotoxy(52,x); puts(usuario.email);
				
			}
					  
			x += 2; y++; Sleep(50);
			if ( x > 20 ) // Caso, a tabela seja completamente preenchida.
			{
				
				gotoxy(33,27); printf("Pressione a tecla \"ESC\" para voltar ou");
				gotoxy(33,28); printf("qualquer outra tecla para continuar"); 
				setlocale(LC_ALL,"C");
				if ( getch() == 27 ) 
				{
					
					int menu_pesquisa();
					menu_pesquisa();
					
				}
				else
				{
					
					clrscr(); tabela(10,6); // Ir� exibir a tabela.
					x = 9;
					
				}
				
			}
			
		}
		
	}
	if ( y == 0 ) // Caso, n�o seja encontrado nada compat�vel.
	{
		
		setlocale(LC_ALL,"Portuguese");
		_setcursortype(_NOCURSOR);
		textcolor(4); gotoxy(20,15); printf("ERR0: N�o h� resultados para essa busca!");
		textcolor(0); gotoxy(20,20); printf("Cadastro");
		gotoxy(20,21); printf("Voltar..");
		gotoxy(20,22); printf("Menu....");
		
		setlocale(LC_ALL,"C");
		textcolor(4);
		for(int i = 20; i < 23; i++)
		{
			gotoxy(30,i); printf("%c",179);
		}
		
		setlocale(LC_ALL,"Portuguese");
		textcolor(0);
		
		char z = mover_seta(17,20,22,10,0);
		switch (z)
		{
			
			case 20:
			cadastro();
			break;
			
			case 21:
			int menu_pesquisa();
			menu_pesquisa();
			break;
			
			case 22:
			int menu();
			menu();
			break;	
			
		}
		
	}
	else
	{
		
		gotoxy(33,27); printf("                                            ");	
		_setcursortype(_NOCURSOR);
		gotoxy(36,27); printf("Pressione a tecla \"ENTER\" para retornar"); 
		z = getch();
		if( z !=13 ) 
		{
			
			do 
			{
				
				gotoxy(36,28); printf("Pressione a tecla \"ENTER\" para retornar");
				z = getch();
				
			} while( z !=13);
			
		}
		
		clrscr();
		int menu_pesquisa();
		menu_pesquisa();
		
	}
	fclose(fp);
}

//-------------------------------------------------------------------------------------------------------------------------//

void pesquisa_por_id() // Fun��o: Pesquisar um determinado usu�rio pelo ID.
{
	
	fclose(fp); textbackground(0); clrscr(); textcolor(15); setlocale(LC_ALL,"PORTUGUESE");
	_setcursortype( _NORMALCURSOR );
	fflush(stdin); 
	abrir_arquivo("rb+");
	int aux = 0; int user = 0;
	do
	{	
	
		gotoxy(5,4); printf("Digite o c�digo que deseja buscar:			        (Digite 0 para retornar ao menu)");
		gotoxy(41,4); scanf("%d",&aux);
		rewind(fp);
		if (aux == 0)
		{
			
			clrscr();
			int menu_pesquisa();
			menu_pesquisa();
			
		}	
		while( !feof(fp) )
		{
			user = 0;
			if(fread(&usuario, sizeof(usuario), 1, fp) == 1) 
			{				
				if ( usuario.id == aux)
				{	               
					user = 1;
					_setcursortype(_NOCURSOR); 
					textcolor(15); setlocale(LC_ALL,"Portuguese");
				 	mascara_dados();
				 	gotoxy(17,6); printf("%d",usuario.id);
				 	gotoxy(17,8); printf("%s",usuario.nome);
				 	gotoxy(17,10); printf("%s",usuario.nick);
				 	gotoxy(17,12); printf("%s",usuario.email);
					if ( usuario.data.dia >= 1 && usuario.data.dia <= 10 )
					{
						gotoxy(17,14); printf("0%d",usuario.data.dia);
					}
					
					else
					{
						gotoxy(17,14); printf("%d",usuario.data.dia);
					}
					
					if ( usuario.data.mes >= 1 && usuario.data.mes <= 10 )
					{
						gotoxy(19,14); printf("/0%d/%d",usuario.data.mes,usuario.data.ano);
					}
					
					else
					{			
						gotoxy(19,14); printf("/%d/%d",usuario.data.mes,usuario.data.ano);
					}
					gotoxy(17,16); printf("(%s)",usuario.telefone.ddd);
					gotoxy(21,16); puts(usuario.telefone.prefixo);
					gotoxy(26,16); printf("-%s",usuario.telefone.sufixo);
					
					textcolor(0);_setcursortype(_NOCURSOR);
					setlocale(LC_ALL,"Portuguese");
					gotoxy(13,20); printf("Voltar");
					gotoxy(13,21); printf("Buscar outro ID");
					setlocale(LC_ALL,"C");
					char z = mover_seta(10,20,21,10,0);
					switch (z)
					{
						case 20:
						int menu_pesquisa();
						menu_pesquisa();
						break;	

						case 21:
						pesquisa_por_id();
						break;	
					}
				}
			}
		}
		if ( user == 0 )
			{
				clrscr();
				textcolor(4);
				gotoxy(30,12);printf("ERRO: ID n�o localizado!");
				textcolor(15);
				_setcursortype(_NOCURSOR);
				gotoxy(36,14); printf("Voltar");
				gotoxy(36,15); printf("Cadastro");
				gotoxy(36,16); printf("Buscar outro ID");
				char z = mover_seta(34,14,16,10,0);
				switch (z)
				{
					case 14:
						int menu_pesquisa();
						menu_pesquisa();
						break;
					case 15:
						int cadastro();
						cadastro();
						break;
					case 16:
				    	pesquisa_por_id();
						break;
				} 
			}
	} while( aux != 0 );
}


//-------------------------------------------------------------------------------------------------------------------------//

void pesquisa_por_nick() // Fun��o: Pesquisar um determinado usu�rio pelo seu nick (apelido).
{
	
	char aux[12]; fclose(fp);
	fflush(stdin); abrir_arquivo("rb+"); // Fun��o: Abrir o arquivo em modo de leitura.
	_setcursortype(_NORMALCURSOR); 
	int x = 9; int y = 0; char z;
	do
	{
		
		x = 9; y = 0;
	    setlocale(LC_ALL,"Portuguese"); 
		_setcursortype(_NORMALCURSOR);
		textbackground(0); clrscr(); 
	    textcolor(15); gotoxy(5,7); printf("Digite o nick que deseja buscar:                               (Digite 0 para retornar ao menu)"); 
		gotoxy(38,7); gets(aux);	
		
		if (strcmp(aux,"0") == 0) // Retrocede se digitarem 0.
		{
			
			int menu_pesquisa();
			menu_pesquisa();
			
		}
		strupr(aux);
		rewind(fp);
		while( !feof(fp) )
		{
			
			if ( fread(&usuario, sizeof(usuario), 1, fp) == 1) // Caso, exista um registro no arquivo.
			{
				
				strupr(usuario.nick);
				if ( strstr(usuario.nick,aux) != NULL ) // Compara os dados digitados com os dados ja registrados.
				{
								
					if ( y == 0 )
					{
						
						textbackground(15); clrscr();
						tabela(10,6); // Ir� exibir a tabela.
						
					}
					textcolor(0);
					gotoxy(12,x); puts(usuario.nome);  
					gotoxy(29,x); puts(usuario.nick);  
					gotoxy(45,x); printf("%d",usuario.id);  
					if ( strlen(usuario.email) > 16 )
					{
						
						gotoxy(52,x); puts(usuario.email);
						gotoxy(64,x); printf("...|          ");
						
					}
					else
					{
						
						gotoxy(52,x); puts(usuario.email);
						
					}
					  
					x += 2; y++; Sleep(45);
					if ( x > 20 ) // Caso, a tabela seja completamente preenchida.
					{	
					
			 			gotoxy(30,27); printf("Pressione a tecla \"ESC\" para voltar");
						setlocale(LC_ALL,"C");
						if ( getch() == 27 ) 
						{
							
							int menu_pesquisa();
							menu_pesquisa();
							
						}
						
					}
					
				}
				
			}
			
		}
		if ( y == 0 ) // Caso, nada seja encontrado.
		{
			
			setlocale(LC_ALL,"Portuguese");
			_setcursortype(_NOCURSOR);
			textcolor(4); gotoxy(20,15); printf("ERRO: Sem resultados para essa busca!");
			textcolor(15);
			gotoxy(20,20); printf("Cadastro");
			gotoxy(20,21); printf("Buscar outro nick");
			gotoxy(20,22); printf("Menu");
			
			char z = mover_seta(17,20,22,10,0); 
			switch (z)
			{
				
				case 20:
				cadastro();
				break;

				case 21:
				pesquisa_por_nick();
				break;

				case 22:
				int menu_pesquisa();
				menu_pesquisa();
				break;	
				
			}
		}
		else
		{
			
			gotoxy(33,27); printf("                                            ");	
			_setcursortype(_NOCURSOR);
			gotoxy(36,27); printf("Pressione a tecla \"ENTER\" para retornar "); z = getch();
			if( z !=13 ) 
			{
				
				do 
				{
					
					gotoxy(36,28); printf("Pressione a tecla \"ENTER\" para retornar ");
					z = getch();
				} while( z !=13);
				
			}
			clrscr();
			
		}
	} while ( strcmp(aux,"0") != 0);
	fclose(fp);
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void menu_pesquisa() // Fun��o: Menu da aba de pesquisas.
{
	
	moldura(0,10);
	aviso(15,15);
	imprimir_kirby();
	int selecionar;
	do
	{
		
		textcolor(15); setlocale(LC_ALL,"PORTUGUESE");
		gotoxy(9,8); printf("Pesquisa geral....");
		gotoxy(9,9); printf("Pesquisa por ID...");
		gotoxy(9,10); printf("Pesquisa por nick.");
		gotoxy(9,11); printf("Voltar para o menu");

		setlocale(LC_ALL,"C"); textcolor(4);
		
		for(int i = 8; i < 12; i++)
		{
			gotoxy(29,i); printf("%c",179);
		}
		
		selecionar = mover_seta( 6, 8, 11, 10, 0);
		switch(selecionar)
		{
			
			case 8:
			pesquisa_geral();
			break;
			
			
			case 9:
			pesquisa_por_id();
			break;
			
			case 10:
			pesquisa_por_nick();
			break;
			
		}
		
	} while ( selecionar != 11);
	
	clrscr();
	int menu();
	menu();
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void sobre() // Fun��o: Informar o objetivo desse sistema, e quem foram as pessoas renpons�veis pelo seu desenvolvimento.
{
	
	system("cls");
	moldura(0,9);
	textcolor(15);
	gotoxy(10,4); printf("SAVAGE COIN - SOBRE");
	textcolor(10);
	setlocale(LC_ALL,"Portuguese");
	gotoxy(13,8);printf("Nosso fliperama 'Savage Coin' traz como proposta a volta dos");
	gotoxy(13,10);printf("t�o amados espa�os de jogos, famosos nas d�cadas de 80 e 90,");
	gotoxy(13,12);printf("com a mesma carinha das vers�es antigas!");
	gotoxy(13,16);printf("Este software cadastra nossos usu�rios, registrando seus dados ");
	gotoxy(13,18);printf("para possiveis consultas futuras!");
	gotoxy(13,22);printf("Os desenvolvedores s�o:");
	gotoxy(13,24);printf("Gabriel Tavares da Silva, n� 06, 71B;");
	gotoxy(13,26);printf("Sara Brand�o do Amaral, n� 30, 71B.");
	getch();
	
}

//-------------------------------------------------------------------------------------------------------------------------//

void menu() // Fun��o: Exibir as funcionalidades do sistema.
{
	
	moldura(0,10);
	gotoxy(35,4); textcolor(15); printf("              	      SAVAGE COIN%c   ",169);
	gotoxy(45,5); printf(" *Registrando jogadores com qualidade, desde 2020.*");
	aviso(15,15);
	imprimir_mario();
	int selecionar;
	do
	{
		
		textcolor(15); setlocale(LC_ALL,"PORTUGUESE");
		gotoxy(9,8); printf("Cadastro...");
		gotoxy(9,9); printf("Pesquisa...");
		gotoxy(9,10); printf("Informa��es");
		gotoxy(9,11); printf("Finalizar..");
				
		setlocale(LC_ALL,"C"); textcolor(4);
		
		for(int i = 8; i < 12; i++)
		{
			gotoxy(22,i); printf("%c",179);
		}
		
		selecionar = mover_seta( 6, 8, 11, 10, 1);
		switch(selecionar)
		{
			
			case 8: // Realiza o cadastro dos jogadores.
			cadastro();
			break;
			
			
			case 9: // Pesquisa de cadastros (dados dos registros).
			menu_pesquisa(); 
			menu();
			break;
			
			case 10: // Informa��es do sistema.
			sobre();
			menu();
			break;
			
		}
		
	} while ( selecionar != 11); // Finalizar o sistema.
	
	clrscr();
	textcolor(15);
	gotoxy(30,14); printf("Muito obrigado por utilizar o Savage Coin! <3");
	Sleep(2500);
	exit(0);
	
}

//-------------------------------------------------------------------------------------------------------------------------//

main() // Fun��o Principal.
{
	
	SetConsoleTitle("Savage Coin"); // Fun��o: Alterar o nome do t�tulo da barra do Windows.
	HWND wh = GetConsoleWindow(); // � necess�rio para utilizar o MoveWindow.
	MoveWindow(wh, 275, 131, 787, 511, TRUE); // Fun��o: Mover e redimensionar a janela do v�deo.
	system("mode con:cols=100 lines=30"); // Fun��o: Utilizado para redimensionar o tamanho do v�deo em linhas e colunas.
	abrir_arquivo("ab+");
	title_screen();
	tela_de_carregamento();
	menu();
	
}

//-------------------------------------------------------------------------------------------------------------------------//
