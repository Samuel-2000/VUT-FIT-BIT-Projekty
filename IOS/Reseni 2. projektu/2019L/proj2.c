// IOS Projekt 2 Semafory Faneuil Hall Problem
// Samuel Kuchta 2019/2020 (xkucht11)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>


#define Semafor1_NAME "/Sudca_sem"
#define Semafor2_NAME "/Mutex_sem"
#define Semafor3_NAME "/Confirmed_sem"
#define Semafor4_NAME "/VsetciPrihlaseny_sem"
#define Semafor5_NAME "/inkrement_sem"

static void wait_random_time(int max) {
  if (max > 0) {
    usleep(random() % max * 1000);
  }
}

typedef sem_t Semafor;

int sem_signal(Semafor * sem) {
  return sem_post(sem);
}

// Pocet imigrantov, doba na imigranta, doba na soudce, doba na certifikat, doba na rozhodnuti
int main(int argc, char * argv[]) {
  (void) argc;
    // prevent buffering of the output
  setbuf(stdout, NULL);
  // remove Semafors
  sem_unlink(Semafor1_NAME);
  sem_unlink(Semafor2_NAME);
  sem_unlink(Semafor3_NAME);
  sem_unlink(Semafor4_NAME);
  sem_unlink(Semafor5_NAME);

  if (argc <= 5) {
    printf("zle");
    return (EXIT_FAILURE);
  }

  int * checked_nerozhodnuto = NULL;
  int * vevnitr = NULL;
  int * vevnitr_nerozhodnuto = NULL;
  int * A = NULL;
  int * pocet_zbyva = NULL;

  int checked_nerozhodnutoId = 0;
  int vevnitrId = 0;
  int vevnitr_nerozhodnutoId = 0;
  int AId = 0;
  int pocet_zbyvaId = 0;

  int PI = atoi(argv[1]);
  int IG = atoi(argv[2]);
  int JG = atoi(argv[3]);
  int IT = atoi(argv[4]);
  int JT = atoi(argv[5]);

  int Sudca_vnutri = 0;
  

  if (PI < 0) {
    return (EXIT_FAILURE);
  }
  if (IG < 0 || IG > 2000) {
    return (EXIT_FAILURE);
  }
  if (JG < 0 || JG > 2000) {
    return (EXIT_FAILURE);
  }
  if (IT < 0 || IT > 2000) {
    return (EXIT_FAILURE);
  }
  if (JT < 0 || JT > 2000) {
    return (EXIT_FAILURE);
  }
  Semafor * Sudca = NULL;
  Semafor * Mutex = NULL;
  Semafor * Confirmed = NULL;
  Semafor * VsetciPrihlaseny = NULL;
  Semafor * inkrement = NULL;


  if ((Sudca = sem_open(Semafor1_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
    perror("sem_init failed\n");
    exit(-1);
  }

  if ((Mutex = sem_open(Semafor2_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
    perror("sem_init failed\n");
    exit(-1);
  }
  if ((Confirmed = sem_open(Semafor3_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
    perror("sem_init failed\n");
    exit(-1);
  }
  if ((VsetciPrihlaseny = sem_open(Semafor4_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
    perror("sem_init failed\n");
    exit(-1);
  }
  if ((inkrement = sem_open(Semafor5_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
    perror("sem_init failed\n");
    exit(-1);
  }




  if ((vevnitrId = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
    perror("shmget failed\n");
    exit(-1);
  }
  if ((vevnitr = shmat(vevnitrId, NULL, 0)) == NULL) {
    perror("shmat failed\n");
    exit(-1);
  }

  if ((vevnitr_nerozhodnutoId = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
    perror("shmget failed\n");
    exit(-1);
  }
  if ((vevnitr_nerozhodnuto = shmat(vevnitr_nerozhodnutoId, NULL, 0)) == NULL) {
    perror("shmat failed\n");
    exit(-1);
  }

  if ((checked_nerozhodnutoId = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
    perror("shmget failed\n");
    exit(-1);
  }
  if ((checked_nerozhodnuto = shmat(checked_nerozhodnutoId, NULL, 0)) == NULL) {
    perror("shmat failed\n");
    exit(-1);
  }
    if ((AId = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
    perror("shmget failed\n");
    exit(-1);
  }
  if ((A = shmat(AId, NULL, 0)) == NULL) {
    perror("shmat failed\n");
    exit(-1);
  }

    if ((pocet_zbyvaId = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
    perror("shmget failed\n");
    exit(-1);
  }
  if ((pocet_zbyva = shmat(pocet_zbyvaId, NULL, 0)) == NULL) {
    perror("shmat failed\n");
    exit(-1);
  }

  * pocet_zbyva = PI;
  //(* pocet_zbyva) --;

  pid_t processPid = fork();
  if (processPid == 0) {
    // pomocny
    pid_t processPid = fork();
    if (processPid == 0) {
      //imigrant
      for (int i = 1; i <= PI; i++) {
        pid_t processPid = fork();
        if (processPid == 0) {
          if (IG != 0) {
            wait_random_time(IG);
          }
          // imigranti
          sem_wait(Sudca); //  pokial neni sudca, tak immigranti po jednom vstupuju. Ak niekto nestihne vsupit skor ako vstupi sudca, tak musi pockat na dalsie kolo.
          
          sem_wait(inkrement);
          ( * vevnitr) ++;
          ( * vevnitr_nerozhodnuto) ++;
          sem_signal(inkrement);
                        sem_wait(inkrement);
                                if (i < 10) {
                                    printf("%d\t: IMM %d\t\t: enters\t\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                } else {
                                    printf("%d\t: IMM %d\t: enters\t\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                }
                        sem_signal(inkrement);
          sem_signal(Sudca); //  moze vstupit dalsi imigrant, alebo sudca
          
          sem_wait(Mutex); //  caka na registraciu, alebo na skontrolovanie sudcom
          sem_wait(inkrement);
          ( * checked_nerozhodnuto) ++;
          sem_signal(inkrement);
                        sem_wait(inkrement);
                                if (i < 10) {
                                    printf("%d\t: IMM %d\t\t: checks\t\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                } else {
                                    printf("%d\t: IMM %d\t: checks\t\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                }
                        sem_signal(inkrement);
          if ((Sudca_vnutri == 1) && ( * vevnitr_nerozhodnuto == * checked_nerozhodnuto)) {
            sem_signal(VsetciPrihlaseny);
          }
          sem_signal(Mutex);

          sem_wait(Confirmed); //  caka kym ich sudca naturalizuje
                        sem_wait(inkrement);
                                if (i < 10) {
                                    printf("%d\t: IMM %d\t\t: wants certificate\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                } else {
                                    printf("%d\t: IMM %d\t: wants certificate\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                }
                        sem_signal(inkrement);
          wait_random_time(IT);
                        sem_wait(inkrement);
                                if (i < 10) {
                                    printf("%d\t: IMM %d\t\t: got certificate\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                } else {
                                    printf("%d\t: IMM %d\t: got certificate\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                }
                        sem_signal(inkrement);

          sem_wait(Sudca);
            sem_wait(inkrement);
            ( * vevnitr) --;
            sem_signal(inkrement);
                        sem_wait(inkrement);
                                if (i < 10) {
                                    printf("%d\t: IMM %d\t\t: leaves\t\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                } else {
                                    printf("%d\t: IMM %d\t: leaves\t\t\t: %d\t: %d\t: %d\n", (* A)++, i, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                                }
                        sem_signal(inkrement);
                sem_wait(inkrement);
                (* pocet_zbyva) --;
                sem_signal(inkrement);
          sem_signal(Sudca);

          exit(EXIT_SUCCESS);
        }
      }
    } else if (processPid > 0) {
      //Sudca

label:
      wait_random_time(JG);

                    sem_wait(inkrement);
                        printf("%d\t: JUDGE\t\t: wants to enter\t\t: %d\t: %d\t: %d\n", (* A)++, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                    sem_signal(inkrement);
      sem_wait(Sudca);
      sem_wait(Sudca); //  sudca vstupi, takze uz ziadny imigrant nemoze vstupit
                    sem_wait(inkrement);
                        printf("%d\t: JUDGE\t\t: enters\t\t\t: %d\t: %d\t: %d\n", (* A)++, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                    sem_signal(inkrement);  
      sem_wait(Mutex); //  nikto sa nemoze momentalne registrovat
      Sudca_vnutri = 1;
      if ( * vevnitr_nerozhodnuto > * checked_nerozhodnuto) { //  skontroluje ci su vsetci registrovany
                    sem_wait(inkrement);
                            printf("%d\t: JUDGE\t\t: waits for imm\t\t: %d\t: %d\t: %d\n", (* A)++, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                    sem_signal(inkrement);
        sem_signal(Mutex);
        sem_wait(VsetciPrihlaseny); //  caka kym budu vsetci zaregistrovany
        sem_wait(Mutex);
      }

      // ak su vsetci pritomny zaregistrovany, tak sudca ich naturalizuje
                    sem_wait(inkrement);
                        printf("%d\t: JUDGE\t\t: starts confirmation\t\t: %d\t: %d\t: %d\n", (* A)++, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                    sem_signal(inkrement);
        sem_signal(Mutex);
      wait_random_time(JT);
      for (int j = 0; j < * vevnitr; j++) {
        sem_signal(Confirmed);
      }
      * vevnitr_nerozhodnuto = * checked_nerozhodnuto = 0;

                    sem_wait(inkrement);
                        printf("%d\t: JUDGE\t\t: ends confirmation\t\t: %d\t: %d\t: %d\n", (* A)++, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                    sem_signal(inkrement);

      wait_random_time(JT);
      Sudca_vnutri = 0;
                    sem_wait(inkrement);
                        printf("%d\t: JUDGE\t\t: leaves\t\t\t: %d\t: %d\t: %d\n", (* A)++, * vevnitr_nerozhodnuto, * checked_nerozhodnuto, * vevnitr);
                    sem_signal(inkrement);
      sem_signal(Sudca);
      sem_signal(Sudca);
      //  ak nejaky immigrant caka, tak sudca znova vstupi do budovy, inak sa ukonci.
      if ( * pocet_zbyva > 0) {
          goto label;
      }
      exit(EXIT_SUCCESS);

    } else {
      printf("error failed\n");
      exit(-1);
    }

  } else if (processPid > 0) {
    // hlavny

    sem_signal(Sudca);

    // wait for child to end
    waitpid(processPid, NULL, 0);

    // close Semafors
    sem_close(Sudca);
    sem_close(Mutex);
    sem_close(Confirmed);
    sem_close(VsetciPrihlaseny);
    sem_close(inkrement);

    // remove Semafors
    sem_unlink(Semafor1_NAME);
    sem_unlink(Semafor2_NAME);
    sem_unlink(Semafor3_NAME);
    sem_unlink(Semafor4_NAME);
    sem_unlink(Semafor5_NAME);

    shmctl(checked_nerozhodnutoId, IPC_RMID, NULL);
    shmctl(vevnitrId, IPC_RMID, NULL);
    shmctl(vevnitr_nerozhodnutoId, IPC_RMID, NULL);
    shmctl(AId, IPC_RMID, NULL);
    shmctl(pocet_zbyvaId, IPC_RMID, NULL);
    return EXIT_SUCCESS;

  } else {
    printf("error failed\n");
    exit(-1);
  }
}