int get_min_val_entry(double* mat, int size) {

    int min_val_entry = 0;

    for (int i = 0; i < size; i++) {
        if (mat[i] < mat[min_val_entry])
            min_val_entry = i;
    }
    return min_val_entry;
}

int get_max_val_entry(double* mat, int size) {

    int max_val_entry = 0;

    for (int i = 0; i < size; i++) {
        if (mat[i] > mat[max_val_entry])
            max_val_entry = i;
    }
    return max_val_entry;
}