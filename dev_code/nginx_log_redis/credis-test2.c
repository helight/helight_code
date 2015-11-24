/* credis-test.c -- a sample test application using credis (C client library 
 * for Redis)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "credis.h"

int main(int argc, char **argv) {
  REDIS redis;
  REDIS_INFO info;
  char *val, **valv;
  const char *keyv[] = {"kalle", "adam", "unknown", "bertil", "none"};
  int rc, keyc=5, i;


  redis = credis_connect(NULL, 0, 10000);
  if (redis == NULL) {
    printf("Error connecting to Redis server. Please start server to run tests.\n");
    exit(1);
  }

  printf("\n\n************* misc info ************************************ \n");

  rc = credis_info(redis, &info);
  printf("info returned %d\n", rc);
  printf("> redis_version: %s\n", info.redis_version);
  printf("> arch_bits: %d\n", info.arch_bits);
  printf("> multiplexing_api: %s\n", info.multiplexing_api);
  printf("> process_id: %ld\n", info.process_id);
  printf("> uptime_in_seconds: %ld\n", info.uptime_in_seconds);
  printf("> uptime_in_days: %ld\n", info.uptime_in_days);
  printf("> connected_clients: %d\n", info.connected_clients);
  printf("> connected_slaves: %d\n", info.connected_slaves);
  printf("> blocked_clients: %d\n", info.blocked_clients);
  printf("> used_memory: %zu\n", (size_t)info.used_memory);
  printf("> used_memory_human: %s\n", info.used_memory_human);
  printf("> changes_since_last_save: %lld\n", info.changes_since_last_save);
  printf("> bgsave_in_progress: %d\n", info.bgsave_in_progress);
  printf("> last_save_time: %ld\n", info.last_save_time);
  printf("> bgrewriteaof_in_progress: %d\n", info.bgrewriteaof_in_progress);
  printf("> total_connections_received: %lld\n", info.total_connections_received);
  printf("> total_commands_processed: %lld\n", info.total_commands_processed);
  printf("> expired_keys: %lld\n", info.expired_keys);
  printf("> hash_max_zipmap_entries: %zu\n", (size_t)info.hash_max_zipmap_entries);
  printf("> hash_max_zipmap_value: %zu\n", (size_t)info.hash_max_zipmap_value);
  printf("> pubsub_channels: %ld\n", info.pubsub_channels);
  printf("> pubsub_patterns: %u\n", info.pubsub_patterns);
  printf("> vm_enabled: %d\n", info.vm_enabled);
  printf("> role: %d\n", info.role);

  printf("\n\n************* get/set ************************************ \n");

  rc = credis_set(redis, "kalle", "kula");
  printf("set kalle=kula returned: %d\n", rc);

  rc = credis_get(redis, "kalle", &val);
  printf("get kalle returned: %s\n", val);

  rc = credis_type(redis, "someunknownkey");
  printf("get type unknown key returned: %d\n", rc);

  rc = credis_type(redis, "kalle");
  printf("get type known key returned: %d\n", rc);

  rc = credis_getset(redis, "kalle", "buhu", &val);
  printf("get set kalle=buhu returned: %s\n", val);

  rc = credis_get(redis, "kalle", &val);
  printf("get kalle returned: %s\n", val);

  rc = credis_del(redis, "kalle");
  printf("del kalle returned: %d\n", rc);

  rc = credis_get(redis, "kalle", &val);
  printf("get kalle returned: %s\n", val);

  rc = credis_set(redis, "adam", "aaa");
  rc = credis_set(redis, "bertil", "bbbbbbb");
  rc = credis_set(redis, "caesar", "cccc");
  rc = credis_get(redis, "adam", &val);
  printf("get adam returned: %s\n", val);
  rc = credis_get(redis, "bertil", &val);
  printf("get bertil returned: %s\n", val);
  rc = credis_get(redis, "caesar", &val);
  printf("get caesar returned: %s\n", val);

  rc = credis_mget(redis, keyc, keyv, &valv);
  printf("mget returned: %d\n", rc);
  for (i = 0; i < rc; i++)
    printf(" % 2d: %s\n", i, valv[i]);
     
  credis_close(redis);

  return 0;
}
