#!/bin/bash

echo
echo "Para obter a lista com os comandos disponíveis digite 'help' ou 'comandos'."
echo

# modififca o prompt
mostrar_prompt() { 
    echo -n "Script -> "
}

# função que exibe os comandos disponíveis
help() {
    echo
    echo "............................................"
    echo
    echo "      Lista de comandos disponíveis:"
    echo
    echo "-> listar"
    echo "-> mostrar"
    echo "-> inicio"
    echo "-> fim"
    echo "-> contar"
    echo "-> informacoes"
    echo "-> processos"
    echo "-> recursos"
    echo "-> tempo-atividade"
    echo "-> fusao-ficheiro"
    echo "-> utilizacao-cpu"
    echo "-> exit"
    echo 
    echo "Para mais informações sobre cada comando, digitar 'ajuda [nome do comando]'."
    echo
    echo "............................................"
    echo
}

# função que lista os ficheiros existentes na pasta. Utiliza o comando ls
listar () {
    echo "Lista dos ficheiros existentes na pasta:"
    ls
}

# função de ajuda do comando listar
ajuda_listar() {
    echo
    echo "listar"
    echo
    echo "Ajuda listar -> Listar arquivos e diretórios."
}

# função que mostra o conteúdo de um ficheiro. Utiliza o comando cat
mostrar () {
    ficheiro="$1" # ficheiro escolhido
    if [ -e "$ficheiro" ]; then 
        cat "$ficheiro"
        echo
    else
        echo "Ficheiro não encontrado!"
    fi
}

# função de ajuda do comando mostrar
ajudar_mostrar() {
    echo
    echo "mostrar [nome do ficheiro]"
    echo
    echo "Ajuda mostrar -> Mostrar conteúdo de um ficheiro específico."
}

# função para mostra as n primeiras linhas de um ficheiro. Utiliza o comando head
inicio () {
    ficheiro="$1" # ficheiro escolhido
    numero_linhas="$2" # nº de linhas pretendidas
    if [ -e "$ficheiro" ]; then 
        head -n "$numero_linhas" "$ficheiro"
    else
        echo "Ficheiro não encontrado!"
    fi
}
    
# função de ajuda do comando inicio
ajuda_inicio() {
    echo
    echo "inicio [nome do ficheiro] [numero de linhas]"
    echo
    echo "Ajuda início -> Pede o nome de um arquivo e mostra o número de linhas pedidas a começar pelo início."
}

# função para mostra as n últimas linhas de um ficheiro. Utiliza o comando tail
fim () {
    ficheiro="$1" # ficheiro escolhido
    numero_linhas="$2" # nº de linhas pretendidas
    if [ -e "$ficheiro" ]; then 
        tail -n "$numero_linhas" "$ficheiro"
    else
        echo "Ficheiro não encontrado!"
    fi
}

# função de ajuda do comando fim
ajuda_fim() {
    echo
    echo "fim [nome do ficheiro] [numero de linhas]"
    echo
    echo "Ajuda fim -> Pede o nome de um arquivo e mostra o número de linhas pedidas a começar pelo fim."
}

# função que conta o nº de palavras, linhas e caracteres de um ficheiro. Utiliza o comando wc
contar () {
    ficheiro="$1" # ficheiro escolhido
    if [ -e "$ficheiro" ]; then 
        palavras=$(wc -w < "$ficheiro") # nº de palavras
        linhas=$(wc -l < "$ficheiro") # nº de linhas
        caracteres=$(wc -c < "$ficheiro") # nº de caracteres
        echo "'$ficheiro' tem $palavras palavras;"
        echo "'$ficheiro' tem $linhas linhas;"
        echo "'$ficheiro' tem $caracteres caracteres."
    else
        echo "Ficheiro não encontrado!"
    fi
}

# função de ajuda do comando contar
ajuda_contar() {
    echo
    echo "contar [nome do ficheiro]"
    echo
    echo "Ajuda contar -> Solicita o nome de um arquivo e conta o número de palavras, linhas e caracteres do mesmo."
}

# função que exibe informções sobre o utilizador e o sistema. Utiliza os comandos id, who e uname
informacoes () {
    echo
    echo "Informações do utilizador atual:"
    id
    echo
    echo "Informações dos utilizadores atualmente conectados:"
    who
    echo
    echo "Informações do sistema:"
    uname
    echo
}

