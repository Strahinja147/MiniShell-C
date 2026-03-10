#include <stdio.h>

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "Upotreba: %s <izvor> <destinacija>\n", argv[0]);
        return 1;
    }

    const char* putanjaIzvora = argv[1];
    const char* putanjaDestinacije = argv[2];

    FILE* izvorFajl = fopen(putanjaIzvora, "rb");
    if (izvorFajl == NULL) {
        perror("Greška pri otvaranju izvornog fajla");
        return 1;
    }

    FILE* destinacijaFajl = fopen(putanjaDestinacije, "wb");
    if (destinacijaFajl == NULL) {
        perror("Greška pri kreiranju odredišnog fajla");
        fclose(izvorFajl); 
        return 1;
    }

    char bafer[BUFSIZ];
    size_t procitanoBajta;

    while ((procitanoBajta = fread(bafer, 1, sizeof(bafer), izvorFajl)) > 0) {
        fwrite(bafer, 1, procitanoBajta, destinacijaFajl);
    }

    fclose(izvorFajl);
    fclose(destinacijaFajl);

    return 0;
}