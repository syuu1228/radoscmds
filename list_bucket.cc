#include <errno.h>
#include <stdlib.h>

#include <rados/librados.h>
#include <rados/librados.hpp>
using namespace librados;

#include <string>
using namespace std;

Rados *rados = NULL;

int main(int argc, const char **argv)
{
  int ret;
  std::list<string> list;

  rados = new Rados();
  if (!rados)
    return -ENOMEM;

  ret = rados->initialize(0, NULL);
  if (ret < 0)
   return ret;

  ret = rados->list_pools(list);
  if (ret < 0)
    return ret;

  std::list<std::string>::iterator it = list.begin();
  while(it != list.end()) {
    std::cout << *it << std::endl;
    it++;
  }

  return 0;
}
