/**
 * (c) 2014 Nucleus project. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include "sys_fs.h"
#include "nucleus/filesystem/filesystem.h"
#include "nucleus/syscalls/lv2.h"
#include "nucleus/emulator.h"

#include <cstring>

// SysCalls
s32 sys_fs_open(const s8* path, s32 flags, be_t<s32>* fd, u64 mode, const void* arg, u64 size)
{
    FileSystem* fs = getFilesystem(path);
    OpenMode openMode;

    // Create file
    if (flags & CELL_FS_O_CREAT) {
        fs->createFile(path);
    }

    // Access mode
    switch (flags & CELL_FS_O_ACCMODE) {
    case CELL_FS_O_RDONLY:
        openMode = Read;
        break;

    case CELL_FS_O_WRONLY:
        if (flags & CELL_FS_O_APPEND) {
            openMode = WriteAppend;
        }
        else if (flags & CELL_FS_O_EXCL) {
            openMode = WriteExcl;
        }
        else if (flags & CELL_FS_O_TRUNC) {
            openMode = Write;
        }
        else {
            openMode = Write;
        }
        break;

    case CELL_FS_O_RDWR:
        if (flags & CELL_FS_O_TRUNC) {
            fs->createFile(path);
        }
        openMode = ReadWrite;
    }

    if (!fs->existsFile(path)) {
        return CELL_ENOENT;
    }

    auto* file = new sys_fs_t();
    file->type = CELL_FS_S_IFREG;
    file->path = path;
    file->file = fs->openFile(path, openMode);
    file->fs = fs;

    *fd = nucleus.lv2.objects.add(file, SYS_FS_FD_OBJECT);
    return CELL_OK;
}

s32 sys_fs_read(s32 fd, void* buf, u64 nbytes, be_t<u64>* nread)
{
    auto* file = nucleus.lv2.objects.get<sys_fs_t>(fd);
    FileSystem* fs = file->fs;

    *nread = fs->readFile(file->file, buf, nbytes);
    return CELL_OK;
}

s32 sys_fs_write(s32 fd, const void* buf, u64 nbytes, be_t<u64>* nwrite)
{
    auto* file = nucleus.lv2.objects.get<sys_fs_t>(fd);
    FileSystem* fs = file->fs;

    *nwrite = fs->writeFile(file->file, buf, nbytes);
    return CELL_OK;
}

s32 sys_fs_close(s32 fd)
{
    auto* file = nucleus.lv2.objects.get<sys_fs_t>(fd);
    FileSystem* fs = file->fs;

    fs->closeFile(file->file);
    return CELL_OK;
}
