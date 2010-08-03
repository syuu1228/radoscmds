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
  int r;
  std::string bucket(argv[1]);

  rados = new Rados();
  if (!rados)
    return -ENOMEM;

  r = rados->initialize(0, NULL);
  if (r < 0)
    goto err;

  r = rados->lookup_pool(bucket.c_str());
  printf("lookup_pool:%d\n", r);
 err:
  rados->shutdown();
  delete rados;
  return r;
}
