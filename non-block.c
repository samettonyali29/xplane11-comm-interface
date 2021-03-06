/* set socket non blocking */
fl = fcntl(accsock, F_GETFL);
fcntl(accsock, F_SETFL, fl | O_NONBLOCK);


void
poll_wait(int fd, int events)
{
    int n;
    struct pollfd pollfds[1];
    memset((char *) &pollfds, 0, sizeof(pollfds));

    pollfds[0].fd = fd;
    pollfds[0].events = events;

    n = poll(pollfds, 1, -1);
    if (n < 0) {
        perror("poll()");
        errx(1, "Poll failed");
    }
}

size_t
readmore(int sock, char *buf, size_t n) {

    fd_set rfds;
    int ret, bytes;



    poll_wait(sock,POLLERR | POLLIN );
    bytes = readall(sock, buf, n);

    if (0 == bytes) {
        perror("Connection closed");
        errx(1, "Readmore Connection closure");
        /* NOT REACHED */
    }

    return bytes;
}

size_t
readall(int sock, char *buf, size_t n) {
    size_t pos = 0;
    ssize_t res;

    while (n > pos) {
        res = read (sock, buf + pos, n - pos);
        switch ((int)res) {
            case -1:
                if (errno == EINTR || errno == EAGAIN)
                    continue;
                return 0;
            case 0:
                errno = EPIPE;
                return pos;
            default:
                pos += (size_t)res;
        }
    }
    return (pos);
}


size_t
writenw(int fd, char *buf, size_t n)
{
    size_t pos = 0;
    ssize_t res;
    while (n > pos) {
        poll_wait(fd, POLLOUT | POLLERR);
        res = write (fd, buf + pos, n - pos);
        switch ((int)res) {
            case -1:
                if (errno == EINTR || errno == EAGAIN)
                    continue;
                return 0;
            case 0:
                errno = EPIPE;
                return pos;
            default:
                pos += (size_t)res;
        }
    }
    return (pos);

}