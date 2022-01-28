// author: Shadi Zoldjalali
// date: July 26, 2021
// description: seeing whether or not tests will pass

#include "t0imgr.c"

// helper function that prints the content of the img
void print_img(imgr_t *im)
{
    if (im == NULL)
    {
        printf("Invalid img (null).\n");
        return;
    }

    printf("Printing img of row length %d and col length %d:\n", im->rows, im->cols);
    for (unsigned int i = 0; i < im->rows; i++)
    {
        for (unsigned int j = 0; j < im->cols; j++)
        {
            printf("%d ", im->pixels[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    //IMGR CREATE TEST
    printf("Testing imgr_create...\n\n");
    printf("Creating im by calling 'imgr_create(3, 3)'\n");

    imgr_t *im = NULL;
    im = imgr_create(3, 3);

    if (im == NULL)
    {
        printf("im == NULL\n");
        return 1; //exit with a non-zero value
    }

    printf("Printing empty arr im\n");
    print_img(im);

    printf("Populating im\n");

    int inc = 1;
    for (int i = 0; i < im->rows; i++)
    {
        for (int j = 0; j < im->cols; j++)
        {
            im->pixels[i][j] = inc;
            inc++;
        }
    }

    printf("Printing full arr im\n");
    print_img(im);

    printf("IMGR CREATE TEST PASSED\n\n");
    imgr_destroy(im);

    //IMGR APPEND TEST
    printf("Testing imgr_append...\n\n");

    imgr_t *am = NULL;
    am = imgr_create(3, 3);

    if (am == NULL)
    {
        printf("am == NULL\n");
        return 1; //exit with a non-zero value
    }

    printf("Printing empty arr am\n");
    print_img(am);

    printf("Populating am with imgr_append\n");

    inc = 1;
    for (unsigned int i = 0; i < am->rows; i++)
    {
        for (unsigned int j = 0; j < am->cols; j++)
        {
            if (imgr_append(am, 1, inc) == IMGR_BADALLOC)
            {
                printf("APPEND FAILED\n");
                return 1;
            }
            inc++;
        }
    }

    printf("Printing full arr am\n");
    print_img(am);
    printf("reserved rows are %d and reserved columns are %d\n", am->reserved_rows, am->reserved_cols);

    printf("Appending one more number now that it is full\n");
    printf("We will set expand row to 1 to double size of res rows\n");
    imgr_append(am, 1, inc);
    printf("\nPrinting full arr am after one more append\n");
    print_img(am);
    printf("Reserved rows are %d and reserved columns are %d\n", am->reserved_rows, am->reserved_cols);

    printf("Now testing whether reserved cols will double too when it is full\n");
    printf("Appending two numbers to get a full row\n");
    imgr_append(am, 0, inc + 1);
    imgr_append(am, 0, inc + 2);
    print_img(am);

    printf("We will set expand row to 0 to double size of res cols\n");
    imgr_append(am, 0, inc + 3);
    print_img(am);
    printf("Reserved rows are %d and reserved columns are %d\n", am->reserved_rows, am->reserved_cols);

    printf("IMGR APPEND TEST PASSED\n\n");
    imgr_destroy(am);

    //TESTING IMG REMOVE
    printf("Testing imgr_remove...\n\n");

    imgr_t *rm = NULL;
    rm = imgr_create(5, 3);

    if (rm == NULL)
    {
        printf("rm == NULL\n");
        return 1; //exit with a non-zero value
    }

    inc = 1;
    for (int i = 0; i < rm->rows; i++)
    {
        for (int j = 0; j < rm->cols; j++)
        {
            rm->pixels[i][j] = inc;
            inc++;
        }
    }
    printf("Using matrix am\n");
    print_img(am);
    printf("We are going to try to remove the second row by calling imgr_remove on all the row values\n");
    if (imgr_remove(rm, 1, 0) != IMGR_OK)
    {
        printf("REMOVING FAILED\n");
    }
    print_img(rm);

    imgr_remove(rm, 1, 1);
    print_img(rm);

    imgr_remove(rm, 1, 0);
    printf("Printing rm after removing the second row\n");
    print_img(rm);

    printf("\nNow we will try to remove a column by calling imgr_remove\n");
    imgr_remove(rm, 0, 1);
    print_img(rm);
    imgr_remove(rm, 1, 1);
    print_img(rm);
    imgr_remove(rm, 2, 1);
    print_img(rm);
    imgr_remove(rm, 3, 0);
    printf("Printing rm after removing the last column\n");
    print_img(rm);
    imgr_destroy(rm);

    printf("IMGR REMOVE TEST PASSED\n\n");

    //TESTING SAVE AND LOAD JSON
    printf("Testing imgr_save_json and imgr_load_json...\n\n");

    printf("Making a 10x10 image and saving to json...\n");
    imgr_t *sm = NULL;
    sm = imgr_create(4, 8);

    if (sm == NULL)
    {
        printf("sm == NULL\n");
        return 1; //exit with a non-zero value
    }

    inc = 1;
    for (int x = 0; x < sm->rows; x++)
    {
        for (int y = 0; y < sm->cols; y++)
        {
            sm->pixels[x][y] = inc;
            inc++;
        }
    }
    print_img(sm);
    if (imgr_save_json(sm, "jsontest") != 0)
    {
        printf("SAVE AS JSON FAILED");
        return 1;
    }
    printf("Image is saved as json, now we will load it with imgr_load_json\n");
    imgr_t *lm = imgr_load_json("jsontest");
    printf("Image loaded successfully, printing loaded image\n\n");
    print_img(lm);

    imgr_destroy(sm);
    imgr_destroy(lm);

    return 0;
}