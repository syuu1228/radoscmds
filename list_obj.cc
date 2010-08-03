#include <errno.h>
#include <stdlib.h>

#include <rados/librados.hpp>
using namespace librados;

#include <string>
using namespace std;

#define MAX_ENTRIES 1000

int main(int argc, const char **argv)
{
  int r;
  Rados rados;
  const char *bucket = argv[1];
  pool_t pool;
  Rados::ListCtx ctx;

  r = rados.initialize(0, NULL);
  if (r < 0)
    goto err;

  r = rados.open_pool(bucket, &pool);
  if (r < 0)
    goto err;

  rados.list_objects_open(pool, &ctx);
  do {
    list<string> entries;
    r = rados.list_objects_more(ctx, MAX_ENTRIES, entries);
    if (r < 0)
      goto err;

    for (list<string>::iterator iter = entries.begin(); iter != entries.end(); ++iter) {
      std::cout << *iter << std::endl;
    }
  } while (r);
  rados.list_objects_close(ctx);

 err:
  rados.close_pool(pool);
  rados.shutdown();

  return r;
}
