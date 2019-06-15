#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <map>
#include <functional>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <chrono> 

static int Help(const std::vector<std::string> &)
{
    std::cout << "Funções construídas" << std::endl;
    std::cout << "ls - Lista todos os conteúdos do diretório atual" << std::endl;
    std::cout << "mkdir <folder name> - Cria o diretório <folder name>" << std::endl;
    std::cout << "cd <new directory> - Muda o diretório para <new directory>" << std::endl;
    std::cout << "clear - Limpa a tela do terminal" << std::endl;
    std::cout << "clock - Mostra a data e hora atual" << std::endl;

    return 1;
}

static int Clock(const std::vector<std::string> &arguments){

    auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); 

    std::cout << ctime(&timenow) << std::endl; 
    return 1;
}

static int ChangeDir(const std::vector<std::string> &arguments)
{
    // char *_arguments;
    // strcpy(_arguments, arguments.c_str());
    
    if (arguments.size() > 1){
        if(chdir(arguments[1].data()) !=0) // data() retorna um ponteiro para o primeiro elemento do array
        {
            perror("chdir() to /error failed");   
        } 
    }

    return 1;
}


static int ListDir(const std::vector<std::string> &arguments)
{
    struct dirent **namelist;
    int n;

    n = scandir(get_current_dir_name(), &namelist, NULL, alphasort);
    if (n < 0)
        perror("scandir");
    else {
        while (n--) {
            printf("%s\n", namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }

    return 1;
}

static int NewDirectory(const std::vector<std::string> &arguments)
{
    if (arguments.size() > 1){
        mkdir(arguments[1].data(), 0777); //0777 permissões - RWX
    }
    else{
        std::cout <<  "Digite um novo diretório válido" << std::endl;
    }

    return 1;
}

static int Clear(const std::vector<std::string> &arguments)
{
    system("clear");
    return 1;
}

static int Quit(const std::vector<std::string> &arguments)
{
    std::cout << "Bye ..." << std::endl;
    exit(0);
}
// Mapeia uma string para uma função que retorna um status(int) e recebe um vetor de strings(args)
static std::map<std::string, std::function<int(const std::vector<std::string> &)>> Commands = {
    {"help", Help},
    {"cd", ChangeDir},
    {"ls", ListDir},
    {"mkdir", NewDirectory},
    {"clear", Clear},
    {"clock", Clock},
    {"quit", Quit}
};

void Loop();
std::string ReadLine();
std::vector<std::string> GetArgs(const std::string &_line); // passa a copia do valor do endereco
int Execute(const std::vector<std::string> &_args);
