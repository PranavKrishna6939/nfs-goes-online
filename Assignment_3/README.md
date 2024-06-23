# Distributed File System

This project implements a working distributed file server using UDP communication. The server stores all its data in an on-disk, fixed-sized file referred to as the file system image.

## Current Implementation Status

We have implemented the following functions in `myfilesystem.h`:

- `openFs()`: Opens the file system image.
- `findInode()`: Locates and reads an inode from the file system image.
- `MFS_Lookup()`: Looks up an entry in a directory.
- `MFS_Stat()`: Retrieves information about a file or directory.
- `MFS_Write()`: Writes data to a file (partially implemented).

## File Server

The file server is a stand-alone UDP-based server that processes incoming messages and replies to clients. It uses a file system image that contains on-disk representations of data structures.

### On-Disk File System Structure

The file system follows a simple Unix-like structure:

- Super block (4KB)
- Inode bitmap
- Data bitmap
- Inode table
- Data region

Detailed structures are defined in `ufs.h`.

## Client Library

The client library (`libmfs.so`) provides an interface for programs to access the file server. It implements the following functions:

- `MFS_Init(char *hostname, int port)`
- `MFS_Lookup(int pinum, char *name)`
- `MFS_Stat(int inum, MFS_Stat_t *m)`
- `MFS_Write(int inum, char *buffer, int offset, int nbytes)`
- `MFS_Read(int inum, char *buffer, int offset, int nbytes)`
- `MFS_Creat(int pinum, int type, char *name)`
- `MFS_Unlink(int pinum, char *name)`
- `MFS_Shutdown()`

## Server Idempotency

The server implements idempotency by committing all changes to the file system image using `fsync()` before returning success codes.

## Usage

To start the server:

```
prompt> server [portnum] [file-system-image]
```

## TODO

- Complete the implementation of `MFS_Write()`
- Implement remaining client library functions
- Add error handling and timeout mechanisms
- Implement `select()` for client-side timeouts

## References

- [File System Implementation](https://pages.cs.wisc.edu/~remzi/OSTEP/file-implementation.pdf)
- [Distributed Systems](https://pages.cs.wisc.edu/~remzi/OSTEP/dist-intro.pdf)
- [Distributed File System: NFS](https://pages.cs.wisc.edu/~remzi/OSTEP/dist-nfs.pdf)

## Helper Code

- UDP communication code: [OSTEP Code Repository](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/dist-intro)
- Header files and utilities:
  - [mfs.h](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/filesystems-distributed-ufs/mfs.h)
  - [ufs.h](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/filesystems-distributed-ufs/ufs.h)
  - [mkfs.c](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/filesystems-distributed-ufs/mkfs.c)

