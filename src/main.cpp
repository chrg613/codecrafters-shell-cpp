#include <iostream>
#include <string>


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  bool running = true;
  while(running){
    std::cout << "$ ";
    std::string x;
    std::cin>> x;
    std::cout<<x<<": command not found"<<std::endl;
  }
  
  

}
