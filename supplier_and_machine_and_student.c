#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
#include <unistd.h>
int Number_of_coins,Number_of_Students;
pthread_cond_t Full,Empty;
pthread_mutex_t machine_m;

typedef struct{
    int Capacity;
    int Coins_Inserted;
    int How_Many_Drinks;
    int Collected_Coins;
}Machine;
typedef struct{
    int id;
    Machine*machine;
}Student;
typedef struct{
    int Collected_Coins;
    Machine*machine;
}Supplier;

void* student(void*argument) {
    Student *student = (Student*)argument;
    Machine*machine_using = student->machine; 
   
        
        printf("energy: [%d/%d drinks, %d coins, %d inserted] student %d requires an energy drink\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted, student->id);
        if (machine_using->How_Many_Drinks == 0)
        {
            printf("energy: [%d/%d drinks, %d coins, %d inserted] student %d waiting for machine to be refilled\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted, student->id);
        }
        
        pthread_mutex_lock(&machine_m);
        if(machine_using->How_Many_Drinks == 0){
        
        pthread_cond_signal(&Empty);
        pthread_cond_wait(&Full,&machine_m);
        }
        
        
        
        printf("energy: [%d/%d drinks, %d coins, %d inserted] machine waiting for more coins\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted);
        
        printf("energy: [%d/%d drinks, %d coins, %d inserted] student %d is next to be served\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted, student->id);
        for (int i = 0; i < 5; i++)
        {
            machine_using->Coins_Inserted++;
            printf("energy: [%d/%d drinks, %d coins, %d inserted] student %d inserted another coin\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted, student->id);
            sleep(1);
        }
        printf("energy: [%d/%d drinks, %d coins, %d inserted] student %d waiting for drink to arrive\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted, student->id);

        machine_using->How_Many_Drinks--;
        machine_using->Collected_Coins += machine_using->Coins_Inserted;
        machine_using->Coins_Inserted = 0;
        printf("energy: [%d/%d drinks, %d coins, %d inserted] machine despensing drink\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted); 
        printf("energy: [%d/%d drinks, %d coins, %d inserted] machine waiting for pickup of drink\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted);
        
        sleep(1);
        
        
        printf("energy: [%d/%d drinks, %d coins, %d inserted] student %d picked up a drink\n",machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted, student->id);
        printf("energy: [%d/%d drinks, %d coins, %d inserted] student %d enjoying an energy drink\n",
               machine_using->How_Many_Drinks, machine_using->Capacity, machine_using->Collected_Coins, machine_using->Coins_Inserted, student->id);
        pthread_mutex_unlock(&machine_m); 
        sleep(3);
    return NULL; 
} 
void* supplier(void*argument) {
    Machine*machine_our = (Machine*)argument; 
    while(1){
        pthread_mutex_lock(&machine_m);
        pthread_cond_wait(&Empty,&machine_m);
        
        printf("energy: [%d/%d drinks, %d coins, %d inserted] supplier arriving\n",machine_our->How_Many_Drinks,machine_our->Capacity,machine_our->Collected_Coins,machine_our->Coins_Inserted);
        printf("energy: [%d/%d drinks, %d coins, %d inserted] supplier loading %d drinks\n",machine_our->How_Many_Drinks,machine_our->Capacity,machine_our->Collected_Coins,machine_our->Coins_Inserted,machine_our->Capacity);
        machine_our->How_Many_Drinks = machine_our->Capacity;
        
        Number_of_coins += machine_our->Collected_Coins;
        printf("energy: [%d/%d drinks, %d coins, %d inserted] supplier collected %d coins\n",machine_our->How_Many_Drinks,machine_our->Capacity,machine_our->Collected_Coins,machine_our->Coins_Inserted,machine_our->Collected_Coins);
        machine_our->Collected_Coins = 0;
        sleep(1);
        printf("energy: [%d/%d drinks, %d coins, %d inserted] supplier leaving\n",machine_our->How_Many_Drinks,machine_our->Capacity,machine_our->Collected_Coins,machine_our->Coins_Inserted);
        
        pthread_cond_broadcast(&Full);
        pthread_mutex_unlock(&machine_m);
    }
    return NULL; 
}
int main(int argc,char* argv[]){
    
    int opt;
    int capacity = 4;
    Number_of_Students = 2;
    while ((opt = getopt(argc, argv, "n:c:")) != -1) {
        switch (opt) {
            case 'n':
                Number_of_Students = atoi(optarg);
                break;
            case 'c':
                capacity = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-n num_students] [-c capacity]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    pthread_cond_init(&Full,NULL);
    pthread_cond_init(&Empty,NULL);
    pthread_mutex_init(&machine_m,NULL);
    Machine machine= {
        .Capacity = capacity,
        .Coins_Inserted = 0,
        .Collected_Coins = 0,
        .How_Many_Drinks = 0
    };

    pthread_t students[Number_of_Students];
    Student student_data[Number_of_Students];
    pthread_t supplier_t;
    if(pthread_create(&supplier_t, NULL, supplier, &machine) != 0){
        fprintf(stderr, "Problem with thread\n");
        exit(EXIT_FAILURE);
    };
    for (int i = 0; i < Number_of_Students; i++) {
        student_data[i].id = i;
        student_data[i].machine = &machine;
        if(pthread_create(&students[i], NULL, student, &student_data[i])!=0){
            fprintf(stderr,"Problem with threads\n");
            exit(EXIT_FAILURE);
        };
        printf("energy: [%d/%d drinks, %d coins, %d inserted] student %d established\n",machine.How_Many_Drinks, machine.Capacity, machine.Collected_Coins, machine.Coins_Inserted, i);
    }
    
    printf("energy: [%d/%d drinks, %d coins, %d inserted] supplier established\n",machine.How_Many_Drinks, machine.Capacity, machine.Collected_Coins, machine.Coins_Inserted);
    printf("energy: [%d/%d drinks, %d coins, %d inserted] machine booting up\n",machine.How_Many_Drinks, machine.Capacity, machine.Collected_Coins, machine.Coins_Inserted);
    pthread_join(supplier_t, NULL);
    for (int i = 0; i < Number_of_Students; i++) {
        
        pthread_join(students[i], NULL);
        sleep(5);
    }
    pthread_cond_destroy(&Full);
    pthread_cond_destroy(&Empty);
    pthread_mutex_destroy(&machine_m);
    return 0;
}