#include <stdio.h>
#include "export.h"

int simpanKeFile(Node *head, const char *namaFile)
{
    FILE *file = fopen(namaFile, "w");
    if (!file)
        return 0;

    while (head)
    {
        fprintf(file, "%s,%d,%s\n", head->makanan.nama, head->makanan.harga, head->makanan.tanggalProduksi);
        head = head->berikut;
    }
    fclose(file);
    return 1;
}
