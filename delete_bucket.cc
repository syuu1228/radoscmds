#include <errno.h>
#include <stdlib.h>

#include <rados/librados.h>
#include <rados/librados.hpp>
using namespace librados;

#include <string>
using namespace std;

Rados *rados = NULL;

static int open_pool(string& bucket, rados_pool_t *pool)
{
  return rados->open_pool(bucket.c_str(), pool);
}

int main(int argc, const char **argv)
{
  int r;
  std::string bucket(argv[1]);
  rados_pool_t pool;

  rados = new Rados();
  if (!rados)
    return -ENOMEM;

  r = rados->initialize(0, NULL);
  if (r < 0)
    goto err;

  r = open_pool(bucket, &pool);
  if (r < 0)
    goto err;

  r = rados->delete_pool(pool);
  if (r < 0)
    goto err;

 err:
  rados->close_pool(pool);
  rados->shutdown();
  delete rados;
  return r;
}
