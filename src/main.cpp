#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;





  bool running = true;
  while(running){
    std::cout << "$ ";
    std::string line;
    std::getline(std::cin, line);
    if(line==""){
      continue;
    }
    std::vector<std::string> args;
    std::stringstream ss(line);
      std::string arg;
      while (ss >> arg) {
          args.push_back(arg);
      }
    std::vector<std::string> commands={"exit","echo","type","pwd","cd"};





    char* p=getenv("PATH");
    std::string path(p);
    std::vector<std::string> path_dirs;
    std::string str="";
    for(const char& i:path){
      if (i==':'){
        path_dirs.push_back(str);
        str="";
      }
      else{
        str.append(1,i);
      }
      
    }
    if(str!=""){
          path_dirs.push_back(str);
    }



    bool redirected=false;
    std::string output_file;
    for(int i=0;i<args.size();++i){
      if(args[i]==">"||args[i]=="1>"){
        redirected=true;
        if (i + 1 < args.size()) {
            output_file = args[i + 1];
        }
        args.resize(i); 

        break; 
      }
    }
    int pre_stdout = dup(STDOUT_FILENO);
    if(redirected){
      int fd= open(output_file.c_str(),O_WRONLY | O_CREAT | O_TRUNC, 0644);
      dup2(fd,STDOUT_FILENO);
      close(fd);

    }


    if (std::find(commands.begin(), commands.end(), args[0]) == commands.end()) {
      bool found=false;


        for(const auto& i:path_dirs){
          std::filesystem::path p = std::filesystem::path(i+"/"+args[0]);
          if(std::filesystem::exists(p)){
            if (access(p.c_str(), X_OK) == 0) {
              std::vector<char *>argv ;
              for (const auto& arg : args) {
                  argv.push_back(const_cast<char*>(arg.c_str()));
              }
              argv.push_back(nullptr);
              pid_t pid= fork();
              if(pid==0){
                execvp(p.c_str(), argv.data());
                exit(1);
              }
              waitpid(pid, nullptr, 0);
              found=true;
              break;
            }
          }
        }


        if(!found){
          std::cout<<args[0]<<": command not found"<<std::endl;
          continue;

        }
      
    }





    else if(args[0] == commands[0]){
      running = false;
      
    }





    else if(args[0]==commands[1]){
      for(int i=1;i<args.size();++i){
        std::cout<<args[i]<<" ";
      }
      
    }





    else if(args[0]==commands[2]){
      if (std::find(commands.begin(), commands.end(), args[1]) != commands.end()) {
        std::cout<<args[1]<<" is a shell builtin"<<std::endl;
      }





      else{
        bool found=false;
        for(const auto& i:path_dirs){
          std::filesystem::path p = std::filesystem::path(i+"/"+args[1]);
          if(std::filesystem::exists(p)){
            if (access(p.c_str(), X_OK) == 0) {
              std::cout<<args[1]<<" is "<<p.string()<<std::endl;
              found=true;
              break;
            }
          }
        }
        if(!found){
          std::cout<<args[1]<<": not found"<<std::endl;
          continue;
        }
      }
      
    
    }





    else if(args[0]==commands[3]){
      std::cout<<std::filesystem::current_path().string()<<std::endl;
      
    }





    else if(args[0]==commands[4]){
      std::string dir;
      std::cin.ignore();
      std::getline(std::cin,dir);
      if(dir=="~"){
        char* p=getenv("HOME");
        std::string path(p);
        std::filesystem::current_path(p);
        continue;
      }
      std::filesystem::path p=std::filesystem::path(dir);
      if(!std::filesystem::exists(p)){
        //cd: /does_not_exist: No such file or directory
        std::cout<<"cd: "<<p.string()<<": No such file or directory"<<std::endl;
      }
      else if(std::filesystem::current_path().string()!=dir){
        std::filesystem::current_path(dir);
      }
      
    }



    
    redirected=false;
    dup2(pre_stdout,STDOUT_FILENO);
    close(pre_stdout);
  }
}
