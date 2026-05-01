#include <iostream>
#include <string>
#include <vector>
std::vector<std::string> commands={"exit","echo","type"};

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  bool running = true;
  while(running){
    std::cout << "$ ";
    std::string command;
    std::cin>> command;
    
    if (std::find(commands.begin(), commands.end(), command) != commands.end()) {
      std::cout<<command<<": command not found"<<std::endl;
    }
    else if(command == commands[0]){
      running = false;
      continue;
    }
    else if(command==commands[1]){
      std::string echo_line;
      std::cin.ignore();
      std::getline(std::cin,echo_line);
      std::cout<<echo_line<<std::endl;
      continue;
    }
    else if(command==commands[2]){
      std::string command_type;
      std::cin>>command_type;
      if (std::find(commands.begin(), commands.end(), command_type) != commands.end()) {
        std::cout<<command_type<<": not found"<<std::endl;
      }
      else{
        std::cout<<command_type<<" is a shell builtin"<<std::endl;
      }
      continue;
    }
    
  }
  
  

}
