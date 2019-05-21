#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define ROUNDS 100

pthread_mutex_t lock;

int burger = 0;
int fries = 0;
int shake = 0;

int burger_meal_consumed = 0;
int fries_meal_consumed = 0;
int shake_meal_consumed = 0;

int burger_customer_took_item = 0;
int fries_customer_took_item = 0;
int shake_customer_took_item = 0;

int customer_queue = 0;

int rounds = 0;
int chef_active = 1;
int customer_active = 0;

void* chef(void* arg) {
	while(rounds < ROUNDS) {
		pthread_mutex_lock(&lock);
		if(chef_active == 1) {
			int random_num = rand() % 3 + 1;
			if(random_num == 1) {
				burger = 0;
				fries = 1;
				shake = 1;
			} else if(random_num == 2) {
				burger = 1;
				fries = 0;
				shake = 1;
			} else if(random_num == 3) {
				burger = 1;
				fries = 1;
				shake = 0;
			} else {
				burger = 0;
				fries = 0;
				shake = 0;
			}
			chef_active = 0;
			customer_active = 1;
			burger_customer_took_item = 0;
			fries_customer_took_item = 0;
			shake_customer_took_item = 0;
			customer_queue = 0;

			rounds++;
		}
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

void* burger_customer(void* arg) {
	int fries_consumed = 0;
	int shake_consumed = 0;

	while(rounds < ROUNDS) {
		pthread_mutex_lock(&lock);
		if((fries == 1) && (burger_customer_took_item == 0)) {
			fries_consumed++;
			fries = 0;
			burger_customer_took_item = 1;
			customer_queue++;
		} else if((shake == 1) && (burger_customer_took_item == 0)) {
			shake_consumed++;
			shake = 0;
			burger_customer_took_item = 1;
			customer_queue++;
		}

		if((customer_active == 1) && (customer_queue == 2)) {
			chef_active = 1;
			customer_active = 0;
		}

		if((fries_consumed >= 1) && (shake_consumed >= 1)) {
			burger_meal_consumed++;
			fries_consumed--;
			shake_consumed--;
		}
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

void* fries_customer(void* arg) {
	int burger_consumed = 0;
	int shake_consumed = 0;

	while(rounds < ROUNDS) {
		pthread_mutex_lock(&lock);
		if((burger == 1) && (fries_customer_took_item == 0)) {
			burger_consumed++;
			burger = 0;
			fries_customer_took_item = 1;
			customer_queue++;
		} else if((shake == 1) && (fries_customer_took_item == 0)) {
			shake_consumed++;
			shake = 0;
			fries_customer_took_item = 1;
			customer_queue++;
		}

		if((customer_active == 1) && (customer_queue == 2)) {
			chef_active = 1;
			customer_active = 0;
		}

		if((burger_consumed >= 1) && (shake_consumed >= 1)) {
			fries_meal_consumed++;
			burger_consumed--;
			shake_consumed--;
		}
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

void* shake_customer(void* arg) {
	int burger_consumed = 0;
	int fries_consumed = 0;

	while(rounds < ROUNDS) {
		pthread_mutex_lock(&lock);
		if((burger == 1) && (shake_customer_took_item == 0)) {
			burger_consumed++;
			burger = 0;
			shake_customer_took_item = 1;
			customer_queue++;
		} else if((fries == 1) && (shake_customer_took_item == 0)) {
			fries_consumed++;
			fries = 0;
			shake_customer_took_item = 1;
			customer_queue++;
		}

		if((customer_active == 1) && (customer_queue == 2)) {
			chef_active = 1;
			customer_active = 0;
		}

		if((burger_consumed >= 1) && (fries_consumed >= 1)) {
			shake_meal_consumed++;
			burger_consumed--;
			fries_consumed--;
		}
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);
}

int main() {

	pthread_t chef_t;
	pthread_t burger_t;
	pthread_t fries_t;
	pthread_t shake_t;

	pthread_create(&chef_t, NULL, chef, NULL);
	pthread_create(&burger_t, NULL, burger_customer, NULL);
	pthread_create(&fries_t, NULL, fries_customer, NULL);
	pthread_create(&shake_t, NULL, shake_customer, NULL);

	pthread_join(chef_t, NULL);
	pthread_join(burger_t, NULL);
	pthread_join(fries_t, NULL);
	pthread_join(shake_t, NULL);

	

	printf("Customer #1 ate %d times.\nCustomer #2 ate %d times.\nCustomer #3 ate %d times.\n", burger_meal_consumed, fries_meal_consumed, shake_meal_consumed);
	
	return 0;
}
