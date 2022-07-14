sndsize = 16384;
setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&sndsize,
(int)sizeof(sndsize));
rcvsize = 16384;
setsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char *)&rcvsize,
(int)sizeof(rcvsize));

writeiovall(int fd, struct iov *iov, int nvec) {

    int i, bytes;

    i = 0;
    while (i < nvec) {
        do
        {
            rv = writev(fd, &vec[i], nvec - i);
        } while (rv == -1 &&
                 (errno == EINTR || errno == EAGAIN));

        if (rv == -1) {
            if (errno != EINTR && errno != EAGAIN) {
                perror("write");
            }
            return -1;
        }
        bytes += rv;
        /* recalculate vec to deal with partial writes */
        while (rv > 0) {
            if (rv < vec[i].iov_len) {
                vec[i].iov_base = (char *)
                                          vec[i].iov_base + rv;
                vec[i].iov_len -= rv;
                rv = 0;
            }
            else {
                rv -= vec[i].iov_len;
                ++i;
            }
        }
    }

    /* We should get here only after we write out everything */

    return 0;

}