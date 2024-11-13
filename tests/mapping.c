#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <query.h>
#include <mol.h>
#include <get_mapping.h>


char* read_file(const char* filepath) {
    FILE* fileptr;
    char* buffer;
    long filelen;

    fileptr = fopen(filepath, "rb");  // Open the file in binary mode
    if (!fileptr) {
        perror("File error");
    }
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    return buffer;
}


int main() {
    const char* q_filepath = "qbytes";
    const char* o_filepath = "molbytes";

    char* q_buffer = read_file(q_filepath);
    QMol query = create_QMol(q_buffer);

    char* o_buffer = read_file(o_filepath);
    OMol mol = create_OMol(o_buffer);

    int res = get_mapping(&query, &mol);
    
    printf("%u\n", res);

    return 0;
}
