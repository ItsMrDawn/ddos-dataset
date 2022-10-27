#ifndef INDEXAR_H
#define INDEXAR_H
  
#pragma pack(push,1) //diretiva pra não usar padding no struct

//uma entrada no indice
struct entrada {

    unsigned int dado;
    unsigned int pos;

};
typedef struct entrada ENTRADA;

#pragma pack(pop) //desfazer a diretiva do padding

///////////////////////////////////////////////////////////////////////////////////////

int comp_entrada_dado (const void * a, const void * b);
int comp_entrada_pos_desc (const void * a, const void * b);

int cortar_colunas_csv();
int converter_para_bin();
int indexar();
int indexar_ip_n2(int nument);;
int verificar_arquivos();

  
#endif