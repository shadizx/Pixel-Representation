// author: Shadi Zoldjalali
// date: July 23, 2021
// description: making an image from a struct and applying functions to them.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "t0imgr.h"


//Task 01.1: create
// input: rows and cols
// output:a new image with that many rows and cols
imgr_t *imgr_create(unsigned int rows, unsigned int cols)
{
    assert(rows >= 0 && cols >= 0);
    imgr_t *ptr = malloc(sizeof(imgr_t));
    if (ptr != NULL)
    {
        ptr->pixels = malloc(rows * sizeof(uint8_t *));
        if (ptr->pixels != NULL)
        {
            ptr->rows = rows;
            ptr->cols = cols;
            ptr->reserved_rows = rows;
            ptr->reserved_cols = cols;
            for (int i = 0; i < rows; i++)
            {
                ptr->pixels[i] = malloc(cols * sizeof(uint8_t));
            }

            //setting all values to 0 in the matrix to show that its empty
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    ptr->pixels[i][j] = 0;
                }
            }
            return ptr;
        }
    }
    return NULL;
}

//Task 01.2: destroy
// input: an image
// output: nothing, just destroys the image
void imgr_destroy(imgr_t *im)
{
    if (im != NULL)
    {
        if (im->pixels != NULL)
        {
            for (int i = 0; i < im->reserved_rows; i++)
            {
                free(im->pixels[i]);
            }
            free(im->pixels);
        }
        free(im);
    }
}

//Task 2: Append
imgr_result_t imgr_append(imgr_t *im, int expand_row, int val)
{
    //if array has space

    //if there is space in rowsxcols
    for (int i = 0; i < im->rows; i++)
    {
        for (int j = 0; j < im->cols; j++)
        {
            if (im->pixels[i][j] == 0)
            {
                im->pixels[i][j] = val;
                return IMGR_OK;
            }
        }
    }

    //if there isnt space in rowsxcols but there is space in reserved
    for (int i = 0; i < im->reserved_rows; i++)
    {
        for (int j = 0; j < im->reserved_cols; j++)
        {
            if (im->pixels[i][j] == 0)
            {
                im->pixels[i][j] = val;
                if (i >= im->rows)
                {
                    im->rows++;
                }
                else if (j >= im->cols)
                {
                    im->cols++;
                }
                return IMGR_OK;
            }
        }
    }

    //if array doesn't have space
    if (expand_row != 0) //expanding rows
    {
        im->pixels = realloc(im->pixels, sizeof(uint8_t *) * (2 * im->rows));
        if (im->pixels != NULL)
        {
            for (int i = 0; i < im->rows; i++)
            {
                im->pixels[i + im->reserved_rows] = malloc(sizeof(uint8_t) * im->cols);
            }
            for (int j = 0; j < im->reserved_cols; j++)
            {
                im->pixels[im->rows][j] = 0;
            }
            im->reserved_rows *= 2;
            im->pixels[im->rows][0] = val;
            im->rows += 1;
            return IMGR_OK;
        }
    }
    else //expanding cols
    {
        for (int i = 0; i < im->reserved_rows; i++)
        {
            im->pixels[i] = realloc(im->pixels[i], sizeof(uint8_t) * (2 * im->cols));
            im->pixels[i][im->cols] = 0;
        }
        im->reserved_cols *= 2;

        im->pixels[0][im->cols] = val;
        im->cols += 1;

        return IMGR_OK;
    }
    return IMGR_BADALLOC;
}

//helper functions for task 3:
void swap(imgr_t *im, int i, int j)
{
    int count = 0;
    //finding number of zeroes in each row
    for (int y = im->cols - 1; y >= 0; y--)
    {
        if (im->pixels[i][y] == 0)
        {
            count++;
        }
    }
    if (j == im->cols - 1 || count + j == im->cols)
    {
        im->pixels[i][j] = 0;
        return;
    }

    for (int y = im->cols - 1; y >= 0; y--)
    {
        if (im->pixels[i][y] != 0)
        {
            im->pixels[i][j] = im->pixels[i][y];
            im->pixels[i][y] = 0;
            return;
        }
    }
}

