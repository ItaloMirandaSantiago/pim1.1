#include <stdio.h>
#include <string.h>


//typedef - semelhante a um objeto em js, sendo uma definição de um tipo de dado,
// por exemplo, "int", "float", neste caso o tipo de dado é "produto" 
//struct - serve para agrupar várias variáveis em um único tipo

typedef struct {
    char nome[100];
    float preco;
} Produto;


//função, recebe uma string "nome"
// retorna 0 se não for encontrado o produto ou se o arquivo não existir e 1 se encontrado
int verificarNome(const char* nome) {
    //FILE *file ponteiro para manipular o arquivo Produtos
    //"r" para abrir em modo leitura
    FILE *file = fopen("Produtos.txt", "r");
    if (file == NULL) {
        return 0;
    }
    //loop para ler todo o arquivo
    //linha[150] é para armazenar todas as caracteres da linha
    char linha[150];

    //sizeof(linha) é o tamanho máximo permitido

    while (fgets(linha, sizeof(linha), file)) {

        // strstr função interna da linguagem, serve para pegar toda a linha e procurar o nome do produto
        //se for diferente de null, significa que ele achou, por isso ele retorna 1
        if (strstr(linha, nome) != NULL) {

            //fclose é o fechamento do arquivo
            fclose(file);
            return 1;
        }
    }
    //produto não achado
    fclose(file);
    return 0;
}

//recebe uma string
float buscarPrecoProduto(const char* nomeProduto) {
    //array de 100 produtos, tendo dentro de cada um deles nome e preço 
    Produto produtos[100];
    //chamado da função lerProdutos
    int qtdProdutos = lerProdutos(produtos);

    //pecorrer cada elemento
    for (int i = 0; i < qtdProdutos; i++) {
        //verifica se existe e retorna o preço
        if (strcmp(produtos[i].nome, nomeProduto) == 0) {
            return produtos[i].preco;
        }
    }
//falha - produto não encontrado
    return -1; 
}



int lerProdutos(Produto* produtos) {
    //abre o arquivo
    FILE *file = fopen("Produtos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }
    //quantidade de produtos zero
    int qtdProdutos = 0;

   //transforma em string fgts
    while (fgets(produtos[qtdProdutos].nome, sizeof(produtos[qtdProdutos].nome), file) != NULL) {

        //extrai as informações de cada produto
        //produtos[qtdProdutos].nome fica após o Nome: e ele pega essa informção
        sscanf(produtos[qtdProdutos].nome, "Nome: %[^,], Preço: %f", produtos[qtdProdutos].nome, &produtos[qtdProdutos].preco);
        
        //a cada produto lido ele acrescenta um em qtdProdutos, para ler o próximo

        qtdProdutos++;
    }
    //fecha arquivo e retornar a quantidade de arquivos lidos
    fclose(file);
    return qtdProdutos;
}

// não retorna nada e pega um numero e o objeto produtos, contendo nome e preço
void salvarProdutos(Produto* produtos, int qtdProdutos) {
    //abre o arquivo em modo escrita
    FILE *file = fopen("Produtos.txt", "w");

    //verifca se o arquivo existe
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar!\n");
        return;
    }

    //escreve os dados de todos os produtos, dnv e atualiza
    for (int i = 0; i < qtdProdutos; i++) {
        fprintf(file, "Nome: %s, Preço: %.2f\n", produtos[i].nome, produtos[i].preco);
    }
    //fecha
    fclose(file);
}


//não retorna nada
void listarProdutos() {
    Produto produtos[100];

    //ler produtos
    int qtdProdutos = lerProdutos(produtos);

    //mostra na tela todos os produtos
    printf("Lista de Produtos:\n");
    
    for (int i = 0; i < qtdProdutos; i++) {
        printf("Nome: %s, Preço: %.2f\n", produtos[i].nome, produtos[i].preco);
    }
}

void CompraDeProdutos() {

}

