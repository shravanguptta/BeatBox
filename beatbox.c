#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdbool.h>
#include <pthread.h>
#include "audioMixer_template.h"
#include "utilities.h"
#include "joystick.h"

#define THREAD_NUM 1
#define MAX_TEMPO 300
#define MIN_TEMPO 40

#define MAX_VOLUMN 100
#define MIN_VOLUMN 0

static pthread_t tids[THREAD_NUM];
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static bool isRunning = true;
static int mode = 2;
static int tempo = 120;


void changeTempo(bool value) {
	if(value) {
		tempo = tempo + 5 >= MAX_TEMPO ? MAX_TEMPO : tempo + 5;
	} else {
		tempo = tempo - 5 <= MIN_TEMPO ? MIN_TEMPO : tempo - 5;
	}
}

void changeVolumn(bool value) {
	int newVolumn = AudioMixer_getVolume();
	if(value) {
		newVolumn = newVolumn + 5 >= MAX_VOLUMN ? MAX_VOLUMN : newVolumn + 5;
	} else {
		newVolumn = newVolumn - 5 <= MIN_VOLUMN ? MIN_VOLUMN : newVolumn - 5;
	}
	AudioMixer_setVolume(newVolumn);
}

void initAudioFiles() {
    //Add more as needed
	//AudioMixer_readWaveFileIntoMemory();
}

void removeAudioFiles() {
	//AudioMixer_freeWaveFileData();
}

static void *selectBeat() 
{
	initAudioFiles();

	while(isRunning) {
		if(mode == 1) {
			continue;
		} else if(mode == 2) {
			//standardRockBeat();
		} else {
			//customDrumBeat();
		}
	}

	removeAudioFiles();
	return NULL;
}

static void *updateTempoAndVolumn() 
{
	while(isRunning) {
        char joystickDir = Joystick_position(); 
		if(joystickDir == 'U') {
			pthread_mutex_lock(&mutex);
			{
                changeVolumn(true);
			}
			pthread_mutex_unlock(&mutex);
		} else if (joystickDir == 'D') {
			pthread_mutex_lock(&mutex);
			{
				changeVolumn(false);
			}
			pthread_mutex_unlock(&mutex);
		} else if(joystickDir == 'R'){
			pthread_mutex_lock(&mutex);
			{
				changeTempo(true);
			}
			pthread_mutex_unlock(&mutex);
		} else if(joystickDir == 'L') {
			pthread_mutex_lock(&mutex);
			{
				changeTempo(false);
			}
			pthread_mutex_unlock(&mutex);
		} else if(joystickDir == 'C') {
			pthread_mutex_lock(&mutex);
			{
				
			}
			pthread_mutex_unlock(&mutex);
		} else {
			continue;
		}
		sleepForMs(500);
	}
	return NULL;
}

int main()
{
    printf("Starting application \n");

    printf("Enter 'Q' to quit.\n");
	/*
    while (true) {
        // Quit?
        if (toupper(getchar()) == 'Q') {
			isRunning = false;
            break;
        }
    }
	*/
	AudioMixer_init();
	pthread_create(&tids[0], NULL, selectBeat, NULL);
	pthread_create(&tids[1], NULL, updateTempoAndVolumn, NULL);
	
	for(int i = 0; i < THREAD_NUM; i++) {
		pthread_join(tids[i], NULL);
	}
    printf("Ending application \n");
}