# função de ajuda do comando informações
ajuda_informacoes() {
    echo
    echo "informacoes"
    echo
    echo "Ajuda informações -> Fornece informações úteis sobre o utilizador atual, os conectados e os detalhes do sistema."
}

# função que mostra informações detalhadas sobre os processos de todos os utilizadores. Utiliza o comando ps
processos () {  
    echo
    echo "Processos em execução:"
    ps -au # a: Mostra os processos de todos os utilizadores; u: Mostar informações detalhadas sobre os processos
    echo
}

# função de ajuda do comando processos
ajuda_processos() {
    echo
    echo "processos"
    echo
    echo "Ajuda processos -> Lista os processos em execução no sistema e suas informações detalhadas."
}

# função que exibe informações sobre o uso de memória e o espaço de disco disponível. Utiliza os comandos free e df
recursos () {
    echo
    echo "Informações sobre uso da memória:"
    free -m # m: Estatísticas em megabytes
    echo
    echo "Informações sobre o espaço de disco disponível:"
    df -h # h: Passa a mostrar a informação em gigabytes (GB), megabytes (MB) e kilobytes (KB), em vez de blocos de 1 kilobyte (1K)
    echo
}

# função de ajuda do comando recursos
ajuda_recursos() {
    echo
    echo "recursos"
    echo
    echo "Ajuda recursos -> Obter informações sobre o uso de memória e espaço em disco envolvido no sistema."
}

# função que exibe o tempo da atividade atual. Utiliza o comando uptime
tempo-atividade () {
    echo
    echo "Tempo de atividade:"
    uptime
    echo
}

# função de ajuda do comando tempo-atividade
ajuda_tempo-atividade() {
    echo
    echo "tempo de atividade"
    echo
    echo "Ajuda tempo-atividade -> Obter informações sobre o tempo de atividade do sistema."
}

# função para combinar o conteúdo de dois ficheiros num distinto. Utiliza o comando cat
fusao-ficheiro () {
    ficheiro1="$1" # primeiro ficheiro escolhido
    ficheiro2="$2" # segundo ficheiro escolhido
    if [ -e "$ficheiro1" ] && [ -e "$ficheiro2" ]; then
        ficheiro_saida="$3" # nome do ficheiro de saída
        cat "$ficheiro1" "$ficheiro2" > "$ficheiro_saida"
        echo "'$ficheiro1' e '$ficheiro2' combinados com sucesso em '$ficheiro_saida'."
    elif [ ! -e "$ficheiro1" ] && [ ! -e "$ficheiro2" ]; then
        echo "Nenhum dos ficheiros foram encontrados."
    else
        echo "Um dos ficheiros não foi encontrado."
    fi
}

# função de ajuda do comando fusao-ficheiro
ajuda_fusao-ficheiro() {
    echo
    echo "fusao-ficheiro [nome do ficheiro1] [nome do ficheiro2] [nome do ficheiro de saída]"
    echo
    echo "Ajuda fusão-ficheiro -> Combina o conteúdo de dois ficheiros especificados e guarda em um novo ficheiro."
}

# função que exibe os cinco primeiros processos com maior uso de cpu. Utiliza os comandos ps, uname, sort e head com o mecanismo de piping
utilizacao-cpu () {
    echo
    echo "Os 5 primeiros processos com maior uso de CPU:"
    # Use "ps" para listar todos os processos, "sort" para classificá-los por uso de CPU, "head" para exibir os 5 primeiros e "uname" para exibir informações do sistema.
    (ps au && uname) | sort -k3 -r | head -n 6
}

# função de ajuda do comando utilizacao-cpu
ajuda_utilizacao-cpu() {
    echo
    echo "utilizacao-cpu"
    echo 
    echo "Ajuda utilizacao-cpu -> Mostra os cinco primeiros processos com maior uso de CPU. Este comando personalizado utiliza o mecanismo "piping" e inclui os seguintes comandos: ps, uname, sort e head."
}

