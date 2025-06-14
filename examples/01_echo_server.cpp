#include <iostream>

#include "httpserver.hpp"

int main() {
  shttp::http_server server;
  // We can just create http_server and run it. The default behaviout is send
  // Request as Respond. So it just echo server by default

  // But we can specify custom behaviour like this
  /*
    // setRequestHandler(ResponseCallback)
    // must accept only 1 parameter HttpRequest and return HttpResponse

    server.setRequestHandler(
        [](const shttp::HttpRequest& req) -> shttp::HttpResponse {
          std::cout << "Custom handler: " << req << '\n';
          return "It's custom handler:" + req ;
        });
  */

  // run function return HttpServerResult struct as the result
  // We can check for error using this structure and if there's one get the error message
  auto result = server.run();

  if(result.status != shttp::HttpServerStatus::OK) {
    std::cerr << "Server failed to run: " << result.error_message << '\n';
    return 1;
  }


  return 0;
}