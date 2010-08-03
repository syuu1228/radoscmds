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
  std::string oid(argv[2]);
  std::string file_name(argv[3]);
  rados_pool_t pool;
  bufferlist bl;

  rados = new Rados();
  if (!rados)
    return -ENOMEM;

  r = rados->initialize(0, NULL);
  if (r < 0)
    goto err;

  r = rados->open_pool(bucket.c_str(), &pool);
  if (r < 0)
    goto err;

  r = bl.read_file(file_name.c_str());
  if (r < 0)
    goto err;

  r = rados->write_full(pool, oid, bl);
  if (r < 0)
    goto err;

 err:
  rados->close_pool(pool);
  delete rados;
  return r;
}
