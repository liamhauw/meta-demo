#include <exception>
#include <iostream>

#include "luka/engine.h"

int main(int argc, char** argv) {
  try {
    luka::Engine engine;
    engine.Run();
  } catch (const vk::SystemError& err) {
    std::cout << "vk::SystemError: " << err.what() << std::endl;
    exit(-1);
  } catch (const std::exception& err) {
    std::cout << "std::exception: " << err.what() << std::endl;
    exit(-1);
  } catch (...) {
    std::cout << "unknown error\n";
    exit(-1);
  }
  return 0;
}