#include <errno.h>
#include <stdlib.h>

#include <rados/librados.hpp>
using namespace librados;

#include <string>
using namespace std;

#define ROOT_BUCKET ".rgw" //keep this synced to rgw_user.cc::root_bucket!
static string root_bucket(ROOT_BUCKET);
static pool_t root_pool;

/**
 * Open the pool used as root for this gateway
 * Returns: 0 on success, -ERR# otherwise.
 */
int open_root_pool(Rados &rados, pool_t *pool)
{
  int r = rados.open_pool(root_bucket.c_str(), pool);
  if (r < 0) {
    r = rados.create_pool(root_bucket.c_str());
    if (r < 0)
      return r;

    r = rados.open_pool(root_bucket.c_str(), pool);
  }

  return r;
}

int main(int argc, const char **argv)
{
  int r;
  Rados rados;
  const char *bucket = argv[1];
  string oid(argv[2]);
  string file_name(argv[3]);
  pool_t pool;
  bufferlist bl;
  //  off_t off = 0;
  //  size_t len = 0;

  r = rados.initialize(0, NULL);
  if (r < 0)
    goto err;

  r = open_root_pool(rados, &root_pool);
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
  rados.close_pool(&root_pool);
  rados.shutdown();

  return r;
}
