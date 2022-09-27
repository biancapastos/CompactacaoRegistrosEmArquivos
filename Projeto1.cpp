#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

// -------------------- funcoes ------------------------------
int inserir(FILE *out,FILE *in);
void remover(FILE *in, FILE *out);
void compactar(FILE *in);
void abrir(FILE *in);

// ------------------- struct --------------------------------
typedef struct{
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
		fwrite(&num, sizeof(int), 1, out);
    }
    
    // abre arq insere.bin para leitura dos dados a serem inseridos
    if( (insere = fopen("insere.bin","r+b")) == NULL){
       
	    printf("Erro na abertura do arquivo");
        return 0;
    }
    
    // abre arq remove.bin para leitura dos dados a serem removidos
    if( (remove = fopen("remove.bin","r+b")) == NULL){
       
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
	            compactar(out);
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
    //fclose(remove);
    fclose(out);   
    return 1;
}

int inserir(FILE *out, FILE *in){
	
	// reposiciona os ponteiros para o inicio dos arquivos
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
    while( fread(&reg, sizeof(reg), 1, in) ){		// armazena na var reg um registro lido
    
        sprintf(aux, "%s", reg.isbn);				// var aux recebe o isbn
        
		//if(aux[0] == '/'){} //foi lido
        //else if(aux[0] == '@'){} //foi apagado
        
        if( aux[0] != '/'){							// se nao possui / no inicio do isbn é porque esse registro ainda nao foi lido
        
	        sprintf(registro, "%s#%s#%s#%s", reg.isbn, reg.titulo, reg.autor, reg.ano);		// var registro recebe a string formatada
	        tam_reg = strlen(registro);														// var tam_reg guarda o tamanho da string inteira
	        tam_reg++;
			
			// formata string com tamanho incluso e recalcula tam da string final
			sprintf(registro, "%d%s#%s#%s#%s", tam_reg, reg.isbn, reg.titulo, reg.autor, reg.ano);	
			tam_reg = strlen(registro);	
			tam_reg++;
				
			
// ---------------- AQUI ENTRA AS VERIFICACOES PARA PROCURAR ESPACOS REMOVIDOS PARA INSERIR --------------------------------- //
			
			// se header for -1 nenhum arq foi removido, insere no final do arquivo, caso contrario verifica lista ligada
			if(header == -1){
				
				fseek(out, 0, SEEK_END);								// posiciona no final do arq para escrever	
	        	fwrite(registro, sizeof(char), tam_reg, out);			// escreve no arq main a string formatada com o tam no inicio
			}
			// else {	
			//	}
			
// ------------------------------------------------------------------------------------------------------------------------ //
			
			// mofifica no arq insere o isbn com / no inicio para marcar aquele registro como já lido		
	        aux[0] = '/';
	        fseek(in, contador*sizeof(reg), SEEK_SET);
			fwrite(&aux, sizeof(reg.isbn), 1, in);
				
	        break;
	    }
	    contador++;			// contador para saber quantos registros ja foram lidos no insere
    
    }	// fim while
    return 1;
}

void remover(FILE *in, FILE *out){

	rewind(in);
	rewind(out);
	
	int header;					// recupera o que está no header no momento
	//int tam_reg;
	
	char aux[18];
	char isbn_remove[18];	
	char isbn_main[18];
	
	// enquanto conseguir ler arq remove, procura isbn que ainda nao foi usado
	while( fread(&aux, sizeof(aux), 1, in) ){

		//if(aux[0] == '/'){} 	// registro já foi removido
		
		printf("aux remove: %s -", &aux);
		if(aux[0] != '/'){
			
			sprintf(isbn_remove, "%s", aux);
			printf("isbn_remove: %s -", &isbn_remove);
			
			// recupera header
			fread(&header, sizeof(int), 1, out);
			printf("header no remove: %d -", &header);
			
			// busca no arq main isbn igual
			while( fread(&isbn_main, sizeof(isbn_main), 1, out) ){
				
				// separar tamanho no inicio do isbn para comparar
				
				printf("isbn_main: %s \n", &isbn_main);
				
				if( strcmp(isbn_main, isbn_remove) ){
					
					// processo de voltar a quantidade de bytes com fseek , recuperar offset, guardar numa variavel, 
					// substituir registro inteiro pela string <tam*header>,
					// colocar no header offset recuperado
				}
				
			}
			
			
		}
		//contador++;
	
	}

}

void compactar(FILE *in){}


/*

// recupera isbn + o tamanho do registro e salva no vetor registro_num_isbn
			do{
				fread(&aux,sizeof(char),1,out);
				while(aux != '#'){
					if(aux != '#'){
			
					registro_num_isbn[i] = aux;	
					fread(&aux,sizeof(char),1,out);
					i++;
					}
			}
		
			printf("%s\n",registro_num_isbn);
		
			//Nesse for ele separa o tamanho do registro do isbn	
			for(i = 0;i<strlen(registro_num_isbn);i++){
				if(i >= strlen(registro_num_isbn)-tam_isbn){
					isbn_main[j] = registro_num_isbn[i];
					j++;
				}
				else{
					tam_registro[k] = registro_num_isbn[i];
					k++;
				}
			}
			printf("%s\n",tam_registro);
			printf("%s\n",isbn_main);
			//Validar se as strings sao iguais
			validador = strcmp(isbn_main, isbn);
		
			//fseek(out, tam_registro - 13, SEEK_CUR);
			}while(validador != 0);
		
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
			isbn[0] = '/';
			fseek(in, contador*sizeof(isbn), SEEK_SET);
			fwrite(&isbn, sizeof(isbn), 1, in);
			break;

*/
