// Samuel Kuchta (xkucht11), 2. Projekt IPS 2022/23

#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <string.h>

using namespace std;

vector<mutex*> output_mutex, work_mutex;
mutex line_mutex, score_mutex;

int total_score = 0;
char* line = NULL;
int num_threads;

char* read_line(int* res) {  // function reads first line from input pipeline
	string line;
	char* str;
	if (getline(cin, line)) {
		if ((str = (char*)malloc(sizeof(char) * (line.length() + 1))) != NULL) {
			strcpy(str, line.c_str());
			*res = 1;
			return str;
		}
	}
	*res = 0;
	return NULL;
}

void automat(int ID, string sub_str, int score) {  // funkcia vlakna. stavovy automat pre najdenie podretazca v riadku.
  	// printf("Thread %i started, sub_str: %s\n", ID, sub_str);
	line_mutex.lock(); // threads needs to wait for first line
	line_mutex.unlock(); // if line was read, threads need to unlock the mutex so no deadlock occures

  int k = 0;
	int state = 0;
	int res = -1;

	while(line != NULL) {
    int line_len = strlen(line);
		k = 0;
		res = -1;
    if (line_len >= 5) {
      while(res < 0 && k < line_len - 4) {
        while(state < 6) {
          switch(state) {
            case 0: case 1: case 2: case 3: case 4:
              if (line[k + state] == sub_str[state]) {
                state++;
              } else {
                state = 6;
              }
              break;
            case 5:
              res = 1;
              state++;
              break;
            
            default:
              printf("\n\nERROR: default in switch.\n\n");
              exit(1);
          }
        }
        k++;
        state = 0;
      }
      if (res == 1) {
        score_mutex.lock();
        total_score += score;
        score_mutex.unlock();
      };
    }
	  	// printf("Thread %i res: %d score: %d  total_score: %d\n", ID, res, score, total_score);
		work_mutex[ID]->unlock();
		output_mutex[ID]->lock();  // caka na odomknutie hlavnym procesom
		if (ID != (num_threads - 1)) {  // odomkne ostatne vlakne navzajom.
			output_mutex[ID + 1]->unlock();
		}
	}
}

int main(int argc, char* argv[]) {
	vector <thread*> threads;  // pole vlaken promenne velikosti

	if (argc % 2 != 0) {
		printf("\n\nnespravny pocet argumetu\n\n");
		exit(0);
	}

  int score_threshold = atoi(argv[1]);
	num_threads = (argc - 2) / 2;

	output_mutex.resize(num_threads); // all thread need their own priting and work_mutex mutexes
	work_mutex.resize(num_threads);
	threads.resize(num_threads);

	line_mutex.lock();  // vlakna cakaju na vstup

	for (int i = 0; i < num_threads; i++) {  // vytvorime vlakna a zamky_vlakien
		output_mutex[i] = new mutex();
		output_mutex[i]->lock();

		work_mutex[i] = new mutex();
		work_mutex[i]->lock();

    if ((threads[i] = new thread (automat, i, argv[(i*2)+2], atoi(argv[(i*2)+3]))) == NULL) {
			fprintf(stderr, "ERROR: unable to create thread.\n");
			exit(1);
		}
	}

	int res;
	line = read_line(&res);
	
	line_mutex.unlock();  // vstup nacitany, odomkneme vlakna

	while (res) {  // dokym existuje novy riadok(vstup)
		for(int i = 0; i < num_threads; i++) {
			work_mutex[i]->lock(); // hlavny proces caka na vlakna
		}

    if (total_score >= score_threshold) {
      printf("%s\n", line);
    }
    // printf("main_total_score: %d\n", total_score);
    total_score = 0;

    free(line);  // uvolnim pamet
		line = read_line(&res);
		output_mutex[0]->unlock();  // odomkne prve vlakno, ktore odomkne dalsie atd..
	}

  for (int i = 0; i < num_threads; i++) {  // provedeme join a uvolnime pamet threads
		(*(threads[i])).join();
		delete threads[i];
		delete output_mutex[i];
    delete work_mutex[i];
	}	
}
