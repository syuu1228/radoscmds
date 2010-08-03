#include <errno.h>
#include <stdlib.h>

#include <string>

#include <rados/librados.hpp>
using namespace librados;

static void usage(void)
{
  std::cout << "put_obj [bucket] [oid] [file_name]" << std::endl;
}

int main(int argc, const char **argv)
{
  if (argc < 4)
    usage();

  int r;
  std::string bucket(argv[1]);
  std::string oid(argv[2]);
  std::string file_name(argv[3]);
  pool_t pool;
  bufferlist bl;
  Rados rados;

  r = rados.initialize(0, NULL);
  if (r < 0)
    return r;


  r = rados.open_pool(bucket.c_str(), &pool);
  if (r < 0)
    return r;

  r = bl.read_file(file_name.c_str());
  if (r < 0)
    goto err;

  r = rados.write_full(pool, oid, bl);
  if (r < 0)
    goto err;

 err:
  rados.close_pool(pool);
  return r;
}