int main(int argc, char const *argv[]) {
    int numeroInicial;

    printf("Bem-vindo, selecione o número de uma das opções a seguir \n");
    printf("1 - administrador \n");
    printf("2 - caixa \n");
    printf("3 - consulta \n \n");

    scanf("%d", &numeroInicial);

    if (numeroInicial == 1) {
        int numeroProdutos;
        Produto produtos[100];
        int qtdProdutos = lerProdutos(produtos);

        printf("Selecione o número de uma das opções a seguir \n");
        printf("1 - criar produto \n");
        printf("2 - editar produto \n");
        printf("3 - excluir produto \n \n");   

        scanf("%d", &numeroProdutos);

        if (numeroProdutos == 1) {
            Produto produto;
            printf("Digite o nome do novo produto \n");
            scanf("%s", produto.nome);

            printf("Digite o preço do novo produto \n");
            scanf("%f", &produto.preco);

            FILE *file = fopen("Produtos.txt", "a");
            if (file == NULL) {
                printf("Erro ao abrir o arquivo!\n");
                return 1;
            }

            int verificacao = verificarNome(produto.nome);
            if (verificacao == 1) {
                printf("Produto já cadastrado\n");
            } else {
                fprintf(file, "Nome: %s, Preço: %.2f\n", produto.nome, produto.preco);
                fclose(file); 
                printf("Produto '%s' com preço %.2f criado e salvo com sucesso!\n", produto.nome, produto.preco);
            }

        } else if (numeroProdutos == 2) {
            char nomeProduto[100];
            printf("Digite o nome do produto que deseja editar: \n");
            scanf("%s", nomeProduto);

            int encontrado = -1;
            for (int i = 0; i < qtdProdutos; i++) {
                if (strcmp(produtos[i].nome, nomeProduto) == 0) {
                    encontrado = i;
                    break;
                }
            }

            if (encontrado != -1) {
                printf("Produto encontrado: Nome: %s, Preço: %.2f\n", produtos[encontrado].nome, produtos[encontrado].preco);
                printf("Digite o novo nome (ou pressione Enter para manter o atual): ");
                char novoNome[100];
                getchar(); 
                fgets(novoNome, sizeof(novoNome), stdin);
                novoNome[strcspn(novoNome, "\n")] = 0; 

                if (strlen(novoNome) > 0) {
                    strcpy(produtos[encontrado].nome, novoNome);
                }

                printf("Digite o novo preço (ou 0 para manter o atual): ");
                float novoPreco;
                scanf("%f", &novoPreco);
                if (novoPreco > 0) {
                    produtos[encontrado].preco = novoPreco;
                }

                salvarProdutos(produtos, qtdProdutos);
                printf("Produto editado com sucesso!\n");
            } else {
                printf("Produto não encontrado!\n");
            }

        } else if (numeroProdutos == 3) {
            char nomeProduto[100];
            printf("Digite o nome do produto que deseja excluir: \n");
            scanf("%s", nomeProduto);

            int encontrado = -1;
            for (int i = 0; i < qtdProdutos; i++) {
                if (strcmp(produtos[i].nome, nomeProduto) == 0) {
                    encontrado = i;
                    break;
                }
            }

            if (encontrado != -1) {
                
                for (int i = encontrado; i < qtdProdutos - 1; i++) {
                    produtos[i] = produtos[i + 1]; 
                }
                qtdProdutos--; 

                salvarProdutos(produtos, qtdProdutos);
                printf("Produto '%s' excluído com sucesso!\n", nomeProduto);
            } else {
                printf("Produto não encontrado!\n");
            }
        }

    } else if (numeroInicial == 2) {

 printf("Você escolheu caixa.\n");
        
        char nomeProduto[100];
        float total = 0.0;
        int continuar = 1;

        while (continuar) {
            listarProdutos();
            printf("Digite o nome do produto para adicionar ao total ou 'sair' para finalizar: ");
            scanf("%s", nomeProduto);

            if (strcmp(nomeProduto, "sair") == 0) {
                break;
            }

            float preco = buscarPrecoProduto(nomeProduto);
            if (preco >= 0) {
                total += preco;
                printf("Produto '%s' adicionado com preço %.2f. Total até agora: %.2f\n", nomeProduto, preco, total);
            } else {
                printf("Produto não encontrado!\n");
            }
        }

        printf("O total final é: %.2f\n", total);
        
        
    
    
    } else if (numeroInicial == 3) {
        char produtoselecionado[100];

         printf("Lista de Produtos:\n");

         listarProdutos();
         
    } else {
        printf("Selecione uma opção válida!\n");
    }

    return 0;
}
