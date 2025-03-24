#ifndef DATA_H
#define DATA_H

typedef struct
{
    char nama[100];
    int harga;
    char tanggalProduksi[11];
} Makanan;

typedef struct Node
{
    Makanan makanan;
    struct Node *berikut;
    struct Node *sebelum;
} Node;

#endif
