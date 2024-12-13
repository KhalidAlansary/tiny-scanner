#include <exception>
#include <string>

class ParserException : public std::exception {
 private:
  std::string message;

 public:
  ParserException(const std::string& msg) : message(msg) {}
  const char* what() const noexcept override { return message.c_str(); }
};
