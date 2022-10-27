#include "indexar.h"
#include "registro.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////

//para uso com o quicksort
int comp_entrada_dado(const void * a, const void * b)
{

	//comparar dado, depois pos

    ENTRADA *entA = (ENTRADA*)a;
    ENTRADA *entB = (ENTRADA*)b;

    if (entA->dado < entB->dado) {
        return -1;
    } else if (entA->dado > entB->dado) { 
        return 1;
    } else {
		
		//dados iguais, comparar pos
        
		if (entA->pos < entB->pos) {
        	return -1;
    	} else if (entA->pos > entB->pos) { 
        	return 1;
		} else {
			return 0;
		}

    }

}

int comp_entrada_pos_desc(const void * a, const void * b)
{

	//comparar pos, depois dado (descendente)

    ENTRADA *entA = (ENTRADA*)a;
    ENTRADA *entB = (ENTRADA*)b;

    if (entA->pos < entB->pos) {
        return 1;
    } else if (entA->pos > entB->pos) { 
        return -1;
    } else {
		
		//pos igual, comparar dados
        
		if (entA->dado < entB->dado) {
        	return 1;
    	} else if (entA->dado > entB->dado) { 
        	return -1;
		} else {
			return 0;
		}

    }

}


//////////////////////////////////////////////////////////////////////////////////////////

int cortar_colunas_csv() {

    printf("Criando arquivo "DADOSCSV"\n");

    //converter os registros do arquivo dados.csv para cortar todas colunas alem das primeiras 6

    FILE *txtin;
    FILE *txtout;

    txtin = fopen(DATASET, "r");
    
    if ( txtin == NULL ){

        printf("Erro ao abrir o arquivo "DATASET" \n");
        return 1;

    }

    txtout = fopen(DADOSCSV, "w");

	char linha[10000];

	int j, cont, pos1, pos2;
	
	while (1) {
    //for (int i=0; i<100; i++) {
		
		fgets(linha, 10000, txtin);
		
		// fim do arquivo
		if (feof(txtin)) {
			break;
		}
		
		j    = 0;
		cont = 0;
        pos1 = 0;
        pos2 = 0;
		
		while (cont < 6) {
			
        	if (linha[j] == ','){
        		cont++;
                linha[j] = ' ';
			}	

            if (cont == 0) {
                pos1 = j;
            }

            if (cont == 1) {
                pos2 = j;
            }

            j++;
			
		}
			
        pos1++;
        pos2++;

        //j agora tem a posicao da ultima virgula
        //acabar a string antes da parte indesejada
		linha[j-1] = '\n';
		linha[j] = '\0';

        // tirar a coluna do "flow id"
        memmove(&linha[pos1], &linha[pos2], strlen(linha) - pos2 + 1);
		
        fputs(linha, txtout);
        fflush(txtout);
        
    }

    fclose(txtin);
    fclose(txtout);

    return 0;

}


int converter_para_bin() {

    printf("Criando arquivo "ARQBIN"\n");

    //converter os registros do arquivo dados.csv para um binario.bin

    FILE *bin;
    FILE *txt;

    REGISTRO reg;

    txt = fopen(DADOSCSV, "r");
    
    if ( txt == NULL ){

        printf("Erro ao abrir o arquivo "DADOSCSV"\n");
        return 1;

    }

    bin = fopen(ARQBIN, "wb");
    
    char flowid[1000];
    char ip_src_str[20], ip_dest_str[20];
    unsigned int  ip_int;

    fgets(flowid, 1000, txt); //1a linha (cabecalho)

    while (fscanf(txt, "%u %s %hu %s %hu", &reg.chave, &ip_src_str, &reg.port_src, &ip_dest_str, &reg.port_dest) != EOF) {
            
		ip_int  = ip_to_int(ip_src_str);
		reg.ip_src  = ip_int;
		
		ip_int = ip_to_int(ip_dest_str);
		reg.ip_dest = ip_int;
		
		fwrite(&reg, sizeof(REGISTRO), 1, bin);   
    }

    fclose(txt);
    fclose(bin);

    return 0;

}

