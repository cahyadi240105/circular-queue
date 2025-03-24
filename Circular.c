#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"
#include "import.h"
#include "export.h"

void formatHarga(int harga){
    char str[20];
    sprintf(str,"%d",harga);
    int len = strlen(str);
    int count = 0;
    char result[30];
    int j = 0;

    for (int i = len - 1; i >= 0; i--){
        result[j++] = str[i];
        count++;
        if (count == 3 && i != 0){
            result[j++] = '.';
            count = 0;
        }
    }

    result[j] = '\0';

    for (int i = 0; i < j / 2; i++){
        char temp = result[i];
        result[i] = result[j - i - 1];
        result[j - i - 1] = temp;
    }

    printf("Rp. %s\n", result);
}

void tampilkanPilihan(){
    printf("A001.Tampilan awal Menu Masakan");
    printf("Menu Masakan\n");
    printf("[\033[34m1\033[0m] Input Data Baru\n");
    printf("[\033[34m2\033[0m] Import Data Baru\n");
    printf("[\033[34m3\033[0m] Browse Data\n");
    printf("[\033[34m4\033[0m] Export ke CSV\n");
    printf("[\033[34m9\033[0m] Berhenti Program\n");
    printf("Pilihan anda :");
}

int cekTanggalValid(const char *tanggal){
    if (strlen(tanggal) != 8 || tanggal[2] != '-' || tanggal[5] != '-')
        return 0;

    int hari = atoi(tanggal);
    int bulan = atoi(&tanggal[3]);
    int tahun = atoi(&tanggal[6]);
    int hariDalamBulan[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (tahun % 4 == 0 && (tahun % 100 != 0 || tahun % 400 == 0))
        hariDalamBulan[1] = 29; 
    if (bulan < 1 || bulan > 12 || hari < 1 || hari > hariDalamBulan[bulan - 1])
        return 0;

    return 1; 
}

void tambahData(Node **head){
    char lanjut;
    do{
        Makanan masakan;
        printf("\nMasukan Nama Masakan: ");
        scanf(" %[^\n]", masakan.nama);
        printf("Masukan Harga Jual: Rp. ");
        scanf("%d", &masakan.harga);
        do{
            printf("Masukan Tanggal Produksi hari-bulan-tahun (dd-mm-yy): ");
            scanf("%s", masakan.tanggalProduksi);
            if (!cekTanggalValid(masakan.tanggalProduksi)){
                printf("E002. Error Tanggal Produksi\n");
                printf("Maaf, tanggal salah. Format yang diperbolehkan dd-mm-yy\n");
                printf("Silahkan ulangi penginputan tanggal produksi.\n");
            }
        } while (!cekTanggalValid(masakan.tanggalProduksi));

        Node *baru = (Node *)malloc(sizeof(Node));
        baru->makanan = masakan;
        baru->berikut = NULL;
        baru->sebelum = NULL;

        if (*head == NULL){
            *head = baru;
        }else{
            Node *temp = *head;
            while (temp->berikut)
                temp = temp->berikut;
            temp->berikut = baru;
            baru->sebelum = temp;
        }

        printf("Masih ada data? [Y / T ]: ");
        scanf(" %c", &lanjut);
    } while (lanjut == 'Y' || lanjut == 'y');
}

void lihatData(Node *head){
    if (!head){
        printf("Belum ada data.\n");
        return;
    }

    Node *sekarang = head;
    int total = 0, posisi = 1;
    Node *temp = head;
    while (temp){
        total++;
        temp = temp->berikut;
    }

    char aksi;
    do{
        printf("\nA004. Tampilan Browse Data\n");
        printf("Data ke %d dari %d\n", posisi, total);
        printf("Nama masakan : %s\n", sekarang->makanan.nama);
        printf("Harga : ");
        formatHarga(sekarang->makanan.harga); 
        printf("Tanggal Produksi : %s\n", sekarang->makanan.tanggalProduksi);
        printf("Pilihan P / N / Q : ");
        scanf(" %c", &aksi);

        if (aksi == 'P' || aksi == 'p'){
            sekarang = sekarang->sebelum ? sekarang->sebelum : head;
            posisi = sekarang->sebelum ? posisi - 1 : total;
        }else if (aksi == 'N' || aksi == 'n'){
            sekarang = sekarang->berikut ? sekarang->berikut : head;
            posisi = sekarang->berikut ? posisi + 1 : 1;
        }else if (aksi == 'Q' || aksi == 'q'){
            break;
        }else{
            printf("E004. Error Pilihan\n");
            printf("Pilihan hanya boleh P untuk data sebelum, N untuk data sesudah atau Q untuk berhenti browsing data.\n");
        }
    } while (1);
}

int main(){
    Node *head = NULL;
    int pilihan;
    do{
        tampilkanPilihan();
        scanf("%d", &pilihan);

        switch (pilihan){
        case 1:
            tambahData(&head);
            break;
        case 2:{
            char file[100];
            printf("A003. Tampilan Import Data Baru\n");
            printf("Masukan path dan nama file CSV: ");
            scanf(" %s", file);
            if (!bacaDariFile(&head, file))
                printf("E003. Error Import Data Baru\n");
            break;
        }
        case 3:
            lihatData(head);
            break;
        case 4:{
            char file[100];
            printf("A005. Tampilan Export ke file CSV\n");
            printf("Masukan path dan nama file untuk di export data: ");
            scanf(" %s", file);
            if (!simpanKeFile(head, file))
                printf("E005. Error Export\n");
            break;
        }
        case 9:
            printf("A006. Tampilan Berhenti\n");
            printf("Terima kasih sudah menggunakan aplikasi keren ini\n");
            break;
        default:
            printf("E001. Error Pilihan\n");
            printf("Maaf, pilihan salah. Hanya boleh 1, 2, 3, 4 atau 9. Silahkan ulangi pilihan menu.\n");
            break;
        }
    } while (pilihan != 9);

    Node *sekarang = head;
    while (sekarang){
        Node *hapus = sekarang;
        sekarang = sekarang->berikut;
        free(hapus);
    }
    return 0;
}
