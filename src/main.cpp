#include <iostream>
#include <string>


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  bool running = true;
  while(running){
    std::cout << "$ ";
    std::string command;
    std::cin>> command;
    if(command == "exit"){
      running = false;
      continue;
    }
    else if(command=="echo"){
      std::string echo_line;
      std::cin>>echo_line;
      std::cout<<echo_line<<std::endl;
      continue;
    }
    std::cout<<command<<": command not found"<<std::endl;
  }
  
  

}