# ciclo de leitura dos comandos introduzidos
while true; do 
    mostrar_prompt
    read comando argumento1 argumento2 argumento3 # comando escolhido e os argumentos
    
    # estrutura de escolha do comando introduzido
    case "$comando" in
        "help" | "comandos")
            help
            ;;
        "ajuda")
            if [ -n "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then 
                # estrutura de escolha do comando de ajuda introduzido
                case "$argumento1" in
                    "listar")
                        ajuda_listar
                        ;;
                    "mostrar")
                        ajudar_mostrar
                        ;;
                    "inicio")
                        ajuda_inicio
                        ;;
                    "fim")
                        ajuda_fim
                        ;;
                    "contar")
                        ajuda_contar
                        ;;
                    "informacoes")
                        ajuda_informacoes
                        ;;
                    "processos")
                        ajuda_processos
                        ;;
                    "recursos")
                        ajuda_recursos
                        ;;
                    "tempo-atividade")
                        ajuda_tempo-atividade
                        ;;
                    "fusao-ficheiro")
                        ajuda_fusao-ficheiro
                        ;;
                    "utilizacao-cpu")
                        ajuda_utilizacao-cpu
                        ;;
                    *)
                        echo "Comando não reconhecido. Digite 'help' ou 'comandos' para obter ajuda."
                        ;;
                esac
            else
                echo "Erro: número de argumentos deverá ser um."
            fi
            ;;
        "listar")
            # verifica se o número de argumentos é o pretendido
            if [ -z "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then
                listar
            else
                echo "Erro: número de argumentos deverá ser zero."
            fi
            ;;
        "mostrar")
            # verifica se o número de argumentos é o pretendido
            if [ -n "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then
                mostrar $argumento1
            else
                echo "Erro: número de argumentos deverá ser um."
            fi
            ;;
        "inicio")
            # verifica se o número de argumentos é o pretendido
            if [ -n "$argumento1" ] && [ -n "$argumento2" ] && [ -z "$argumento3" ]; then
                inicio $argumento1 $argumento2
            else
                echo "Erro: número de argumentos deverá ser dois."
            fi
            ;;
        "fim")
            # verifica se o número de argumentos é o pretendido
            if [ -n "$argumento1" ] && [ -n "$argumento2" ] && [ -z "$argumento3" ]; then
                fim $argumento1 $argumento2
            else
                echo "Erro: número de argumentos deverá ser dois."
            fi
            ;;
        "contar")
            # verifica se o número de argumentos é o pretendido
            if [ -n "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then
                contar $argumento1
            else
                echo "Erro: número de argumentos deverá ser um."
            fi
            ;;
        "informacoes")
            # verifica se o número de argumentos é o pretendido
            if [ -z "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then
                informacoes
            else
                echo "Erro: número de argumentos deverá ser zero."
            fi
            ;;
        "processos")
            # verifica se o número de argumentos é o pretendido
            if [ -z "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then
                processos
            else
                echo "Erro: número de argumentos deverá ser zero."
            fi
            ;;
        "recursos")
            # verifica se o número de argumentos é o pretendido
            if [ -z "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then
                recursos
            else
                echo "Erro: número de argumentos deverá ser zero."
            fi
            ;;
        "tempo-atividade")
            # verifica se o número de argumentos é o pretendido
            if [ -z "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then
                tempo-atividade
            else
                echo "Erro: número de argumentos deverá ser zero."
            fi
            ;;
        "fusao-ficheiro")
            # verifica se o número de argumentos é o pretendido
            if [ -n "$argumento1" ] && [ -n "$argumento2" ] && [ -n "$argumento3" ]; then
                fusao-ficheiro $argumento1 $argumento2 $argumento3
            else
                echo "Erro: número de argumentos deverá ser três."
            fi
            ;;
        "utilizacao-cpu")
            # verifica se o número de argumentos é o pretendido
            if [ -z "$argumento1" ] && [ -z "$argumento2" ] && [ -z "$argumento3" ]; then
                utilizacao-cpu
            else
                echo "Erro: número de argumentos deverá ser zero."
            fi
            ;;
        "exit")
            # opção de terminar a execução do programa
            echo "A terminar..."
            exit 0
            ;;
        *)
            # mensagem apresentada ao inserir um comando não existente
            echo "Comando não reconhecido. Digite 'help' ou 'comandos' para obter ajuda."
            ;;
        esac
    done