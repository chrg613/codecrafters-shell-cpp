#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <unistd.h>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  bool running = true;
  while(running){
    std::cout << "$ ";
    std::string command;
    std::cin>> command;
    std::vector<std::string> commands={"exit","echo","type"};
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
      if(i==path.back()){
        path_dirs.push_back(str);
      }
    }
    
    if (std::find(commands.begin(), commands.end(), command) == commands.end()) {
      std::cout<<command<<": command not found"<<std::endl;
      continue;
    }
    if(command == commands[0]){
      running = false;
      continue;
    }
    if(command==commands[1]){
      std::string echo_line;
      std::cin.ignore();
      std::getline(std::cin,echo_line);
      std::cout<<echo_line<<std::endl;
      continue;
    }
    if(command==commands[2]){
      std::string command_type;
      std::cin.ignore();
      std::getline(std::cin,command_type);
      if (std::find(commands.begin(), commands.end(), command_type) != commands.end()) {
        std::cout<<command_type<<" is a shell builtin"<<std::endl;
      }
      else{
        bool found=false;
        for(const auto& i:path_dirs){
          std::filesystem::path p = std::filesystem::path(i+"/"+command_type) ;
          if(std::filesystem::exists(p)){
            if (access(p.c_str(), X_OK) == 0) {
              std::cout<<command_type<<" is "<<p.string()<<std::endl;
              found=true;
              break;
            }
          }
        }
        if(!found){
          std::cout<<command_type<<": not found"<<std::endl;
          continue;
        }
      }
      
    continue;
    }
    
  }
}
