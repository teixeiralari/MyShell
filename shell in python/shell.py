import os
import sys
import datetime

class Shell:

    def ChangeDir(args):
        if len(args) > 1:
            try:
                os.chdir(args[1])
            except Exception as e:
                print('chdir() error: {0}'.format(e))
        return 1

    def Clock(args):
        now = datetime.datetime.now()
        print(now.strftime("%A, %d. %B %Y %I:%M%p"))
        return 1

    def Help(args):
        print('The following are built in: \n')
        for key in Shell.MyCommands.keys():
            print(key)
        return 1

    def Quit(args):
        return 0
    
    MyCommands = {
        'cd' : ChangeDir,
        'quit' : Quit,
        'clock' : Clock,
        'help' : Help
    }

    def __init__(self):
        status = 1
        while(status):
            line = self.ReadLine()
            args = self.GetArgs(line)
            status = self.Execute(args)

    def ReadLine(self):
        return input('\033[33m' + os.getcwd() + '> \033[0m')

    def GetArgs(self, command):
        return command.split(' ')

    def Execute(self, args):
        if(len(args) == 0):
            return 1
        if args[0] in Shell.MyCommands.keys():
            return Shell.MyCommands[args[0]](args)

        return self.SystemCommands(args)

    def SystemCommands(self, args):
        try:
            pid = os.fork()
        except Exception as e:
            print('fork() error : {0}'.format(e))
        
        if pid != 0:
            while(True):
                _, status = os.waitpid(pid, os.WUNTRACED)
                if ((os.WIFSIGNALED(status) or os.WIFEXITED(status))):
                    break
        else:
            try:
                os.execvp(args[0], args)
            except Exception as e:
                print('execvp() error : {0}'.format(e))
            os._exit(-1)

        return 1 
    
shell = Shell()
