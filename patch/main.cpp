#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    FILE* input = fopen("CRCK_ME2.COM", "rb+");
    if (!input) {
        perror("Error opening file!\n");
        return 1;
    }

    fseek(input, 0x37, SEEK_SET);
    
    uint8_t write_data[2];
    write_data[0] = 0xEB;
    
    write_data[1] = 0x47 - 0x37 - 0x2;

    size_t bytes_written = fwrite(write_data, 1, 2, input);
    if (bytes_written != 2) {
        perror("Error writing to file!\n");
        fclose(input);
        return 1;
    }

    printf("Successfully execution.\n");
    fclose(input);
    return 0;
}