#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

// -------------------- funcoes ------------------------------
int inserir(FILE *out, FILE *in);
void remover(FILE *remove, FILE *out);
void compactar(FILE *in);
void recuperar_registro(FILE *in, char isbn[14]);

// ------------------- struct --------------------------------
typedef struct {
    char isbn[14];
    char titulo[50];
    char autor[50];
    char ano[5];
}Registro;


int main(){
	
    FILE *insere;			// ponteiro p/ arq insere.bin
    FILE *remove;			// ponteiro p/ arq remove.bin
    FILE *out;				// ponteiro p/ arq main criado para manipular os dados
    int op;  
    
    // se nao conseguir abrir o arq para leitura/escrita cria ele pela 1 vez (w+b) senao abre para manipulacao
	if( (out = fopen("main.bin","r+b")) == NULL ){
		
		out = fopen("main.bin","w+b");
		
		// adiciona header inicializando com -1 no arq main
        int num = -1;
		fwrite(&num,sizeof(int),1,out);      
    }
    
    if( (insere = fopen("insere.bin","r+b")) == NULL){			// abre arq insere.bin para leitura dos dados a serem inseridos
       
	    printf("Erro na abertura do arquivo");
        return 0;
    }

	if( (remove = fopen("remove.bin","r+b")) == NULL){			// abre arq insere.bin para leitura dos dados a serem inseridos
       
	    printf("Erro na abertura do arquivo");
        return 0;
    }


    do{
    	printf(" MENU \n 1 - Insercao \n 2 - Remocao \n 3 - Compactacao \n 4 - Carrega Arquivos \n Opcao: ");
    	scanf("%d",&op);
    	
        switch(op){
            case 1:{
	            inserir(out, insere);
	            break;
			}
            case 2:{
	            remover(remove, out);
	            break;
			}
            case 3:{
				break;
			}          
            case 0:{
				break;
			}
            default:{ 
				printf("opcão invalida\n");
				break;
			}
		}
		
    }while( op!=0 );

    fclose(insere);
    fclose(remove);
    fclose(out);   
    return 1;
}

int inserir(FILE *out, FILE *in){
	
	rewind(in);
	rewind(out);
	
    Registro reg;				// recebe os campos do registro lido no arq insere
    char registro[125];			// recebe string formatada
	int tam_reg;				// calcula tam da string formatada a ser inserida no main
    int contador = 0;			// conta quantos registros ja foram lidos
    char aux[14];				// recebe isbn e depois modifica com / para marcar qual ja foi lido no arq insere.bin
	
	// recupera header
    int header;
    fread(&header, sizeof(int), 1, out);

	// enquanto consegue ler o arq insere.bin recupera os dados
	while( fread(&reg, sizeof(reg), 1, in) ){					// recebe um registro lido
	
        sprintf(aux, "%s", reg.isbn);							// recebe o isbn
		//if(aux[0] == '/'){} //foi lido
        //else if(aux[0]== '@'){} //foi apagado
        
        if( aux[0] != '/'){							// quando nao tiver / registro no arq insere ainda nao foi inserido, entao le
        
	        sprintf(registro, "%s#%s#%s#%s", reg.isbn, reg.titulo, reg.autor, reg.ano);		// registro recebe a string formatada
	        tam_reg = strlen(registro);														// tam_reg guarda o tamanho da string inteira
	        tam_reg++;
			// ---------------- AQUI ENTRA AS VERIFICACOES PARA PROCURAR ESPACOS REMOVIDOS PARA INSERIR --------------------------------- //
			
			// se header for -1 nenhum arq foi removido, insere no final do arquivo, caso contrario verifica lista ligada
			if(header == -1){
				
				fseek(out, 0, SEEK_END);								// posiciona no final do arq para escrever
				fwrite(&tam_reg, sizeof(int), 1, out);					// escreve tam no inicio do isbn	
	        	fwrite(registro, sizeof(char), tam_reg, out);			// escreve no arq main a string formatada com o tam no inicio
			}
			// else {	
			//	}
			
	// ------------------------------------------------------------------------------------------------------------------------ //
	        
	        // mofifica arq insere com / no inicio do isbn para marcar como já lido
			aux[0] = '/';
	        fseek(in, contador*sizeof(reg), SEEK_SET);				// posiciona 
			fwrite(&aux, sizeof(reg.isbn), 1, in);					// escreve 
				
	        break;
	    }
	    contador++;		// contador para saber quantos registros ja foram lidos no insere
    
    }	// fim while
    return 1;
}

void remover(FILE *remove, FILE *out){
	
	rewind(remove);
	rewind(out);
	
	int header;
	int tam_registro;
	char aux;
	int pos;
	const int tam_isbn = 13;
	
	char isbn[14] = "";
	char isbn_main[14] = "";
	
	int validador;
	int i=0;
	int contador = 0;
	
	// enquanto conseguir ler arq remove, procura isbn que ainda nao foi usado
	while(fread(&isbn, 1, 13, remove) == 13){
		
		//if(isbn[0] == '/'){} //foi removido
		fseek(remove, 1, SEEK_CUR);
		
		if(isbn[0] != '/'){
			
			// recupera header
			fread(&header, sizeof(int), 1, out);
			
			//recupera isbn + tam registro e salva no vetor registro_num_isbn
			do{
				fread(&tam_registro, sizeof(int), 1, out);		// le um inteiro (tam_reg)
				
				fread(isbn_main, 13, 1, out);					// le isbn
				strcat(isbn_main, "\0");						// concatena \0 no final
				
				//Validar se as strings sao iguais
				validador = strcmp(isbn_main, isbn);
				printf("tam_reg: %d - isbnmain: %s, isbnremove: %s, strcmp: %d - ", tam_registro, &isbn_main, &isbn, strcmp(isbn_main, isbn));
				printf("validador: %d \n", validador);
				
				fseek(out, tam_registro-13, SEEK_CUR);
				
			}while(validador != 0);
			
			// volta a quant de bytes (isbn+tam)
			//fseek(out, ?, SEEK_CUR);
			//Salva a posição atual do arquivo
			//pos = ftell(out);
			//Novo header
			//header1 = pos - 13 - sizeof(int);
			//Vai pra posição
			//int tam_registro1 = (int)tam_registro;
			//fseek(out,pos-tam_registro1,SEEK_SET);
			//Reescreve
			//fwrite("*", 1, sizeof(char), out);
			
			//recuperar_registro(out,isbn);
			
			// marca no arq remove o registro que já foi excluido
			isbn[0] = '/';
			fseek(remove, contador*sizeof(isbn), SEEK_SET);
			fwrite(&isbn, sizeof(isbn), 1, remove);
			
			break;
		
		}
		contador++;
	
	}

}
