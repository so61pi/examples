#include <iostream>

#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


void SigChldHandler(int /*signo*/, siginfo_t* /*sinfo*/, void* /*context*/) {
    pid_t proc;
    while ((proc = waitpid(-1, nullptr, WNOHANG)) > 0) {
        std::cout << "child process id " << proc << " just passed away\n";
    }
}


int main() {
    std::cout << "parent process started" << std::endl;

    struct sigaction act = {};
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = SigChldHandler;
    act.sa_flags     = SA_SIGINFO;
    if (sigaction(SIGCHLD, &act, nullptr) < 0) {
        std::cerr << "sigaction error code = " << errno << "\n"
                  << "exits now\n";
        return 1;
    }

    switch (fork()) {
    case -1:
        std::cerr << "error code = " << errno << "\n"
                  << "exits now\n";
        return 1;

    case 0:
        std::cout << "child process started" << std::endl;
        std::cout << "child process id is " << getpid() << std::endl;
        std::cout << "child process ends now" << std::endl;
        return 0;

    default:
        std::cout << "child process is created successfully\n";
        break;
    }

    // sleep returns immediately if a signal occurs
    sleep(10);

    std::cout << "parent process ends now" << std::endl;
    return 0;
}
