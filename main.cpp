#include <boost/filesystem.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <vector>

#include "httplib.h"

void warn_usage();

std::string list_dir(const std::string &dir);

int main(int argc, char **argv) {
  if (argc != 2) {
    warn_usage();
    return 1;
  }

  httplib::Server server;

  const std::string DIR = argv[1];
  const std::string URL = "/files";

  const std::string host = "0.0.0.0";
  const int port = 8080;

  std::cout << "mounting " << DIR << " to " << URL << '\n';
  auto ret = server.set_mount_point(URL, DIR);
  if (!ret) std::cerr << "error mounting file directory.\n";
  ret = server.set_mount_point("/", "./frontend");
  if (!ret) std::cerr << "error mounting frontend resources.\n";

  server.Post("/list", [&DIR](const httplib::Request &req, httplib::Response &res) {
    std::cout << "requesting path: " << req.body << "\n";
    auto fileList = list_dir(DIR + req.body);
    std::cout << "file list: \n" << fileList << "\n";
    std::cout << "sending file list...\n\n";
    res.set_content(fileList, "text/plain");
  });

  std::cout << "listening at " << host << ":" << port << "...\n";
  server.listen(host, port);

  return 0;
}

void warn_usage() {
  std::cerr << "wrong usage\n"
            << "usage:\n"
            << "[executable] [target dir]\n"
            << "usage example:\n"
            << "./main ./frontend\n";
}

std::string list_dir(const std::string &dir) {
  using namespace boost::filesystem;
  path p(dir);
  try {
    if (exists(p)) {
      if (is_directory(p)) {
        boost::json::array res;
        for (directory_entry &x : directory_iterator(p)) {
          boost::json::object info;
          info["name"] = x.path().filename().string();
          info["isDir"] = is_directory(x.path());
          res.push_back(info);
        }
        return boost::json::serialize(res);
      } else {
        std::cerr << "[list_dir]: " << p << " exists, but is not a directory\n";
      }
    } else {
      std::cerr << "[list_dir]: " << p << " does not exist\n";
      return "error";
    }
  } catch (const filesystem_error &ex) {
    std::cerr << "[list_dir]: " << ex.what() << '\n';
    return "error";
  }
  return "error";
}