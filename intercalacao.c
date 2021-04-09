#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "nomes.c"
#include "cliente.c"

#include <limits.h>
#include <dirent.h>
#include <string.h>

typedef struct vetor {
    TCliente *cli;
    FILE *f;
} TVet;

typedef struct vetorNomes {
    char *nome;
} TVetNomes;

TNomes *gera_nomes_particoes(int num){
    TVetNomes nomes[num];
    TNomes* lista_nomes_particoes = NULL;
    for (int i = num-1; i >= 0; i--) {
        nomes[i].nome = (char *) malloc(sizeof(char[10]));
        char num_particao[10];
        strcpy(nomes[i].nome, "p");
        sprintf(num_particao, "%d", i+1);
        strcat(nomes[i].nome, num_particao);
        strcat(nomes[i].nome, ".txt");
        lista_nomes_particoes = insere_inicio(lista_nomes_particoes, nomes[i].nome);
    }
    return lista_nomes_particoes;
}

char *gera_nome_particao(int num_part) {
    char *nome_particao = (char *) malloc(sizeof(char[10]));
    char num_particao[10];

    strcpy(nome_particao, "p");
    sprintf(num_particao, "%d", num_part);
    strcat(nome_particao, num_particao);
    strcat(nome_particao, ".txt");

    return nome_particao;
}

void imprime_arquivo(char *name) {
    FILE *arq; //declara ponteiro para arquivo
    // abre arquivo para leitura
    arq = fopen(name, "r");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char linha[150];
        fgets (linha, 150, arq);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s", linha);
            fgets (linha, 150, arq);
        }
        fclose(arq); //fecha arquivo
    } else printf("Erro ao abrir arquivo\n");
}

TVet *ordena_vetor(TVet *vet, int tam){
    TCliente *cliente;
    for(int i = tam-1; i>0;i = i-1){
            for(int j=0; j<i; j=j+1){
                if(vet[j].cli!=NULL){
                    if(vet[j].cli->cod_cliente > vet[j+1].cli->cod_cliente){
                        cliente = vet[j].cli;
                        vet[j].cli = vet[j+1].cli;
                        vet[j+1].cli = cliente;
                    }
                }
            }
        }
    return vet;
}

int intercalacao_otima(char *nome_arquivo_saida, int num_p, TNomes *nome_particoes, int f) {
    TNomes *aux_nome_particoes= nome_particoes;
    //TODO Implementar essa função
    TCliente *cliente;
    //qual o numero da partição gerada atual
    int count_particao_gerada;
    //contador para o retorno da função
    int count =0;
    while(aux_nome_particoes->prox != NULL){
        count = count + 1;
        TVet *vet = (TVet*)malloc(sizeof(TVet));
        count_particao_gerada = (conta_nomes(nome_particoes) + 1);
        char *nome_particao_gerada = gera_nome_particao(count_particao_gerada);

        FILE *particao_gerada;
        //para checar quando o ultimo arquivo é o utilizado
        TNomes *checa_ultimo_arq = aux_nome_particoes;
        for(int a =0;a<f-1;a=a+1){
            checa_ultimo_arq=checa_ultimo_arq->prox;
        }
        
        if(checa_ultimo_arq!=NULL){
            particao_gerada = fopen(nome_particao_gerada,"w");
        }
        else{
            //caso seja o ultimo arquivo
            particao_gerada = fopen("saida.txt", "w");
        }

        //adiciona a nova particao no vetor de nomes das particoes
        nome_particoes = insere_fim(nome_particoes, nome_particao_gerada);
        //contador da posição que cliente deve ser adicionado ao vetor
        int count_vet = 0;

        for(int a =0; a < f-1; a=a+1){
            //char *nome = gera_nome_particao(particao_atual);
            FILE *arq = fopen(aux_nome_particoes->nome, "r");
            while (!feof(arq))
            {
                cliente = le_cliente(arq);
                if (cliente != NULL){
                    vet[count_vet].cli = cliente;
                    vet[count_vet].f = particao_gerada;
                    //printf("%s\n", vet[count_vet].cli->nome);
                    count_vet = count_vet +1;
                    
                    
                    //alocando espaço para o proximo cliente no vetor
                    vet = (TVet*)realloc(vet,(count_vet+1)*sizeof(TVet));
                }   
            }
           aux_nome_particoes = aux_nome_particoes->prox;
           
            fclose(arq);
        }

        //algoritmo de ordenação
        vet = ordena_vetor(vet, count_vet);

        
        
        //printf("\n\n\n");
        
        for(int a =0;a<count_vet;a=a+1)
        {
            //printf("%d\n", vet[a].cli->cod_cliente);
            fprintf(particao_gerada,"%d;%s;\n", vet[a].cli->cod_cliente,vet[a].cli->nome);
            
        }
        



    
    free(vet);
    fclose(particao_gerada);
    }
    return count;
    
}

int main() {
    int num_particoes;
    int f;
    int num_particoes_intermediarias;
    TNomes *nomes_particoes = NULL;
    //le numero de particoes a serem intercaladas
    scanf("%d", &num_particoes);
    nomes_particoes = gera_nomes_particoes(num_particoes);
    //le f (número total de arquivos a serem manipulados ao mesmo tempo)
    //lembrar que o método usa f-1 arquivos para leitura, e 1 arquivo para saída
    scanf("%d", &f);
    num_particoes_intermediarias = intercalacao_otima("saida.txt", num_particoes, nomes_particoes, f);
    printf("%d\n", num_particoes_intermediarias);
    printf("*** saida.txt\n");
    imprime_arquivo("saida.txt");
}
