#include "shell.h"

void Loop()
{
    std::string line;
    std::vector<std::string> args;
    int status = -1;

    pid_t pid = fork();

    // std::cout << "ID: " << getpid() << std::endl;

    if (pid < 0) // Falha no fork
	{
		std::cout << "erro" << std::endl;
	}

    if (pid != 0)
    {
        while (true)
        {
            waitpid(pid, &status, WUNTRACED);
            if ((WIFSIGNALED(status) || WIFEXITED(status)))
                break;
        }
    }
    else
    {
        std::string Temp;

        for (auto arg : args)
            Temp.append(arg);

        execvp(args[0].c_str(), (char *const *)Temp.data());
    }

    do
    {
        line = ReadLine();
        args = GetArgs(line);
        status = Execute(args);
    } while (status);
}

std::string ReadLine()
{
    std::cout << get_current_dir_name()
              << "> ";
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

    return Temp;
}

int Execute(const std::vector<std::string> &_args)
{
    int Status = 0;
    if (_args.size() == 0)
    {
        std::cout << "Digite um comando válido" << std::endl;
        return 1;
    }

    if (Commands.find(_args[0]) != Commands.end())
    {
        Status = Commands[_args[0]](_args);
    }
    else{
        std::cout << "Digite um comando válido" << std::endl;
        Status = 1;
    }

    return Status;
}