#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/json.hpp>

std::string list_dir(const std::string &dir);

int main() {
  std::string path = "/mnt/d/videos";
  auto res = list_dir(path);
  std::cout << res << "\n";
  return 0;
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
