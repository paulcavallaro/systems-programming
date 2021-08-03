#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/str_join.h"
#include "glog/logging.h"

int main(int argc, char* argv[]) {
  // Initialize Google’s logging library.
  google::InitGoogleLogging(argv[0]);

  // Join some strings
  std::vector<std::string> v = {"foo", "bar", "baz"};
  std::string s = absl::StrJoin(v, ", ");

  LOG(ERROR) << "Joined string: " << s << "\n";

  return 0;
}
