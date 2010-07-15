#include <errno.h>
#include <stdlib.h>

#include <rados/librados.h>
#include <rados/librados.hpp>
using namespace librados;

#include <string>
using namespace std;

Rados *rados = NULL;

#define ROOT_BUCKET ".rgw" //keep this synced to rgw_user.cc::root_bucket!
static string root_bucket(ROOT_BUCKET);
static rados_pool_t root_pool;

/**
 * Open the pool used as root for this gateway
 * Returns: 0 on success, -ERR# otherwise.
 */
int open_root_pool(rados_pool_t *pool)
{
  int r = rados->open_pool(root_bucket.c_str(), pool);
  if (r < 0) {
    r = rados->create_pool(root_bucket.c_str());
    if (r < 0)
      return r;

    r = rados->open_pool(root_bucket.c_str(), pool);
  }

  return r;
}

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

  r = open_root_pool(&root_pool);
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
  rados->close_pool(&root_pool);
  delete rados;
  return r;
}