int indexar() {

    printf("Criando arquivos "CHAVE_IDX" e "IP1_IDX"\n");

    //ler o arquivo binario.bin e criar um indice ordenado de IPs e um de chaves
    FILE *arqbin;

    arqbin = fopen(ARQBIN, "rb");    

    //calcular tamanho do arquivo e numero de registros
    long int tamreg = sizeof(REGISTRO);

    fseek(arqbin, 0L, SEEK_END); //SEEK_END vai no fim do arquivo
    int numreg = ftell(arqbin) / tamreg; //ftell == pos atual

    //voltar pro começo do arquivo
    rewind(arqbin);

    //alocar um vetor para todos os elementos dos dois indices
    ENTRADA* idxip    = malloc(numreg * sizeof(ENTRADA));
    ENTRADA* idxchave = malloc(numreg * sizeof(ENTRADA));
    ENTRADA  ent;

    REGISTRO reg;
    int i = 0;

    //ler os registros e montar um indice em memoria, no vetor  
    for (i = 0; i < numreg; i++) {
    
        fread(&reg, tamreg, 1, arqbin);

        ent.dado = reg.chave;
        ent.pos  = ftell(arqbin) - tamreg;

        idxchave[i] = ent;

        ent.dado = reg.ip_src;

        idxip[i] = ent;
     
    }

    fclose(arqbin);

    //ordenar ambos indices com o quick
    qsort (idxip,    numreg, sizeof(ENTRADA), comp_entrada_dado);
    qsort (idxchave, numreg, sizeof(ENTRADA), comp_entrada_dado);

    //gravar arquivos "ip1.idx" e "chave.idx"
    FILE *arqidx;
    
    arqidx = fopen(IP1_IDX, "wb");    
    fwrite(idxip, numreg * sizeof(ENTRADA), 1, arqidx); 
    fclose(arqidx);

    arqidx = fopen(CHAVE_IDX, "wb");    
    fwrite(idxchave, numreg * sizeof(ENTRADA), 1, arqidx); 
    fclose(arqidx);    

    free(idxip);
    free(idxchave);

    //criar o segundo nivel do indice de IP
    indexar_ip_n2(numreg);

    return 0;  

}

int indexar_ip_n2(int nument) {

    printf("Criando arquivo "IP2_IDX"\n");

    //como o arquivo de indice dos IPs tem muitos valores repetidos, criar um segundo nivel de indice
    FILE *arqidx1;

    arqidx1 = fopen(IP1_IDX, "rb");    

    //um vetor grande o suficiente pra pelo menos todos os elementos (se nao tivesse nenhum repetido por exemplo)
    ENTRADA* idxip2 = malloc(nument * sizeof(ENTRADA));
    ENTRADA  entn1, entn2;

    int i      = 0;
    int ip_ant = 0;
    int pos    = 0;

    //ler os registros e montar um indice em memoria, no vetor  
    for (i = 0; i < nument; i++) {
    
        fread(&entn1, sizeof(ENTRADA), 1, arqidx1);

        //mesmo IP
        if (entn1.dado == ip_ant) {
            continue;
        }

        //ip novo, insere no indice n2
        entn2.dado = entn1.dado;
        entn2.pos  = ftell(arqidx1) - sizeof(ENTRADA);
        idxip2[pos] = entn2;
        pos++;

        ip_ant = entn1.dado;
     
    }

    fclose(arqidx1);


    //gravar arquivo IP2_IDX
    FILE *arqidx2;
    
    arqidx2 = fopen(IP2_IDX, "wb");    
    fwrite(idxip2, pos * sizeof(ENTRADA), 1, arqidx2); 
    fclose(arqidx2);

    free(idxip2);


    return 0;  

}

//verificar se existem os arquivos binarios e de indice necessarios
int verificar_arquivos() {

    FILE *arqbin;
    FILE *arqcsv;
    FILE *idx;

    //sempre abrir o arquivo com nome fixo ARQBIN
    arqbin = fopen(ARQBIN, "rb");

    if ( arqbin == NULL ){

        //verificar se existe um DADOSCSV, que ja possui as colunas cortadas, e criar se necessario
        arqcsv = fopen(DADOSCSV, "r");

        if ( arqcsv == NULL ){
            cortar_colunas_csv();
        }

        fclose(arqcsv);
        
        //caso o arquivo binario não tenha sido criado ainda, converter de um arquivo DADOSCSV
        if (converter_para_bin()) {
            return 0;
        }

        arqbin = fopen(ARQBIN, "rb");

        if ( arqbin == NULL ){
            printf("Erro ao abrir o arquivo "ARQBIN"\n");
            return 0;
        }

	}  

    fclose(arqbin);
    
    //verificar se existem os arquivos de indice CHAVE_IDX e IP1_IDX
    arqbin = fopen(CHAVE_IDX, "rb");
    arqcsv = fopen(IP1_IDX, "rb");
    idx    = fopen(IP2_IDX, "rb");

    //se qualquer um nao existir, criar
    if ((arqbin == NULL ) || (arqcsv == NULL) || ( idx == NULL)){

        indexar();

	} 

    if (arqbin != NULL ) {
        fclose(arqbin);
    }

    if (arqcsv != NULL ) {
        fclose(arqcsv);
    }

    if (idx != NULL ) {
        fclose(idx);
    }
    
    return 1;

};
