#include "shell.h"


void Loop()
{
    std::string line;
    std::vector<std::string> args;
    int status;

    do
    {
        line = ReadLine();
        args = GetArgs(line);
        status = Execute(args);
    } while (status);
}

int SystemCommands(std::vector<std::string> args_orig)
{
        int status;
        pid_t pid = fork(); // cria o processo filho e armazena o PID retornado da função fork

        char **args = new char *[args_orig.size() + 1];
        
        for(int i = 0; i < args_orig.size(); i++)
        {
            args[i] = new char[args_orig[i].size()];
            strcpy(args[i], args_orig[i].c_str());
        }

        args[args_orig.size()] = NULL;

        if (pid < 0) // Falha no fork
        {
            perror("lsh");
            return -1;
        }

        else{
            if (pid != 0) // Caso fork() retorne o PID do filho, ele é o processo pai
            {
                while (true)
                {
                    waitpid(pid, &status, WUNTRACED); // espera o filho terminar o processo
                    if ((WIFSIGNALED(status) || WIFEXITED(status)))
                        break;
                }
            }
            else
            {
                if (execvp(args[0], args) == -1) { // filho executa o comando
                        perror("lsh");
                    }
                    exit(EXIT_FAILURE);

                    /*modifica a imagem do processo filho para o arquivo chamado*/
            }

            for(int i = 0; i < args_orig.size(); i++)
            {
                delete[] args[i];
                args[i] = nullptr;
            }
            
            delete[] args;
            args = nullptr;
        
        return 1;
        }
}

std::string ReadLine()
{

    std::cout <<  "\x1b[35m" <<get_current_dir_name()
              << "> " << "\x1b[0m";
    char *Temp;
    size_t Size = 0;

    if (getline(&Temp, &Size, stdin) == -1)
    {
        return "";
    }

    return Temp;
}

std::vector<std::string> GetArgs(const std::string &_line)
{
    std::vector<std::string> Temp;
    char *TempChar = new char[64];
    strcpy(TempChar, _line.c_str());
    // copia o valor da linha para o ponteiro tempchar
    // a função strtok recebe um ponteiro de char, entao copiamos o conteudo da linha para um ponteiro de char
    char *TempTokens = strtok(TempChar, " \n\b\t\r"); // a primeira chamada da função passa um ponteiro da string e retorna tudo que encontrar antes dos tokens

    while (TempTokens != nullptr)
    {
        Temp.push_back(TempTokens);
        TempTokens = strtok(0, " \n\b\t\r"); // nessa segunda chamada, o argumento 0 é usado para o ponteiro começar de onde parou
    }

    delete[] TempChar;
    delete[] TempTokens;
    // Boa pratica
    TempChar = nullptr;
    TempTokens = nullptr;

    // Temp.push_back((char*)0x00);

    return Temp;
}

int Execute(const std::vector<std::string> &_args)
{
    if (_args.size() == 0)
    {
        return 1;
    }

    if (Commands.find(_args[0]) != Commands.end())
    {
        return Commands[_args[0]](_args);
    }
    
    return SystemCommands(_args);
}