void rowremove(imgr_t *im, int i, int j)
{
    //check if a full row is all 0's

    int count = 0, rowtoremove = -1;
    for (int k = 0; k < im->cols; k++)
    {
        if (im->pixels[i][k] == 0)
        {
            count++;
        }
        if (count == im->cols)
        {
            rowtoremove = i;
        }
    }
    if (rowtoremove == -1) //if we dont need to remove a row
    {
        return;
    }
    if (rowtoremove == im->rows - 1)
    {
        for (int x = 0; x < im->rows; x++)
        {
            im->pixels[x] = realloc(im->pixels[x], sizeof(uint8_t) * (im->rows - 1));
        }
        im->rows--;
    }
    int r1 = rowtoremove;
    int r2 = im->rows - 1;
    for (int c = 0; c < im->cols; c++)
    {
        im->pixels[r1][c] = im->pixels[r2][c];
    }
    for (int x = 0; x < im->rows; x++)
    {
        im->pixels[x] = realloc(im->pixels[x], sizeof(uint8_t) * (im->rows - 1));
    }
    im->rows--;
    return;
}

void colremove(imgr_t *im, unsigned int i, unsigned int j)
{
    int count, coltoremove = -1;
    //check if a full col is all 0's
    for (int j = 0; j < im->cols; j++)
    {
        count = 0;
        for (int i = 0; i < im->rows; i++)
        {
            if (im->pixels[i][j] == 0)
            {
                count++;
            }
            if (count == im->rows)
            {
                for (int x = 0; x < im->cols; x++)
                {
                    im->pixels[x] = realloc(im->pixels[x], sizeof(uint8_t) * (im->cols - 1));
                }
                im->cols--;
                return;
            }
        }
    }
    return;
}

//Task 3: remove
imgr_result_t imgr_remove(imgr_t *im, unsigned int i, unsigned int j)
{
    if (i < 0 || i > im->rows)
        return IMGR_BADROW;
    else if (j < 0 || j > im->cols)
        return IMGR_BADCOL;
    else if (im->pixels[i][j] == 0 || (im->rows + im->cols <= 1))
        return IMGR_EMPTY;

    swap(im, i, j);
    rowremove(im, i, j);
    colremove(im, i, j);

    return IMGR_OK;
}

//task 4: save json file
int imgr_save_json(imgr_t *im, const char *filename)
{
    if (im == NULL || filename == NULL)
        return 2;

    FILE *f = fopen(filename, "w");

    if (f == NULL)
        return 1;
    fprintf(f, "[");
    if (im->rows == 0 || im->cols == 0)
    {
        fprintf(f, " ]");

        fclose(f);
        return 0; // runs successfully
    }
    for (int i = 0; i < im->rows; i++)
    {
        fprintf(f, " [");
        for (int j = 0; j < im->cols; j++)
        {
            if (j == (im->cols - 1))
            {
                fprintf(f, "%d", im->pixels[i][j]);
            }
            else
            {
                fprintf(f, "%d, ", im->pixels[i][j]);
            }
        }
        if (i == im->rows - 1)
            fprintf(f, "]");
        else
            fprintf(f, "],\n ");
    }
    fprintf(f, " ]");

    fclose(f);
    return 0; // runs successfully
}

//load json file
imgr_t *imgr_load_json(const char *filename)
{
    if (filename == NULL)
        return NULL;

    FILE *f = fopen(filename, "r");

    if (f == NULL)
        return NULL;

    char current, last = '0';
    int count = 0, count2 = 0, count3 = 0, temp = 0;
    int rows = 0, cols = 0;

    while (!feof(f))
    {
        current = getc(f);
        if (count == '\n')
            count3++;
        if (current == ']')
        {
            count2++;
        }
        if (count2 == 0)
        {
            if (current == ',')
                count++;
        }
        last = current;
    }
    rows = count2 - 1;
    if (count != 0)
        cols = count + 1;
    else if (count3 == 0 && rows > 0)
        cols = 1;
    else
        cols = 0;
    // printf("%d", rows);
    // printf("\n%d", cols);

    imgr_t *load = imgr_create(rows, cols);
    if (load == NULL)
        return NULL;

    fseek(f, 0, SEEK_SET);
    int data = 0;
    for (int i = 0; i < load->rows; i++)
    {
        for (int j = 0; j < load->cols; j++)
        {
            while (fscanf(f, "%d,", &data) != 1)
            {
                fseek(f, 1, SEEK_CUR);
            }
            imgr_append(load, 0, data);
        }
    }
    fseek(f, 0, SEEK_END);

    fclose(f);
    return load;
}
