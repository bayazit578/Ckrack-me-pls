#include "patch.h"

bool patch_file(FILE *input) {
    fseek(input, 0x37, SEEK_SET);

    uint8_t write_data[2] = {0xEB, 0x47 - 0x37 - 0x2};

    size_t bytes_written = fwrite(write_data, 1, 2, input);
    if (bytes_written != 2) {
        perror("Error writing to file!\n");
        fclose(input);
        return true;
    }

    printf("Successfully patched.\n");
    fclose(input);

    return false;
}