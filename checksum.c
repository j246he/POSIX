#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

uint64_t checksum (char * bytes, int length);

int main()
{
    char data[4*1024*1024];

    FILE * file = fopen ("data.bin", "r");
    if (file == NULL)
    {
        perror ("Could not open file\n");
        return 1;
    }

    fread(data, 1, sizeof(data), file);

    uint64_t chksum = 0;

    printf ("Checksum: %" PRIx64 "\n", checksum(data, sizeof(data)));
}

uint64_t checksum (char * bytes, int length)
{
    uint64_t chksum = 0;

    int i, blk;
    for (blk = 0; blk < 4; ++blk)
    {
        uint64_t blksum = 0;
        uint64_t * data = (uint64_t *)(bytes + blk*1024*1024);
        for (i = 0; i < 1024*1024/8; ++i)
        {
            int k;
            uint64_t tmp = *data++;
            for (k = 0; k < 1024; ++k)
            {
                blksum *= tmp++;
                blksum += tmp++;
                blksum *= tmp++;
                blksum ^= tmp++;
                blksum *= tmp++;
            }
        }
        chksum ^= blksum;
    }

    return chksum;
}
