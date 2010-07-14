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
  int ret;
  std::string bucket(argv[1]);

  rados = new Rados();
  if (!rados)
    return -ENOMEM;

  ret = rados->initialize(0, NULL);
  if (ret < 0)
   return ret;

  ret = open_root_pool(&root_pool);

  ret = rados->create(root_pool, bucket, true);
  if (ret < 0)
    return ret;

  ret = rados->create_pool(bucket.c_str());
  return ret;
}
