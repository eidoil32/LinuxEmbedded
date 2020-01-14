#include "launcher.h"

int main(int argc, char* argv[]) {
    int argv_miners = argc > 1 ? atoi(argv[1]) : 0; // checking if any argument is passed to the launcher
    int num_of_miners = (argv_miners == 0 ? DEFAULT_NUMBER_OF_MINERS : argv_miners); // checking if an valid number is passed

    program_start(num_of_miners);

    return 0;
}

void program_start(int num_of_miners) {
    if (loadUpServer() < 0) {
        fprintf(stderr, LOAD_UP_FAILD, SERVER_STRING);
        return;
    }
    
    if (loadUpMiners(num_of_miners) < 0) {
        fprintf(stderr, LOAD_UP_FAILD, MINER_STRING);
        return;
    }

    for (size_t i = 0; i < num_of_miners + 1; ++i) { // +1 is the server proccess
        wait(NULL);
    }
}

int loadUpMiners(int num_of_miners) {
    for (size_t i = 0; i < num_of_miners; ++i) {
        char *argv[3] = { MINER_EXECVE_PROGRAM_NAME, itoa(i + 1),  NULL };
        pid_t miner_pid = fork();
        switch (miner_pid) {
            case -1:    // fork() faild..
                fprintf(stderr, FORK_FAILED, MINER_STRING);
                return -1;
            case 0:     // fork() successeded, now we can launch miner application
                execve(SERVER_EXECVE_PROGRAM_NAME, argv, NULL);
                fprintf(stderr, EXECVE_FAILED, MINER_STRING);
                return -1;
            default:    // launcher proccess, no need to do anything..
                break;
        }
    }

    return 0;
}

int loadUpServer() {
    char *argv[2] = { SERVER_EXECVE_PROGRAM_NAME, NULL };
    pid_t server_pid = fork();
    switch (server_pid) {
        case -1:    // fork() faild..
            fprintf(stderr, FORK_FAILED, SERVER_STRING);
            return -1;
        case 0:     // fork() successeded, now we can launch miner application
            execve(SERVER_EXECVE_PROGRAM_NAME, argv, NULL);
            fprintf(stderr, EXECVE_FAILED, SERVER_STRING);
            return -1;
        default:    // launcher proccess, no need to do anything..
            return 0;
    }
}