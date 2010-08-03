#include <errno.h>
#include <stdlib.h>

#include <rados/librados.hpp>
using namespace librados;

#include <string>
using namespace std;

int main(int argc, const char **argv)
{
  int r;
  Rados rados;
  const char *bucket = argv[1];
  string oid(argv[2]);
  string file_name(argv[3]);
  pool_t pool;
  bufferlist bl;

  r = rados.initialize(0, NULL);
  if (r < 0)
    goto err;

  r = rados.open_pool(bucket, &pool);
  if (r < 0)
    goto err;


  r = rados.read(pool, oid, 0, bl, 0);
  if (r < 0)
    goto err;

  r = bl.write_file(file_name.c_str());

 err:
  rados.close_pool(pool);
  rados.shutdown();

  return r;
}
