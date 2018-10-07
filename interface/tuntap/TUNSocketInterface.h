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
#ifndef TUNSocketInterface_H
#define TUNSocketInterface_H

#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/TUNSocketInterface_" + builder.config.systemName + ".c");

/**
 * Create a new TUNSocketInterface.
 *
 * @param socket UNIX socket to connect with a TCP/IP stack.
 * @param base the libevent event base to use for listening for incoming packet events.
 * @param logger for logging messages about the tun device.
 * @param eh if this function fails, it will raise one an error.
 * @param allocator a means of getting memory.
 * @return a Interface.
 */
struct Iface* TUNSocketInterface_new(const char* socket,
                                    struct EventBase* base,
                                    struct Log* logger,
                                    struct Except* eh,
                                    struct Allocator* alloc);
#endif
