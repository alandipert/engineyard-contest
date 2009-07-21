#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

unsigned char test_phrase_hash[SHA_DIGEST_LENGTH] = {0};
char *test_phrase = "I would much rather hear more about your whittling project";

void pp(unsigned char *md) {
  int i;
  for(i = 0; i < 20; i++) {
    printf("%02x", md[i]);
  }
  printf("\n");
}

unsigned hamdist(unsigned x, unsigned y) {
  unsigned dist = 0, val = x ^ y;
  while(val) { 
    ++dist; val &= val - 1;
  }
  return dist;
}

int distance(unsigned char *md1, unsigned char *md2) {
  int i;
  int distance = 0;
  for(i = 0; i < 20; i++) {
    distance += hamdist(md1[i], md2[i]);
  }
  return distance;
}

int main(int argc, char **argv) {
  if(argc < 2) {
    printf("specify a string to test\n");
    return 1;
  }

  unsigned char md[SHA_DIGEST_LENGTH] = {0};
  SHA1((unsigned char *)argv[1], strlen(argv[1]), md);
  SHA1((unsigned char *)test_phrase, strlen(test_phrase), &test_phrase_hash[0]);

  printf("your hash: ");
  pp(md);
  printf("their hash: ");
  pp(test_phrase_hash);
  printf("hamming distance: ");
  printf("%i\n", distance(md, test_phrase_hash));

  return 0;
}
