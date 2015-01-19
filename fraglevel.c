/*
* Get fragmentation level per each memory zone in percentage
* @ton31337
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUF 1024
#define OFFSET 4

struct blocks_info {
  int *blocks;
  int count;
};

struct zone_info {
  char zone_name[16];
  struct blocks_info blk_info;
  struct zone_info *next;
};

char *get_zone_name(char *);
int *get_zone_blocks(char *, int *);
int total_free_page(struct blocks_info *);
int *fragmentation_level(struct blocks_info *);

int main()
{
  int fd;
  int r;
  int count = 0;
  char buddy_info[MAX_BUF];
  char *saved;
  char *line;
  struct zone_info *zones;
  struct zone_info *head;

  fd = open("/proc/buddyinfo", O_RDONLY);
  if(!fd)
    goto just_exit;

  memset(buddy_info, 0, MAX_BUF);
  r = read(fd, buddy_info, MAX_BUF);
  if(!r)
    goto just_exit;

  zones = malloc(sizeof(struct zone_info));
  head = zones;
    
  line = strtok_r(buddy_info, "\n", &saved);
  while(line != NULL) {
    strcpy(zones->zone_name, get_zone_name(line));
    zones->next = malloc(sizeof(struct zone_info));
    zones->blk_info.blocks = get_zone_blocks(line, &count);
    zones->blk_info.count = count;
    zones = zones->next;
    line = strtok_r(NULL, "\n", &saved);
  }

  zones = head;
  while(zones->next != NULL) {
    int k = 0;
    int i = 0;
    int sum;
    int result = 0;
    int total_free_pages = total_free_page(&zones->blk_info);
    printf("=== Zone %s free_pages %d\n", zones->zone_name, total_free_pages);
    int *x = fragmentation_level(&zones->blk_info);
    for(k; k < (zones->blk_info.count - OFFSET); k++) {
      sum = 0;
      for(i = k; i < (zones->blk_info.count - OFFSET); i++) {
        sum += x[i]; 
      }
      result = ((total_free_pages - sum) * 100) / total_free_pages;
      printf("page_size: %8d frag_level: %d\n", (1 << k), result);
    }
    zones = zones->next;
  }

just_exit:
  close(fd);
  return 0;
}

char *get_zone_name(char *buf)
{
  int i = 0;
  char *tmp[20] = {0};
  char *input = malloc(strlen(buf) * sizeof(char *));

  strcpy(input, buf);
  tmp[i] = strtok(input, " ");
  while(tmp[i] != NULL) {
    tmp[++i] = strtok(NULL, " ");
  }
  return tmp[3];
}

int *get_zone_blocks(char *input, int *count)
{
  int i = 0;
  int j = 0;
  char *tmp[20] = {0};
  int *blocks = malloc(20 * sizeof(int));

  tmp[i] = strtok(input, " ");
  while(tmp[i] != NULL) {
    blocks[j++] = atoi(tmp[i]);
    tmp[++i] = strtok(NULL, " ");
  }
  *count = i;
  return blocks;
}

int total_free_page(struct blocks_info *blk)
{
  int k = OFFSET;
  int x = 0;
  int free_pages = 0;
  for(k; k < blk->count; k++) {
    free_pages += (1 << x++) * blk->blocks[k];
  }
  return free_pages;
}

int *fragmentation_level(struct blocks_info *blk)
{
  int i = 0;
  int k = OFFSET;
  int *block_x = malloc(sizeof(int) * blk->count);
  for(i; i < (blk->count - OFFSET); i++) {
    block_x[i] = (1 << i) * blk->blocks[k++];
  }
  return block_x;
}
