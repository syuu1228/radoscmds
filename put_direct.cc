#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <rados/librados.hpp>
using namespace librados;

#define BUF_SIZE 100000000 /* 100MB */

static void usage(void)
{
  std::cout << "put_direct [bucket] [oid] [file_name]" << std::endl;
}

int main(int argc, const char **argv)
{
  if (argc < 4) {
    usage();
    return 1;
  }

  int r;
  std::string oid(argv[2]);
  pool_t pool = NULL;
  bufferlist bl;
  Rados rados;
  char *buf;
  FILE *fp;
  int len = 0;

  buf = (char *)malloc(BUF_SIZE);
  if (!buf)
	  return 1;

  fp = fopen(argv[3], "r");
  if (!fp)
	  return 1;

  while((r = fread(buf, 1, BUF_SIZE, fp)) > 0)
	  len += r;
  
  fclose(fp);
  
  r = rados.initialize(0, NULL);
  if (r < 0) {
    std::cerr << "initialize failed" << std::endl;
    return r;
  }


  r = rados.open_pool(argv[1], &pool);
  if (r < 0) {
    std::cerr << "open pool failed" << std::endl;
    goto err;
  }

  bl.push_front(ceph::buffer::create_static(len, buf));

  r = rados.write_full(pool, oid, bl);
  if (r < 0) {
    std::cerr << "write failed" << std::endl;
    goto err;
  }

 err:
  if (pool)
    rados.close_pool(pool);
  rados.shutdown();
  free(buf);
  return r;
}
