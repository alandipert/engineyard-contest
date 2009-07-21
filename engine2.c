#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define NUM_WORDS 12
#define BUF_MAX 161

typedef unsigned char sha1_hash;

char *top_string[BUF_MAX];
int top_distance;

unsigned char challenge_hash[SHA_DIGEST_LENGTH];
char *challenge_string = "I would much rather hear more about your whittling project";

char *dict[1000];
int dict_word_sizes[1000];

FILE *random_file;

void init_dict(char *file) {
  FILE *f = fopen(file, "r");
  char tmp[30];
  int i = 0;
  while(fscanf(f, "%s\n", tmp) != EOF) {
    dict[i] = strdup(tmp);
    dict_word_sizes[i] = strlen(tmp);
    i++;
  }
  fclose(f);
}

void init_random() {
  random_file = fopen("/dev/urandom", "r");
}

unsigned hamdist(unsigned x, unsigned y) {
  unsigned dist = 0, val = x ^ y;
  while(val) { 
    ++dist; val &= val - 1;
  }
  return dist;
}

//fills an array w/ rand numbers between 0-1000 inclusive
//these are the indices of 12 words in dict
void get_nums(int *nums_arr, int size) {
  int i;
  unsigned int num;
  for(i = 0; i < size; i++) {
    fread(&num, 2, 1, random_file);
    nums_arr[i] = num % 1000;
  }
}

int distance(sha1_hash *md1, sha1_hash *md2) {
  int i;
  int distance = 0;
  for(i = 0; i < SHA_DIGEST_LENGTH; i++) {
    distance += hamdist(md1[i], md2[i]);
  }
  return distance;
}

//also returns the lenght of the built string
int build_string(char *buf, int nums[], int count) {
  int i, start;
  start = 0;
  for(i = 0; i < count-1; i++) {
    memcpy(&(buf[start]), dict[nums[i]], dict_word_sizes[nums[i]]);
    buf[start+dict_word_sizes[nums[i]]] = ' ';
    start += dict_word_sizes[nums[i]]+1;
  }
  memcpy(&(buf[start]), dict[nums[count-1]], dict_word_sizes[nums[count-1]]);
  buf[start+dict_word_sizes[nums[count-1]]] = '\0';
  return start+dict_word_sizes[nums[count-1]];
}

int main(void) {

  //compute the challenge hash
  SHA1((unsigned char *)challenge_string, strlen(challenge_string), &challenge_hash[0]);

  //load words into the dict
  init_dict("words.txt");

  //get ready to read /dev/urandom
  init_random();

  //init 'best'
  top_distance = 5000;

  //compute!
  int nums[NUM_WORDS], i, len, dist;
  char buf[BUF_MAX];
  sha1_hash tmp_hash[SHA_DIGEST_LENGTH];

  while(1) {
    get_nums(nums,NUM_WORDS);
    len = build_string(buf, nums, NUM_WORDS);
    SHA1((unsigned char*)buf, len, tmp_hash);
    dist = distance(challenge_hash, tmp_hash);
    if(dist < top_distance) {
      //we have a winnar (maybe)
      memcpy(top_string, buf, len+1);
      top_distance = dist;
      printf("%i\t\"%s\"\n", top_distance, top_string);
      fflush(stdout);
    }
  }

  return 0;
}
