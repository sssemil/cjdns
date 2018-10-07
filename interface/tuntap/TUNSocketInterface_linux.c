/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "interface/tuntap/TUNSocketInterface.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"

#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <string.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>
#include <net/if.h>

#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>

struct Iface* TUNSocketInterface_new(const char* socket_path,
                                    struct EventBase* base,
                                    struct Log* logger,
                                    struct Except* eh,
                                    struct Allocator* alloc)
{
    Log_info(logger, "Initializing socket %s;", socket_path);

    struct sockaddr_un addr;
    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        Except_throw(eh, "socket(\"%s\") [%s]", socket_path, strerror(errno));
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if (*socket_path == '\0')
    {
        *addr.sun_path = '\0';
        strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
    }
    else
    {
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        Except_throw(eh, "socket(\"%s\") connect error [%s]", socket_path, strerror(errno));
    }

    if (fd < 0)
    {
        Except_throw(eh, "Could not open socket! (\"%s\") [%s]", socket_path, strerror(errno));
    }

    struct Pipe* p = Pipe_forFiles(fd, fd, base, eh, alloc);

    return &p->iface;
}
