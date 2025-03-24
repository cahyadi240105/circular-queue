#include <stdio.h>
#include <stdlib.h>
#include "import.h"

int bacaDariFile(Node** head, const char* namaFile) {
    FILE* file = fopen(namaFile, "r");
    if (!file) return 0;

    Makanan masakan;
    while (fscanf(file, "%[^,],%d,%s\n", masakan.nama, &masakan.harga, masakan.tanggalProduksi) == 3) {
        Node* baru = (Node*)malloc(sizeof(Node));
        baru->makanan = masakan;
        baru->berikut = NULL;
        baru->sebelum = NULL;

        if (!*head) {
            *head = baru;
        } else {
            Node* temp = *head;
            while (temp->berikut) temp = temp->berikut;
            temp->berikut = baru;
            baru->sebelum = temp;
        }
    }

    fclose(file);
    return 1;
}
