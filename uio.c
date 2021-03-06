#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

size_t
writeuio(int fd, struct iovec *iov, int cnt)
{
    size_t pos = 0;
    ssize_t res;
    n = iov[0].iov_cnt;
    while (n > pos) {
        poll_wait(fd, POLLOUT | POLLERR);
        res = writev (fd, iov[0].iov_base + pos, n - pos);
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